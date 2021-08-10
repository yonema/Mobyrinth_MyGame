/*!
 * @brief	PBRのモデルシェーダー。
 */


 ///////////////////////////////////////////////////
 // 定数
 ///////////////////////////////////////////////////
static const int Max_DirectionLight = 4;	//ディレクションライトの最大数
static const int Max_PointLight = 32;		//ポイントライトの最大数
static const int Max_SpotLight = 4;			//スポットライトの最大数
static const float PI = 3.1415926f;			//π
static const int Max_ShadowMap = 5;			//シャドウマップの最大数

///////////////////////////////////////////////////
// 構造体
///////////////////////////////////////////////////

//ディレクションライトの構造体
//Light.hと同じ構造体にする
struct SDirectionLight
{
	float4 color;
	float3 direction;
	float pad;
};

//ポイントライトのパラメータの構造体
//Light.hと同じ構造体にする
struct SPointLight
{
	float3 ptPosition;		//位置。
	float ptRange;			//影響範囲。
	float4 ptColor;			//カラー。
};

//スポットライトのパラメータ構造体
//Light.hと同じ構造体にする
struct SSpotLight
{
	float3 position;	//座標
	float range;		//影響範囲
	float4  color;		//カラー
	float3 direction;	//射出方向
	float angle;		//射出角度
};

//シャドウマップ用のパラメータ構造体
//Light.hと同じ構造体にする
struct ShadowParam
{
	float4x4 mLVP;		//ライトビュープロジェクション
	float3 lightPos;	//ライトの位置
};

//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
	int4  Indices  	: BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn {
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//モデルの法線ベクトル。
	float3 tangent  : TANGENT;		//接ベクトル
	float3 biNormal : BINORMAL;		//従法線
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn {
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//スクリーン空間でのモデルの法線ベクトル。
	float3 tangent		: TANGENT;		//接ベクトル
	float3 biNormal 	: BINORMAL;		//従法線
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;	//ワールド空間でのピクセルの座標。
	float4 posInProj	: TEXCOORD2;	//正規化スクリーン座標系の座標
	float4 posInLVP[Max_ShadowMap]: TEXCOORD3;
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////

//モデル用の共通定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float4 emissionColor;
	float4 mulColor;
	int outLineFlag;
	int shadowReceiverFlag;
	int stealthFlag;
};

//LightManager用の定数バッファ
//このコンスタントバッファには、どのモデルでも同じ値が渡される
cbuffer LightManagerCb : register(b1)
{
	float3 eyePos;			//視点
	int numDirectionLight;	//ディレクションライトの数。
	float3 ambientLight;	//アンビエントライト。
	int numPointLight;		//ポイントライトの数。
	float specPow;			//スペキュラの絞り
	int numShadow;			//シャドウマップの数
	int numSpotLight;		//スポットライトの数
}

cbuffer DirectionLightCb : register(b2)
{
	SDirectionLight directionLight[Max_DirectionLight];
}

cbuffer PointLightCb : register(b3)
{
	SPointLight pointLight[Max_PointLight];
}

cbuffer SpotLightCb : register(b4)
{
	SSpotLight spotLight[Max_SpotLight];
}

cbuffer ShadowParamCb : register(b5)
{
	ShadowParam shadowParam[Max_ShadowMap];
}


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線マップ
Texture2D<float4> g_specularMap : register(t2);			//スペキュラマップ。
														//rgbにスペキュラカラー、aに金属度。
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。

Texture2D<float4> g_shadowMap : register(t10);		//シャドウマップ
Texture2D<float4> g_depthTexture : register(t11);	//深度テクスチャ
TextureCube<float4> g_skyCubeMap : register(t12);	//スカイキューブ
sampler g_sampler : register(s0);	//サンプラステート。

///////////////////////////////////////////
//関数宣言
///////////////////////////////////////////
float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv);
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin);
float4 SpecialColor(float4 albedoColor, float3 viewNormal, float3 normal);
bool IsOnOutLine(float4 posInProj,float thickness);
////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

