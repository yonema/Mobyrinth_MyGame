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
	float3 objPos		: TEXCOORD3;
	float4 posInLVP[Max_ShadowMap]: TEXCOORD4;
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
float Beckmann(float m, float t);
float SpcFresnel(float f0, float u);
float CookTrranceSpecular(float3 L, float3 V, float3 N, float3 N2, float metaric);
float CalcDiffuseFromFresnel(float3 N, float3 L, float3 V);
SPSIn VSMainCore(SVSIn vsIn, uniform bool hasSkin);
float4 SpecialColor(float4 albedoColor, float3 viewNormal, float3 normal);
bool IsOnOutLine(float4 posInProj,float thickness);
float SimplexNoise(float3 x);
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
float CookTrranceSpecular(float3 L, float3 V, float3 N, float3 N2, float metaric)
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
	if (NdotV == 0.0f)
		NdotV = saturate(dot(N2, V));

	//D項をベックマン分布を用いて計算する。
	float D = Beckmann(microfacet, NdotH);
	//F項をSchlick近似を用いて計算する。
	float F = SpcFresnel(f0, VdotH);
	//G項を求める。
	float G = min(1.0f, min(2 * NdotH * NdotV / VdotH, 2 * NdotH * NdotL / VdotH));
	//m項を求める。
	float m = PI * NdotV * NdotH;
	//ここまで求めた、値を利用して、クックトランスモデルの鏡面反射を求める。
	if (m == 0.0f)
		//0割り防止
		return 0.0f;
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

// ハッシュ関数
float hash(float n)
{
	return frac(sin(n) * 43758.5453);
}

// 3次元ベクトルからシンプレックスノイズを生成する関数
float SimplexNoise(float3 x)
{
	// The noise function returns a value in the range -1.0f -> 1.0f
	float3 p = floor(x);
	float3 f = frac(x);

	f = f * f * (3.0 - 2.0 * f);
	float n = p.x + p.y * 57.0 + 113.0 * p.z;

	return lerp(lerp(lerp(hash(n + 0.0), hash(n + 1.0), f.x),
		lerp(hash(n + 57.0), hash(n + 58.0), f.x), f.y),
		lerp(lerp(hash(n + 113.0), hash(n + 114.0), f.x),
			lerp(hash(n + 170.0), hash(n + 171.0), f.x), f.y), f.z);
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

	psIn.objPos = vsIn.pos;
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
	psIn.posInProj = psIn.pos;


	return psIn;
}

