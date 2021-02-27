#include "stdafx.h"
#include "ModelRender.h"
#include "LightManager.h"

void CModelRender::Init(const char* filePath)
{
	InitSkeleton(filePath);
	InitModel(filePath);
}

void CModelRender::InitSkeleton(const char* filePath)
{
	m_skeleton.Init("Assets/modelData/unityChan.tks");
}
void CModelRender::InitModel(const char* filePath)
{
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/myModel.fx";
	//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
	initData.m_vsEntryPointFunc = "VSMain";
	//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
	initData.m_vsSkinEntryPointFunc = "VSSkinMain";
	//スケルトンを指定する。
	initData.m_skeleton = &m_skeleton;
	//モデルの上方向を指定する。
	//3dsMaxではデフォルトZアップになっているが、
	//ユニティちゃんはアニメーションでYアップに変更されている。
	initData.m_modelUpAxis = enModelUpAxisY;


	initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLightParam();
	initData.m_expandConstantBufferSize = sizeof(*CLightManager::GetInstance()->GetLightParam());

	initData.m_expandConstantBuffer2 = CLightManager::GetInstance()->GetDirectionLigData();
	initData.m_expandConstantBufferSize2 = sizeof(*CLightManager::GetInstance()->GetDirectionLigData()) * 4;

	//作成した初期化データをもとにモデルを初期化する、
	m_model.Init(initData);

}