//法線マップから法線を得る
float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv)
{
	float3 binSpaceNormal = g_normalMap.SampleLevel(g_sampler, uv, 0.0f).xyz;
	binSpaceNormal = (binSpaceNormal * 2.0f) - 1.0f;

	float3 newNormal = tangent * binSpaceNormal.x + biNormal * binSpaceNormal.y + normal * binSpaceNormal.z;

	return newNormal;
}


/// <summary>
//スキン行列を計算する。
/// </summary>
float4x4 CalcSkinMatrix(SSkinVSIn skinVert)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += g_boneMatrix[skinVert.Indices[i]] * skinVert.Weights[i];
        w += skinVert.Weights[i];
    }
    
    skinning += g_boneMatrix[skinVert.Indices[3]] * (1.0f - w);
	
    return skinning;
}

/// <summary>
//	輪郭線を描画する
/// </summary>
bool IsOnOutLine(float4 posInProj, float thickness)
{
	//輪郭線を引くか？
	if (!outLineFlag)
		//引かないなら、falseを戻す
		return false;


	// 近傍8テクセルの深度値を計算して、エッジを抽出する
	float2 uv = posInProj.xy * float2(0.5f, -0.5f) + 0.5f;

	//float thickness = 2.0f;
	float2 uvOffset[8] =
	{
		float2(	0.0f				,  thickness / 720.0f),
		float2(	0.0f				, -thickness / 720.0f),
		float2(	thickness / 1280.0f	,				 0.0f),
		float2(-thickness / 1280.0f	,				 0.0f),
		float2(	thickness / 1280.0f	,  thickness / 720.0f),
		float2(-thickness / 1280.0f	,  thickness / 720.0f),
		float2(	thickness / 1280.0f	, -thickness / 720.0f),
		float2(-thickness / 1280.0f	, -thickness / 720.0f),
	};
	//深度テクスチャ
	Texture2D<float4> depthTex = g_depthTexture;
	//現在のピクセルの深度を出す
	float depth = depthTex.Sample(g_sampler, uv).x;

	float depth2 = 0.0f;
	for (int i = 0; i < 8; i++)
	{
		// 近傍8テクセルの深度値を加算して
		depth2 += depthTex.Sample(g_sampler, uv + uvOffset[i]).x;
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
	float3 normal = g_depthTexture.Sample(g_sampler, uv).yzw;
	float3 normal2 = normal;

	for (int i = 0; i < 8; i++)
	{
		// 近傍8テクセルの頂点の法線をだして
		normal2 = g_depthTexture.Sample(g_sampler, uv + uvOffset[i]).yzw;
		//どれか一つでも頂点の内積が一定より小さかったら
		float inner = dot(normal, normal2);
		if (inner < 0.5f)
			//輪郭線を引く
			return true;
	}



	//輪郭線を引かない
	return false;
}


static const int pattern[4][4] =
{
	{0,32,8,40},
	{48,16,56,24},
	{12,44,4,36},
	{60,28,52,20},
};


/// <summary>
/// スキンなしメッシュ用の頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, false);
}


/// <summary>
/// スキンありメッシュの頂点シェーダーのエントリー関数。
/// </summary>
SPSIn VSSkinMain(SVSIn vsIn)
{
	return VSMainCore(vsIn, true);
}


