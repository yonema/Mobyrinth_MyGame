

cbuffer cb : register(b0) {
	float4x4 mvp;		
	float4 mulColor;	
};

cbuffer IBLCb : register(b1)
{
	float3 eyePos;
	float IBLLuminance;
	float IBLRate;
}

struct VSInput {
	float4 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> albedTexture		: register(t0);		//アルベド
Texture2D<float4> normalTexture		: register(t1);		//法線
Texture2D<float4> viewNormalTexture	: register(t2);		//ビュー座標系の法線
Texture2D<float4> posInWRDTexture	: register(t3);		//ビュー座標系の法線
Texture2D<float4> posInLVPTexture	: register(t4);		//ライトビュープロジェクション座標系の座標
Texture2D<float4> posInProjTexture	: register(t5);		//プロジェクション座標系の座標
Texture2D<float4> emissionColorTexture	: register(t6);	//自己発光色

Texture2D<float4> g_shadowMap		: register(t7);		//シャドウマップ
Texture2D<float4> g_depthTexture	: register(t8);		//深度テクスチャ
TextureCube<float4> g_skyCubeMap	: register(t9);		//スカイキューブ

sampler Sampler : register(s0);

//関数宣言
bool IsOnOutLine(float4 posInProj, float thickness, float outlineFlag);
float4 IBLColor(float4 albedoColor, float3 viewNormal, float3 normal, float3 toEye);


static const int pattern[4][4] =
{
	{0,32,8,40},
	{48,16,56,24},
	{12,44,4,36},
	{60,28,52,20},
};

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = mul(mvp, In.pos);
	psIn.uv = In.uv;
	return psIn;
}

