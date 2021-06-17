#include "stdafx.h"
#include "Mobius.h"
#include "LevelObjectManager.h"


//スタート関数
bool CMobius::Start()
{
	//モデルレンダラーの生成
	m_modelRender = NewGO<CModelRender>(0);

	//初期化データを作成する
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = "Assets/modelData/Mobius.tkm";
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/PBR.fx";

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
		sizeof(*g_graphicsEngine->GetShadowMap().GetShadowParam()) * g_max_shadowMap;
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
}


const bool CMobius::GetIntersectPosWithMobius(
	const Vector3& startPos, const Vector3& endPos, Vector3* const IntersectPos
)
{
	if (m_modelRender->GetModel().InIntersectLine(startPos, endPos))
	{
		if (IntersectPos)
			*IntersectPos = m_modelRender->GetModel().GetIntersectPos();
		return true;
	}

	return false;
}