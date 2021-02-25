/*!
 * @brief	シンプルなモデルシェーダー。
 */

static const int Max_DirectionLight = 4;	//ディレクションライトの最大数
static const int Max_PointLight = 4;		//ポイントライトの最大数
static const int Max_SpotLight = 4;			//スポットライトの最大数
////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////
//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};

////ディレクションライト用の定数バッファ
//cbuffer LightCb : register(b1)
//{
//	float3 dirLigDirection[Max_DirectionLight];	//ディレクションライトの方向
//	float4 dirLigColor[Max_DirectionLight];		//ディレクションライトの色
//	float3 eyePos;								//視点
//	float specPow;								//スペキュラの絞り
//	float3 ambientLight;						//環境光。
//}
//
////ポイントライト用の定数バッファ
//cbuffer PointLightCb : register(b2)
//{
//	float3 ptPosition;	//位置。
//	float ptRange;		//影響範囲。
//	float4 ptColor;		//カラー。
//}

struct SDirectionLight
{
	float4 dirLigColor;
	float3 dirLigDirection;
	float pad;
};

//LightManager用の定数バッファ
cbuffer LightManagerCb : register(b1)
{
	float3 eyePos;			//視点
	int numDirectionLight;	//ディレクションライトの数。
	float3 ambientLight;	//アンビエントライト。
	int numPointLight;		//ポイントライトの数。
	float specPow;			//スペキュラの絞り
}

cbuffer DirectionLightCb : register(b2)
{
	//float4 dirLigColor[Max_DirectionLight];		//カラー。
	//float3 dirLigDirection[Max_DirectionLight];	//位置。
	SDirectionLight directionLight[Max_DirectionLight];
}

////////////////////////////////////////////////
// 構造体
////////////////////////////////////////////////
//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn{
	int4  Indices  	: BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};
//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//モデルの法線ベクトル。
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//スクリーン空間でのモデルの法線ベクトル。
	float2 uv 			: TEXCOORD0;	//uv座標。
	float3 worldPos		: TEXCOORD1;
};

////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。
sampler g_sampler : register(s0);	//サンプラステート。

///////////////////////////////////////////
//関数宣言
///////////////////////////////////////////
float3 CalcLambertDiffuse(float3 lightDirection, float3 lightColor, float3 normal);
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal);
float3 CalcLigFromDirectionLight(SPSIn psIn, int n);
float3 CalcLigFromPointLight(SPSIn psIn, int n);

////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////

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
/// 頂点シェーダーのコア関数。
/// </summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	SPSIn psIn;
	float4x4 m;
	if( hasSkin ){
		m = CalcSkinMatrix(vsIn.skinVert);
	}else{
		m = mWorld;
	}
	psIn.pos = mul(m, vsIn.pos);
	psIn.worldPos = psIn.pos;

	psIn.pos = mul(mView, psIn.pos);
	psIn.pos = mul(mProj, psIn.pos);



	//psIn.normal = normalize(mul(m, vsIn.normal));
	//頂点法線をピクセルシェーダーに渡す。
	psIn.normal = mul(m, vsIn.normal);		//法線を回転させる。


	psIn.uv = vsIn.uv;

	return psIn;
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
SPSIn VSSkinMain( SVSIn vsIn ) 
{
	return VSMainCore(vsIn, true);
}
/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//ディレクションライトによるライティングを計算する。
	float3 directionLig = {0.0f,0.0f,0.0f};

	for (int i = 0; i < numDirectionLight; i++)
	{
		directionLig += CalcLigFromDirectionLight(psIn, i);
	}
	//directionLig += CalcLigFromDirectionLight(psIn, 1);
	//ポイントライトによるライティングを計算する。
	float3 pointLig = { 0.0f,0.0f,0.0f };

	for (int i = 0; i < numPointLight; i++)
	{
		pointLig += CalcLigFromPointLight(psIn, i);
	}

	//各種ライトの反射光を足し算して最終的な反射光を求める。
	float3 finalLig = directionLig + ambientLight/* + hemiLight*/ + pointLig;

	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	albedoColor.xyz *= finalLig;

	return albedoColor;
}


