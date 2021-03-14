#include "stdafx.h"
#include "FontRender.h"

bool CFontRender::Start()
{
	return true;
}

CFontRender::~CFontRender()
{

}

void CFontRender::Init(const wchar_t* text,
	Vector2 position,
	Vector4 color,
	float rotation,
	float scale,
	Vector2 pivot)
{
	wsprintf(m_text, text);
	m_position = position;
	m_color = color;
	m_rotation = rotation;
	m_scale = scale;
	m_pivot = pivot;
}

void CFontRender::SetText(const wchar_t* text)
{
	wsprintf(m_text, text);
}



void CFontRender::Render(RenderContext& rc)
{
	if (m_postRenderFlag)
		return;
	m_font.Begin(rc);

	m_font.Draw(m_text,
		m_position,
		m_color,
		m_rotation,
		m_scale,
		m_pivot);

	m_font.End(rc);
}

void CFontRender::PostRender(RenderContext& rc)
{
	if (!m_postRenderFlag)
		return;

	m_font.Begin(rc);

	m_font.Draw(m_text,
		m_position,
		m_color,
		m_rotation,
		m_scale,
		m_pivot);

	m_font.End(rc);

}