/*!
 * @brief	PBRのモデルシェーダー。
 */


 ///////////////////////////////////////////////////
 // 定数
 ///////////////////////////////////////////////////
static const int Max_DirectionLight = 4;	//ディレクションライトの最大数
static const int Max_PointLight = 4;		//ポイントライトの最大数
static const int Max_SpotLight = 4;			//スポットライトの最大数
static const float PI = 3.1415926f;			//π
static const int Max_ShadowMap = 5;

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
//シャドウマップ用のパラメータ構造体
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
	float4 posInLVP		: TEXCOORD2;
	//float4 exsample		: TEXCOORD3;
};

////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////

//モデル用の共通定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	int shadowReceiverFlag;
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
	int numShadow;
}

cbuffer DirectionLightCb : register(b2)
{
	SDirectionLight directionLight[Max_DirectionLight];
}

cbuffer ShadowParamCb : register(b3)
{
	ShadowParam shadowParam;
}


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線マップ
Texture2D<float4> g_specularMap : register(t2);			//スペキュラマップ。
														//rgbにスペキュラカラー、aに金属度。
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。

Texture2D<float4> g_shadowMap : register(t10);
sampler g_sampler : register(s0);	//サンプラステート。

///////////////////////////////////////////
//関数宣言
///////////////////////////////////////////
float3 GetNormal(float3 normal, float3 tangent, float3 biNormal, float2 uv);
float Beckmann(float m, float t);
float SpcFresnel(float f0, float u);
float CookTrranceSpecular(float3 L, float3 V, float3 N, float metaric);
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V);
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin);
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

//ベックマン分布を計算する。
float Beckmann(float m, float t)
{
	float t2 = t * t;
	float t4 = t * t * t * t;
	float m2 = m * m;
	float D = 1.0f / (4.0f * m2 * t4);
	D *= exp((-1.0f / m2) * (1.0f - t2) / t2);
	return D;
}

//フレネルを計算。Schlick近似を使用。
float SpcFresnel(float f0, float u)
{
	// from Schlick
	return f0 + (1 - f0) * pow(1 - u, 5);
}

/// <summary>
/// クックトランスモデルの鏡面反射を計算。
/// </summary>
/// <param name="L">光源に向かうベクトル</param>
/// <param name="V">視点に向かうベクトル</param>
/// <param name="N">法線ベクトル</param>
/// <param name="metaric">金属度</param>
float CookTrranceSpecular(float3 L, float3 V, float3 N, float metaric)
{
	float microfacet = 0.76f;
	//金属度を垂直入射の時のフレネル反射率として扱う。
	//金属度が高いほどフレネル反射は大きくなる。
	float f0 = metaric;
	//ライトに向かうベクトルと視線に向かうベクトルのハーフベクトルを求める。
	float3 H = normalize(L + V);

	//各種ベクトルがどれくらい似ているかを内積を利用して求める。
	float NdotH = saturate(dot(N, H));
	float VdotH = saturate(dot(V, H));
	float NdotL = saturate(dot(N, L));
	float NdotV = saturate(dot(N, V));

	//D項をベックマン分布を用いて計算する。
	float D = Beckmann(microfacet, NdotH);
	//F項をSchlick近似を用いて計算する。
	float F = SpcFresnel(f0, VdotH);
	//G項を求める。
	float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));
	//m項を求める。
	float m = PI * NdotV * NdotH;
	//ここまで求めた、値を利用して、クックトランスモデルの鏡面反射を求める。
	return max(F * D * G / m, 0.0);
}


