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

	initData.m_expandShaderResoruceView = &m_texture;

	m_model.Init(initData);
	m_model.UpdateWorldMatrix(m_position, g_quatIdentity, m_scale);


	return true;
}

void CSky::Update()
{
	if (m_isDirty)
	{
		m_model.UpdateWorldMatrix(m_position, g_quatIdentity, m_scale);

		m_isDirty = false;
	}
}

void CSky::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}