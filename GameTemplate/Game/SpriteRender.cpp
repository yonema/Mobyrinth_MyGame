#include "stdafx.h"
#include "SpriteRender.h"


using namespace spriteRenderConstData;

//�X�^�[�g�֐�
bool CSpriteRender::Start()
{
	return true;
}

//�f�X�g���N�^
CSpriteRender::~CSpriteRender()
{

}

/// <summary>
/// �������p�֐�
/// �ŏ��ɌĂ�łˁB
/// </summary>
/// <param name="texFilePath">dds�t�@�C���p�X</param>
/// <param name="w">�X�v���C�g�̉���</param>
/// <param name="h">�X�v���C�g�̏c��</param>
/// <param name="pivot">�s�{�b�g�i��_�j</param>
void CSpriteRender::Init(const char* texFilePath, float w, float h, const Vector2& pivot,
	AlphaBlendMode alphaBlendMode)
{
	//�X�v���C�g�̏������p�f�[�^
	SpriteInitData initData;
	initData.m_ddsFilePath[0] = texFilePath;
	initData.m_width = w;
	initData.m_height = h;
	initData.m_fxFilePath = "Assets/shader/sprite.fx";
	initData.m_psEntryPoinFunc = "PSMain";
	initData.m_vsEntryPointFunc = "VSMain";
	initData.m_alphaBlendMode = alphaBlendMode;

	m_pivot = pivot;

	//�X�v���C�g�̏�����
	m_sprite.Init(initData);

	return;
}

//�A�b�v�f�[�g�֐�
void CSpriteRender::Update()
{
	//�X�v���C�g�̃A�b�v�f�[�g
	m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);

	return;
}

//�`��p�֐�
void CSpriteRender::Render(RenderContext& rc)
{
	//PostRenderFlag��true�Ȃ牽��������return
	if (m_postRenderFlag)
		return;

	//�X�v���C�g�̕`��p�֐�
	m_sprite.Draw(rc);
}

//��ԏ�ɕ`�悷��֐�
void CSpriteRender::PostRender(RenderContext& rc)
{
	//PostRenderFlag��false�Ȃ牽��������return
	if (!m_postRenderFlag)
		return;

	//�X�v���C�g�̕`��p�֐�
	m_sprite.Draw(rc);
}