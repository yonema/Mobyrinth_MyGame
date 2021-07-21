#include "stdafx.h"
#include "Mobius.h"
#include "LevelObjectManager.h"

using namespace mobiusConstData;


//スタート関数
bool CMobius::Start()
{
	//モデルレンダラーの生成
	m_modelRender = NewGO<CModelRender>(PRIORITY_FIRST);

	//初期化データを作成する
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = MODEL_FILEPATH_MOBIUS;
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = SHADER_FILEPATH_MOBIUS;

	//シェーダに渡すコンスタントバッファの設定

	//ライト共通のデータの登録
	initData.m_expandConstantBuffer[0] =
		CLightManager::GetInstance()->GetLightParam();
	initData.m_expandConstantBufferSize[0] =
		sizeof(*CLightManager::GetInstance()->GetLightParam());

	//ディレクションライト達のデータの登録
	initData.m_expandConstantBuffer[1] =
		CLightManager::GetInstance()->GetDirectionLigData();
	initData.m_expandConstantBufferSize[1] =
		sizeof(*CLightManager::GetInstance()->GetDirectionLigData()) *
		CLightManager::GetMax_DirectionLight();

	//ポイントライト達のデータの登録
	initData.m_expandConstantBuffer[2] =
		CLightManager::GetInstance()->GetPointLigData();
	initData.m_expandConstantBufferSize[2] =
		sizeof(*CLightManager::GetInstance()->GetPointLigData()) *
		CLightManager::GetMax_PointLight();

	//スポットライト達のデータの登録
	initData.m_expandConstantBuffer[3] =
		CLightManager::GetInstance()->GetSpotLigData();
	initData.m_expandConstantBufferSize[3] =
		sizeof(*CLightManager::GetInstance()->GetPointLigData()) *
		CLightManager::GetMax_SpotLight();

	//シャドウのデータの登録
	initData.m_expandConstantBuffer[4] =
		g_graphicsEngine->GetShadowMap().GetShadowParam();
	initData.m_expandConstantBufferSize[4] =
		sizeof(*g_graphicsEngine->GetShadowMap().GetShadowParam()) * g_MAX_SHADOW_NUM;
	initData.m_expandShaderResoruceView[0] = &g_graphicsEngine->GetShadowMap().GetShadowBlur();

	//ZPrepassで作成された深度テクスチャの登録
	initData.m_expandShaderResoruceView[1] = &g_graphicsEngine->GetZPrepassDepthTexture();

	initData.m_expandShaderResoruceView[2] = &g_graphicsEngine->GetSkyCubeTexture();


	//モデルレンダラーの初期化
	m_modelRender->Init(initData);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	//m_modelRender->SetShadowCasterFlag(true);
	m_modelRender->SetShadowReceiverFlag(true);

	return true;
}

//デストラクタ
CMobius::~CMobius()
{
	//モデルレンダラーの破棄
	DeleteGO(m_modelRender);
	//マネージャーにメビウスの輪が消去されたと伝える
	CLevelObjectManager::GetInstance()->SetMobius(nullptr);

	return;
}

/**
 * @brief メビウスの輪とレイの交差点を求める
 * @param [in] startPos レイの始点
 * @param [in] endPos レイの終点
 * @param [out] IntersectPos 交差点
 * @return 交差しているか？
*/
bool CMobius::GetIntersectPosWithMobius(
	const Vector3& startPos, const Vector3& endPos, Vector3* const IntersectPos
)
{
	//メビウスの輪のモデルとレイが交差しているか？
	if (m_modelRender->GetModel().InIntersectLine(startPos, endPos))
	{
		//交差した

		//交差点の座標が引数で渡されているか？
		if (IntersectPos)
			//渡されている
			//交差点を入れる
			*IntersectPos = m_modelRender->GetModel().GetIntersectPos();

		//交差した、と戻す
		return true;
	}

	//交差してない
	return false;
}