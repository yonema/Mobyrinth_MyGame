#include "stdafx.h"
#include "background_kari.h"
#include "Light.h"

bool background_kari::Start()
{
	ModelInitData initData;
	initData.m_tkmFilePath = "Assets/modelData/bg/bg.tkm";
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//頂点シェーダーのエントリーポイント
	initData.m_vsEntryPointFunc = "VSMain";






	//initData.m_expandConstantBuffer = &Light::GetInstance()->m_directionLight;
	//initData.m_expandConstantBufferSize = sizeof(Light::GetInstance()->m_directionLight);

	//initData.m_expandConstantBuffer2 = &Light::GetInstance()->m_pointLight;
	//initData.m_expandConstantBufferSize2 = sizeof(Light::GetInstance()->m_pointLight);
	m_model.Init(initData);

	//メッシュコライダーを作成。
	m_physicsStaticObject.CreateFromModel(m_model, m_model.GetWorldMatrix());
	return true;
}
void background_kari::Update()
{

}
void background_kari::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}