#include "stdafx.h"
#include "SpriteRender.h"

bool CSpriteRender::Start()
{
	return true;
}

CSpriteRender::~CSpriteRender()
{

}
void CSpriteRender::Init(const char* texFilePath, float w, float h)
{
	SpriteInitData initData;
	initData.m_ddsFilePath[0] = texFilePath;
	initData.m_width = w;
	initData.m_height = h;
	initData.m_fxFilePath = "Assets/shader/sprite.fx";
	initData.m_psEntryPoinFunc = "PSMain";
	initData.m_vsEntryPointFunc = "VSMain";
	m_sprite.Init(initData);

	return;
}
void CSpriteRender::Update()
{
	m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);

	return;
}

void CSpriteRender::Render(RenderContext& rc)
{
	m_sprite.Draw(rc);
}
