


///////////////////////////////////////////////////
// 構造体
///////////////////////////////////////////////////

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
};




////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////

//モデル用の共通定数バッファ
cbuffer ModelCb : register(b0) {
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float4 emissionColor;
	int shadowReceiverFlag;
};


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> g_albedo : register(t0);				//アルベドマップ
Texture2D<float4> g_normalMap : register(t1);			//法線マップ
Texture2D<float4> g_specularMap : register(t2);			//スペキュラマップ。
														//rgbにスペキュラカラー、aに金属度。
StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。

//Texture2D<float4> g_skyCubeMap : register(t10);
TextureCube<float4> g_skyCubeMap : register(t10);
sampler g_sampler : register(s0);	//サンプラステート。

///////////////////////////////////////////
//関数宣言
///////////////////////////////////////////
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin);



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

	if (hasSkin)
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

	return psIn;
}


float4 PSMain(SPSIn psIn) : SV_Target0
{
	float4 color = g_skyCubeMap.Sample(g_sampler, psIn.normal * -1.0f);
	color.xyz += emissionColor.xyz;

	//return float4(1.0f, 0.0f, 0.0f, 1.0f);

	return color;
}