/// <summary>
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	//ピクセルシェーダに渡すデータ
	SPSIn psIn;

	//ワールド行列
	float4x4 m;

	if( hasSkin )
	{
		//スキンあり
		m = CalcSkinMatrix(vsIn.skinVert);
	}
	else
	{
		//スキンなし
		m = mWorld;
	}

	//オブジェクト座標をワールド座標に変換
	psIn.pos = mul(m, vsIn.pos);
	//ワールド座標を保持しておく
	psIn.worldPos = psIn.pos;
	float4 worldPos = psIn.pos;
	//ワールド座標をビュー座標に変換
	psIn.pos = mul(mView, psIn.pos);
	//ビュー座標をプロジェクション座標に変換
	psIn.pos = mul(mProj, psIn.pos);

	//頂点法線
	psIn.normal = normalize(mul(m, vsIn.normal));
	//接ベクトル
	psIn.tangent = normalize(mul(m, vsIn.tangent));
	//従法線
	psIn.biNormal = normalize(mul(m, vsIn.biNormal));
	//UV
	psIn.uv = vsIn.uv;


	for (int i = 0; i < numShadow; i++)
	{
		//ライトビュースクリーン空間の座標を計算する。
		psIn.posInLVP[i] = mul(shadowParam[i].mLVP, worldPos);

		//頂点のライトから見た深度値を計算する。
		psIn.posInLVP[i].z = length(worldPos.xyz - shadowParam[i].lightPos) / 10000.0f;
	}

	// 頂点の正規化スクリーン座標系の座標をピクセルシェーダーに渡す
	//計算はピクセルシェーダーの方で行う
	psIn.posInProj = psIn.pos;


	return psIn;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//ビュー空間での法線を計算
	float3 viewNormal = normalize(mul(mView, psIn.normal));
	//頂点の正規化スクリーン座標系の座標を計算する
	psIn.posInProj.xy /= psIn.posInProj.w;

	//法線を計算。
	float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);
	//アルベドカラーをサンプリングする
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	//アウトラインにディザリングを行うか？
	bool ditherFlag = false;
	//モデルが半透明か透明だったら、ディザリングを行う。
	if (mulColor.w < 1.0f)
		ditherFlag = true;

	//アウトラインの太さ
	float thickness =2.0f;
	//ディザリグを行う場合は太くする
	if (ditherFlag)
		thickness = 4.0f;
	
	//輪郭を描画するか？
	if (IsOnOutLine(psIn.posInProj, thickness))
	{
		//アウトラインを描画する

		//ディザリングを行うか？
		if (ditherFlag)
		{
			//ディザリングを行う
			int x = (int)fmod(abs(psIn.pos.x), 4.0f);
			int y = (int)fmod(abs(psIn.pos.y), 4.0f);

			int dither = pattern[x][y];

			if (dither >= 30)
				return float4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else
			//ディザリングを行わない
			//アウトラインを黒色で出す
			return float4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	float4 finalColor = SpecialColor(albedoColor, viewNormal, normal);

	if (shadowReceiverFlag >= 1)
	{
		for (int shadowNo = 0; shadowNo < numShadow; shadowNo++)
		{
			//ライトビュースクリーン空間からUV空間に座標変換。
			float2 shadowMapUV = psIn.posInLVP[shadowNo].xy / psIn.posInLVP[shadowNo].w;
			shadowMapUV *= float2(0.5f, -0.5f);
			shadowMapUV += 0.5f;

			//ライトビュースクリーン空間でのZ値を計算する。
			float zInLVP = psIn.posInLVP[shadowNo].z;

			if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
				&& shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f
				) {
				//step-13 シャドウレシーバーに影を落とす。
				float2 shadowValue = g_shadowMap.Sample(g_sampler, shadowMapUV).rg;
				if (zInLVP > shadowValue.r + 0.0001)
				{
					float depth_sq = shadowValue.x * shadowValue.x;
					float variance = min(max(shadowValue.y - depth_sq, 0.0001f), 1.0f);
					float md = zInLVP - shadowValue.x;
					float lit_factor = variance / (variance + md * md);
					float3 shadowColor = finalColor.xyz * 0.5f;
					finalColor.xyz = lerp(shadowColor, finalColor.xyz, lit_factor);
				}

			}
		}
	}

	return finalColor;

}

float4 SpecialColor(float4 albedoColor, float3 viewNormal, float3 normal)
{
	float4 lig = albedoColor;

	//自己発光色
	lig.xyz += emissionColor.xyz;
	lig *= mulColor;
	
	float4 color = g_skyCubeMap.Sample(g_sampler, normal * -1.0f);

	//リム
	//輪郭を光らせる
	//法線のZ成分が多いほどリムが弱くなる
	float limPower = pow( 1.0f - abs(viewNormal.z), 5.0f );
	lig.xyz += color * limPower * 0.8f;
	

	return lig;

}