/// <summary>
/// ランバート拡散反射光を計算する。
/// </summary>
float3 CalcLambertDiffuse(float3 lightDirection, float4 lightColor, float3 normal)
{
	//ピクセルの法線とライトの方向の内積を計算する。
	float t = dot(normal, lightDirection) * -1.0f;
	//内積の値を0以上の値にする。
	t = max(0.0f, t);
	//拡散反射光を計算する。
	return lightColor * t;
}
/// <summary>
/// フォン鏡面反射光を計算する。
/// </summary>
float3 CalcPhongSpecular(float3 lightDirection, float3 lightColor, float3 worldPos, float3 normal)
{
	//反射ベクトルを求める。
	float3 refVec = reflect(lightDirection, normal);
	//光が当たったサーフェイスから視点に伸びるベクトルを求める。
	float3 toEye = eyePos - worldPos;
	toEye = normalize(toEye);
	//鏡面反射の強さを求める。
	float t = dot(refVec, toEye);
	//鏡面反射の強さを0以上の数値にする。
	t = max(0.0f, t);
	//鏡面反射の強さを絞る。
	t = pow(t, 5.0f);
	//鏡面反射光を求める。
	return lightColor * t;
}

/// <summary>
/// ディレクションライトによる反射光を計算。
/// </summary
/// <param name="psIn">ピクセルシェーダーからの入力。</param>
float3 CalcLigFromDirectionLight(SPSIn psIn, int n)
{
	//ディレクションライトによるランバート拡散反射光を計算する。
	float3 diffDirection= CalcLambertDiffuse
	(directionLight[n].dirLigDirection, directionLight[n].dirLigColor, psIn.normal);
	//ディレクションライトによるフォン鏡面反射光を計算する。
	float3 specDirection= CalcPhongSpecular
	(directionLight[n].dirLigDirection, directionLight[n].dirLigColor, psIn.worldPos, psIn.normal);
	return diffDirection + specDirection;
}





/// <summary>
/// ポイントライトによる反射光を計算
/// </summary>
/// <param name="psIn">ピクセルシェーダーに渡されている引数</param>
float3 CalcLigFromPointLight(SPSIn psIn, int n)
{
	////step-7 このサーフェイスに入射しているポイントライトの光の向きを計算する。
	//float3 ligDir = psIn.worldPos - ptPosition;
	////正規化して大きさ１のベクトルにする。
	//ligDir = normalize(ligDir);

	////step-7 減衰なしのランバート拡散反射光を計算する。
	//float3 diffPoint = CalcLambertDiffuse(
	//	ligDir, 		//ライトの方向
	//	ptColor,	 	//ライトのカラー
	//	psIn.normal		//サーフェイスの法線
	//);
	////step-9 減衰なしのフォン鏡面反射光を計算する。
	//float3 specPoint = CalcPhongSpecular(
	//	ligDir, 			//ライトの方向。
	//	ptColor,		 	//ライトのカラー。
	//	psIn.worldPos, 		//サーフェイズのワールド座標。
	//	psIn.normal			//サーフェイズの法線。
	//);

	////step-10 距離による影響率を計算する。
	////ポイントライトとの距離を計算する。
	//float3 distance = length(psIn.worldPos - ptPosition);

	////影響率は距離に比例して小さくなっていく。
	//float affect = 1.0f - 1.0f / ptRange * distance;
	////影響力がマイナスにならないように補正をかける。
	//if (affect < 0.0f) {
	//	affect = 0.0f;
	//}
	////影響の仕方を指数関数的にする。今回のサンプルでは3乗している。
	//affect = pow(affect, 3.0f);

	////step-11 拡散反射光と鏡面反射光に減衰率を乗算して影響を弱める。
	//diffPoint *= affect;
	//specPoint *= affect;

	//return diffPoint + specPoint;

	return 0;
}