#include "stdafx.h"
#include "FontRender.h"

//�X�^�[�g�֐�
bool CFontRender::Start()
{
	return true;
}

//�f�X�g���N�^
CFontRender::~CFontRender()
{

}


/// <summary>
/// �������֐��B�ŏ��ɌĂ�łˁB
/// </summary>
/// <param name="text">�\������e�L�X�g</param>
/// <param name="position">���W�B{0.0f,0.0f}������</param>
/// <param name="color">�J���[{1.0f,1.0f,1.0f,1.0f}�����F</param>
/// <param name="rotation">��]</param>
/// <param name="scale">�g��</param>
/// <param name="pivot">�s�{�b�g�i��_�j</param>
void CFontRender::Init
(const wchar_t* text,	//�\������e�L�X�g
	Vector2 position,	//���W
	Vector4 color,		//�J���[
	float rotation,		//��]
	float scale,		//�g��
	Vector2 pivot)		//�s�{�b�g�i��_�j
{
	//���ꂼ��������o�ϐ��ɕێ�������
	wsprintf(m_text, text);
	m_position = position;
	m_color = color;
	m_rotation = rotation;
	m_scale = scale;
	m_pivot = pivot;
}

/// <summary>
/// �\������e�L�X�g��ݒ肷��
/// </summary>
/// <param name="text">�e�L�X�g</param>
void CFontRender::SetText(const wchar_t* text)
{
	swprintf_s(m_text, text);
}


/// <summary>
/// �`��p�֐�
/// </summary>
/// <param name="rc"></param>
void CFontRender::Render(RenderContext& rc)
{
	//PostRenderFlag��true��������A�Ă΂Ȃ�
	if (m_postRenderFlag)
		return;

	//�`��J�n
	m_font.Begin(rc);

	//�`�揈��
	m_font.Draw(m_text,
		m_position,
		m_color,
		m_rotation,
		m_scale,
		m_pivot);

	//�`��I��
	m_font.End(rc);
}

/// <summary>
/// Render�̌�ŌĂ΂��֐��B
/// ��ԏ�ɕ`�悳���B
/// </summary>
/// <param name="rc"></param>
void CFontRender::PostRender(RenderContext& rc)
{
	//PostRenderFlag��true��������A�Ă΂��
	if (!m_postRenderFlag)
		return;

	//�`��J�n
	m_font.Begin(rc);

	//�`�揈��
	m_font.Draw(m_text,
		m_position,
		m_color,
		m_rotation,
		m_scale,
		m_pivot);

	//�`��I��
	m_font.End(rc);

}