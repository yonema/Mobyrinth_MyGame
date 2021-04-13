#include "stdafx.h"
#include "Sky.h"


CSky::CSky()
{

}

CSky::~CSky()
{

}

bool CSky::Start()
{
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = "Assets/modelData/preset/sky.tkm";
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/SkyCubeMap.fx";

	m_texture.InitFromDDSFile(L"Assets/modelData/preset/skyCubeMap.dds");
	//m_texture.InitFromDDSFile(L"Assets/modelData/preset/sky.dds");

	initData.m_expandShaderResoruceView = &m_texture;

	m_model.Init(initData);
	m_model.UpdateWorldMatrix(m_position, g_quatIdentity, m_scale);
	//m_modelRender = NewGO<CModelRender>(0);
	//m_modelRender->Init(initData);
	return true;
}

void CSky::Update()
{
	if (m_isDirty)
	{
		m_model.UpdateWorldMatrix(m_position, g_quatIdentity, m_scale);
		/*m_modelRender->SetPosition(m_position);
		m_modelRender->SetScale(m_scale);
		m_modelRender->SetSelfLuminous(m_selfLuminous);*/
		m_isDirty = false;
	}
}

void CSky::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}