/// <summary>
/// フレネル反射を考慮した拡散反射を計算。
/// </summary>
/// <remark>
/// この関数はフレネル反射を考慮した拡散反射率を計算します。
/// フレネル反射は、光が物体の表面で反射する現象のとこで、鏡面反射の強さになります。
/// 一方拡散反射は、光が物体の内部に入って、内部錯乱を起こして、拡散して反射してきた光のことです。
/// つまりフレネル反射が弱いときには、拡散反射が大きくなり、フレネル反射が強いときは、拡散反射が小さくなります。
/// 
/// </remark>
/// <param name="N">法線</param>
/// <param name="L">光源に向かうベクトル。光の方向と逆向きのベクトル。</param>
/// <param name="V">視線に向かうベクトル。</param>
/// <param name="roughness">粗さ。0～1の範囲。</param>
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
	//step-4 フレネル反射を考慮した拡散反射光を求める。
	// 光源に向かうベクトルと視線に向かうベクトルのハーフベクトルを求める。
	float3 H = normalize(L + V);


	// 3. 法線と光源に向かうベクトルがどれだけ似ているかを内積で求める。
	float dotNL = saturate(dot(N, L));

	float dotNV = saturate(dot(N, V));

	//法線と光源への方向に依存する拡散反射率と、法線と視点ベクトルに依存する拡散反射率を乗算して
	//最終的な拡散反射率を求めている。PIで除算しているのは正規化を行うため。
	return (dotNL * dotNV) / PI;
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

	//ライトビュースクリーン空間の座標を計算する。
	psIn.posInLVP = mul(shadowParam.mLVP, worldPos);

	//頂点のライトから見た深度値を計算する。
	psIn.posInLVP.z = length(worldPos.xyz - shadowParam.lightPos) / 10000.0f;


	return psIn;
}


/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//法線を計算。
	float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);
	
	//アルベドカラー、スペキュラカラー、金属度をサンプリングする。
	//アルベドカラー(拡散反射光)。
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);
	

	//スペキュラカラー(鏡面反射光)。
	float3 specColor = g_specularMap.SampleLevel(g_sampler, psIn.uv, 0).rgb;
	//金属度。
	float metaric = g_specularMap.Sample(g_sampler, psIn.uv).a;
	
	//視線に向かって伸びるベクトルを計算する。
	float3 toEye = normalize(eyePos - psIn.worldPos);

	//ライトのカラー
	float3 lig = 0;

	//ディレクションライトを計算
	for (int dirligNo = 0; dirligNo < numDirectionLight; dirligNo++) 
	{

		//ディズニーベースの拡散反射を実装する。

		//フレネル反射を考慮した拡散反射を計算。
		float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -directionLight[dirligNo].direction, toEye);
		//正規化ランバート拡散反射を求める。
		float NdotL = saturate(dot(normal, -directionLight[dirligNo].direction));
		float3 lambertDiffuse = directionLight[dirligNo].color * NdotL;
		//最終的な拡散反射光を計算する。
		float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;

		//クックトランスモデルを利用した鏡面反射率を計算する。

		//クックトランスモデルの鏡面反射率を計算する。
		float3 spec = CookTrranceSpecular(-directionLight[dirligNo].direction, toEye, normal, metaric) * directionLight[dirligNo].color;
		//金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白。
		//スペキュラカラーの強さを鏡面反射率として扱う。
		float specTerm = length(specColor.xyz);
		spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

		//鏡面反射率を使って、拡散反射光と鏡面反射光を合成する。
		//鏡面反射率が高ければ、拡散反射は弱くなる。
		lig += diffuse * (1.0f - specTerm) + spec;

	}

	//環境光による底上げ。
	lig += ambientLight * albedoColor;
	//最終的なカラー
	float4 finalColor = 1.0f;
	finalColor.xyz = lig;
	bool isShadow = false;
	if (shadowReceiverFlag >= 1)
	{
		//ライトビュースクリーン空間からUV空間に座標変換。
		float2 shadowMapUV = psIn.posInLVP.xy / psIn.posInLVP.w;
		shadowMapUV *= float2(0.5f, -0.5f);
		shadowMapUV += 0.5f;

		//ライトビュースクリーン空間でのZ値を計算する。
		float zInLVP = psIn.posInLVP.z;

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
				isShadow = true;
			}

		}
	}
	if (isShadow)
		return finalColor;

	return finalColor;
}