float4 PSMain(PSInput In) : SV_Target0
{
	//アルベドをアルベドテクスチャからサンプリング
	float4 albedo = albedTexture.Sample(Sampler, In.uv);

	//アウトラインにディザリングを行うか？
	bool ditherFlag = false;
	//完全に透明だったらアウトラインにディザリングをかける
	if (albedo.w <= 0.0f)
		ditherFlag = true;

	//アウトラインの太さ
	float thickness = 2.0f;
	//ディザリグを行う場合は太くする
	if (ditherFlag)
		thickness = 4.0f;

	//プロジェクション座標系の座標をテクスチャからサンプリング
	float4 posInProj = posInProjTexture.Sample(Sampler, In.uv);
	//頂点の正規化スクリーン座標系の座標を計算する
	//posInProj.xy /= posInProj.w;

	//法線を法線マップからサンプリング
	float4 normal = normalTexture.Sample(Sampler, In.uv);

	//輪郭を描画するか？
	if (IsOnOutLine(posInProj, thickness, normal.w))
	{
		//アウトラインを描画する

		//ディザリングを行うか？
		if (ditherFlag)
		{
			//ディザリングを行う
			int x = (int)fmod(abs(posInProj.x), 4.0f);
			int y = (int)fmod(abs(posInProj.y), 4.0f);

			int dither = pattern[x][y];

			if (dither >= 30)
				return float4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else
			//ディザリングを行わない
			//アウトラインを黒色で出す
			return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	//ビュー座標系の法線をテクスチャからサンプリング
	float4 viewNormal = viewNormalTexture.Sample(Sampler, In.uv);

	//最終カラー
	float4 finalColor = albedo;

	//ディザリングフラグがたっていないか？
	//つまり透明ではないか？
	if (!ditherFlag)
	{
		//透明ではない

		float3 toEye = eyePos - posInWRDTexture.Sample(Sampler, In.uv).xyz;
		toEye = normalize(toEye);

		//ライティング処理を行う
		float4 iblColor = IBLColor(albedo, viewNormal.xyz, normal.xyz, toEye);

		finalColor.xyz *= (1.0f - IBLRate);
		iblColor.xyz *= IBLRate;
		finalColor.xyz += iblColor.xyz;
		finalColor.xyz *= IBLLuminance;
		//自己発光カラーをテクスチャからサンプリング
		float4 emissionColor = emissionColorTexture.Sample(Sampler, In.uv);
		//自己発光色を加える
		finalColor.xyz += emissionColor.xyz;
	}
	else
	{
		//return float4(1.0f, 1.0f, 1.0f, 1.0f);
	}


	//if (shadowReceiverFlag >= 1)
	//{
	//	for (int shadowNo = 0; shadowNo < numShadow; shadowNo++)
	//	{
	//		//ライトビュースクリーン空間からUV空間に座標変換。
	//		float2 shadowMapUV = psIn.posInLVP[shadowNo].xy / psIn.posInLVP[shadowNo].w;
	//		shadowMapUV *= float2(0.5f, -0.5f);
	//		shadowMapUV += 0.5f;

	//		//ライトビュースクリーン空間でのZ値を計算する。
	//		float zInLVP = psIn.posInLVP[shadowNo].z;

	//		if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
	//			&& shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f
	//			) {
	//			//step-13 シャドウレシーバーに影を落とす。
	//			float2 shadowValue = g_shadowMap.Sample(g_sampler, shadowMapUV).rg;
	//			if (zInLVP > shadowValue.r + 0.0001)
	//			{
	//				float depth_sq = shadowValue.x * shadowValue.x;
	//				float variance = min(max(shadowValue.y - depth_sq, 0.0001f), 1.0f);
	//				float md = zInLVP - shadowValue.x;
	//				float lit_factor = variance / (variance + md * md);
	//				float3 shadowColor = finalColor.xyz * 0.5f;
	//				finalColor.xyz = lerp(shadowColor, finalColor.xyz, lit_factor);
	//			}

	//		}
	//	}
	//}

	return finalColor; 
}


/// <summary>
//	輪郭線を描画する
/// </summary>
bool IsOnOutLine(float4 posInProj, float thickness, float outlineFlag)
{
	//輪郭線を引くか？
	if (outlineFlag == 0.0f)
		//引かないなら、falseを戻す
		return false;

	posInProj.xy /= posInProj.w;
	// 近傍8テクセルの深度値を計算して、エッジを抽出する
	float2 uv = posInProj.xy * float2(0.5f, -0.5f) + 0.5f;

	//float thickness = 2.0f;
	float2 uvOffset[8] =
	{
		float2(0.0f				,  thickness / 720.0f),
		float2(0.0f				, -thickness / 720.0f),
		float2(thickness / 1280.0f	,				 0.0f),
		float2(-thickness / 1280.0f	,				 0.0f),
		float2(thickness / 1280.0f	,  thickness / 720.0f),
		float2(-thickness / 1280.0f	,  thickness / 720.0f),
		float2(thickness / 1280.0f	, -thickness / 720.0f),
		float2(-thickness / 1280.0f	, -thickness / 720.0f),
	};
	//深度テクスチャ
	Texture2D<float4> depthTex = g_depthTexture;
	//現在のピクセルの深度を出す
	float depth = depthTex.Sample(Sampler, uv).x;

	float depth2 = 0.0f;
	for (int i = 0; i < 8; i++)
	{
		// 近傍8テクセルの深度値を加算して
		depth2 += depthTex.Sample(Sampler, uv + uvOffset[i]).x;
	}
	//平均を取る
	depth2 /= 8.0f;

	//周りの深度値と現在のピクセルの深度値が一定より大きかったら
	if (abs(depth - depth2) > 0.00005f)
	{
		//輪郭線を引く
		return true;
	}


	//あった方とない方どっちがいいかな？
	//法線の内積から輪郭線を引くか決める

	//現在のピクセルの、頂点の法線を出す
	float3 normal = g_depthTexture.Sample(Sampler, uv).yzw;
	float3 normal2 = normal;

	for (int i = 0; i < 8; i++)
	{
		// 近傍8テクセルの頂点の法線をだして
		normal2 = g_depthTexture.Sample(Sampler, uv + uvOffset[i]).yzw;
		//どれか一つでも頂点の内積が一定より小さかったら
		float inner = dot(normal, normal2);
		if (inner < 0.5f)
			//輪郭線を引く
			return true;
	}

	//輪郭線を引かない
	return false;
}

float4 IBLColor(float4 albedoColor, float3 viewNormal, float3 normal, float3 toEye)
{
	float4 lig = albedoColor;

	normal = normal * 2.0f - 1.0f;

	float3 refV = reflect(toEye * -1.0f, normal);

	float4 color = g_skyCubeMap.SampleLevel(Sampler, refV, 12)/* * IBLLuminance*/;
	lig.xyz *= color.xyz;
	return lig;

}