/// <summary>
/// ピクセルシェーダーのエントリー関数。
/// </summary>
float4 PSMain(SPSIn psIn) : SV_Target0
{
	float3 viewNormal = normalize(mul(mView, psIn.normal));
	psIn.posInProj.xy /= psIn.posInProj.w;

	//法線を計算。
	float3 normal = GetNormal(psIn.normal, psIn.tangent, psIn.biNormal, psIn.uv);
	float3 normal2 = psIn.normal;
	//アルベドカラー、スペキュラカラー、金属度をサンプリングする。
	//アルベドカラー(拡散反射光)。
	float4 albedoColor = g_albedo.Sample(g_sampler, psIn.uv);

	bool ditherFlag = false;
	if (mulColor.w < 1.0f)
		ditherFlag = true;

	float thickness =2.0f;
	if (ditherFlag)
		thickness = 4.0f;
	
	//輪郭を描画するか？
	if (IsOnOutLine(psIn.posInProj, thickness))
	{
		
		//区切り
		float separation =32.0f;
		// ピクセルのX座標を64で割った余りがワイプサイズ以下なら表示しない。
		float t = (int)fmod(abs(psIn.objPos.x), separation);


		float kill = 8.0f;// separation / 2.0f;
		//if ((t - kill) < 0)
		//{
		//	return float4(0.0f, 0.0f, 0.0f, 1.0f);
		//	t = (int)fmod(abs(psIn.objPos.y), separation);
		//	if ((t - kill) < 0)
		//	{
		//		t = (int)fmod(abs(psIn.objPos.z), separation);
		//		if ((t - kill) < 0)
		//			return float4(0.0f, 0.0f, 0.0f, 1.0f);

		//	}


		//}
		if (ditherFlag)
		{
			int x = (int)fmod(abs(psIn.pos.x), 4.0f);
			int y = (int)fmod(abs(psIn.pos.y), 4.0f);

			int dither = pattern[x][y];

			if (dither >= 30)
				return float4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		else
			return float4(0.0f, 0.0f, 0.0f, 1.0f);


	}

	return SpecialColor(albedoColor, viewNormal, normal);

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
		float3 spec = CookTrranceSpecular(-directionLight[dirligNo].direction, toEye, normal,normal2, metaric) * directionLight[dirligNo].color;
		//金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白。
		//スペキュラカラーの強さを鏡面反射率として扱う。
		float specTerm = length(specColor.xyz);
		spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

		//鏡面反射率を使って、拡散反射光と鏡面反射光を合成する。
		//鏡面反射率が高ければ、拡散反射は弱くなる。
		lig += diffuse * (1.0f - specTerm) + spec;

	}

	//ポイントライトを計算
	for (int ptLigNo = 0; ptLigNo < numPointLight; ptLigNo++)
	{
		//距離による影響率を計算する
		float3 distance = length(psIn.worldPos - pointLight[ptLigNo].ptPosition);
		float affect = 1.0f - 1.0f / pointLight[ptLigNo].ptRange * distance;

		//影響率が0以下だったら計算する必要なし
		if (affect <= 0.0f)
			continue;

		affect = pow(affect, 3.0f);

		float3 ligDir = psIn.worldPos - pointLight[ptLigNo].ptPosition;
		ligDir = normalize(ligDir);

		//ディズニーベースの拡散反射を実装する。

		//フレネル反射を考慮した拡散反射を計算。
		float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -ligDir, toEye);
		//正規化ランバート拡散反射を求める。
		float NdotL = saturate(dot(normal, -ligDir));
		float3 lambertDiffuse = pointLight[ptLigNo].ptColor * NdotL;
		//最終的な拡散反射光を計算する。
		float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;


		//クックトランスモデルを利用した鏡面反射率を計算する。

		//クックトランスモデルの鏡面反射率を計算する。
		float3 spec = CookTrranceSpecular(-ligDir, toEye, normal, normal2, metaric) * pointLight[ptLigNo].ptColor;
		//金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白。
		//スペキュラカラーの強さを鏡面反射率として扱う。
		float specTerm = length(specColor.xyz);
		spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

		//鏡面反射率を使って、拡散反射光と鏡面反射光を合成する。
		//鏡面反射率が高ければ、拡散反射は弱くなる。
		lig += (diffuse * (1.0f - specTerm) + spec) * affect;

	}

	//スポットライトを計算
	for (int spLigNo = 0; spLigNo < numSpotLight; spLigNo++)
	{
		//距離による影響率を計算する
		float3 distance = length(psIn.worldPos - spotLight[spLigNo].position);
		float affect = 1.0f - 1.0f / spotLight[spLigNo].range * distance;

		//影響率が0以下だったら計算する必要なし
		if (affect <= 0.0f)
			continue;

		affect = pow(affect, 3.0f);

		float3 ligDir = psIn.worldPos - spotLight[spLigNo].position;
		ligDir = normalize(ligDir);

		//ディズニーベースの拡散反射を実装する。

		//フレネル反射を考慮した拡散反射を計算。
		float diffuseFromFresnel = CalcDiffuseFromFresnel(normal, -ligDir, toEye);
		//正規化ランバート拡散反射を求める。
		float NdotL = saturate(dot(normal, -ligDir));
		float3 lambertDiffuse = spotLight[spLigNo].color * NdotL;
		//最終的な拡散反射光を計算する。
		float3 diffuse = albedoColor * diffuseFromFresnel * lambertDiffuse;


		//クックトランスモデルを利用した鏡面反射率を計算する。

		//クックトランスモデルの鏡面反射率を計算する。
		float3 spec = CookTrranceSpecular(-ligDir, toEye, normal, normal2, metaric) * spotLight[spLigNo].color;
		//金属度が高ければ、鏡面反射はスペキュラカラー、低ければ白。
		//スペキュラカラーの強さを鏡面反射率として扱う。
		float specTerm = length(specColor.xyz);
		spec *= lerp(float3(specTerm, specTerm, specTerm), specColor, metaric);

		//鏡面反射率を使って、拡散反射光と鏡面反射光を合成する。
		//鏡面反射率が高ければ、拡散反射は弱くなる。
		float3 diffuseAndSpec = (diffuse * (1.0f - specTerm) + spec) * affect;

		// 入射光と射出方向の角度を求める
		// dot()を利用して内積を求める
		float angle = dot(ligDir, spotLight[spLigNo].direction);

		// dot()で求めた値をacos()に渡して角度を求める
		angle = abs(acos(angle));

		// 角度による影響率を求める
		// 角度に比例して小さくなっていく影響率を計算する
		affect = 1.0f - 1.0f / spotLight[spLigNo].angle * angle;

		// 影響力がマイナスにならないように補正をかける
		if (affect < 0.0f)
		{
			affect = 0.0f;
		}

		// 影響の仕方を指数関数的にする。
		affect = pow(affect, 0.5f);
		diffuseAndSpec *= affect;


		lig += diffuseAndSpec;


	}

	//環境光による底上げ。
	lig += ambientLight * albedoColor;

	//自己発光色
	//lig += albedoColor * emissionColor;
	lig.xyz += emissionColor.xyz;

	//最終的なカラー
	float4 finalColor = 1.0f;
	finalColor.xyz = lig;

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
	finalColor *= mulColor;
	return finalColor;
}

float4 SpecialColor(float4 albedoColor, float3 viewNormal, float3 normal)
{


	float4 lig = albedoColor;
	//環境光による底上げ。
	//lig.xyz += ambientLight * albedoColor;

	//自己発光色
	//lig += albedoColor * emissionColor;
	lig.xyz += emissionColor.xyz;
	lig *= mulColor;

	float4 color = g_skyCubeMap.Sample(g_sampler, normal * -1.0f);

	//リム
	//輪郭を光らせる
	float limPower = pow( 1.0f - abs(viewNormal.z), 5.0f );
	lig.xyz += color * limPower * 0.8f;
	

	return lig;

}