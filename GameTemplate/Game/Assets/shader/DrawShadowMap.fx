/*!
 * @brief	シャドウマップ描画用のシェーダー
 */



//モデル用の定数バッファ
cbuffer ModelCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
	float4 emissionColor;
	float4 mulColor;
	int shadowReceiverFlag;
};

//step-8 影用のパラメータにアクセする定数バッファを定義。
cbuffer ShadowParamCb: register(b1)
{
	float4x4 mLVP;
	float3 lightPos;
};


//スキニング用の頂点データをひとまとめ。
struct SSkinVSIn {
	int4  Indices  	: BLENDINDICES0;
	float4 Weights  : BLENDWEIGHT0;
};

//頂点シェーダーへの入力。
struct SVSIn{
	float4 pos 		: POSITION;		//モデルの頂点座標。
	float3 normal	: NORMAL;		//法線。
	float2 uv 		: TEXCOORD0;	//UV座標。
	SSkinVSIn skinVert;				//スキン用のデータ。
};
//ピクセルシェーダーへの入力。
struct SPSIn{
	float4 pos 			: SV_POSITION;	//スクリーン空間でのピクセルの座標。
	float3 normal		: NORMAL;		//法線。
	float2 uv 			: TEXCOORD0;	//uv座標。
	float2 depth		: TEXCOORD1;	//ライト空間での座標。
};

///////////////////////////////////////////////////
// グローバル変数
///////////////////////////////////////////////////

StructuredBuffer<float4x4> g_boneMatrix : register(t3);	//ボーン行列。



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
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin);


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
/// <summary>
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin)
{
	//シャドウマップ描画用の頂点シェーダーを実装。
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

	float4 pos = mul(m, vsIn.pos);
	psIn.pos = mul(m, vsIn.pos);
	float3 worldPos = psIn.pos;
	
	psIn.pos = mul(mView, pos);
	psIn.pos = mul(mProj, psIn.pos);

	//step-9 頂点のライトから見た深度値と、ライトから見た深度値の２乗を計算する。
	psIn.depth.x = length(worldPos - lightPos) / 10000.0f;
	psIn.depth.y = psIn.depth.x * psIn.depth.x;


	psIn.uv = vsIn.uv;

	return psIn;
}

/// <summary>
/// シャドウマップ描画用のピクセルシェーダー。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	//step-10 ライトから見た深度値と、ライトから見た深度値の２乗を出力する。
	return float4(psIn.depth.x * mulColor.w, psIn.depth.y * mulColor.w,0.0f,1.0f);

}
