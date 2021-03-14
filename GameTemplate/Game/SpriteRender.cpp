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
	m_initData.m_ddsFilePath[0] = texFilePath;
	m_initData.m_width = w;
	m_initData.m_height = h;
	m_initData.m_fxFilePath = "Assets/shader/sprite.fx";
	m_sprite.Init(m_initData);

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
