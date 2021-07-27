#include "stdafx.h"
#include "SpriteRender.h"

//�X�v���C�g�����_���[�̒萔�f�[�^���g�p�\�ɂ���
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

/**
 * @brief �������p�֐��B���̈�����<float>�o�[�W�����B�ŏ��ɌĂ�łˁB
 * @attention ����UINT�ɃL���X�g�����̂ŁA�����_�ȉ��̃f�[�^�͎�����B
 * @param [in] texFilePath dds�t�@�C���p�X
 * @param [in] w �X�v���C�g�̉���
 * @param [in] h �X�v���C�g�̏c��
 * @param [in] pivot �s�{�b�g�i��_�j
 * @param [in] alphaBlendMode �A���t�@�u�����f�B���O���[�h
*/
void CSpriteRender::Init(
	const char* texFilePath,
	const float w,
	const float h,
	const Vector2& pivot,
	AlphaBlendMode alphaBlendMode
)
{
	//�������p�֐��̃��C���R�A
	InitMainCore(texFilePath, static_cast<UINT>(w), static_cast<UINT>(h), pivot, alphaBlendMode);

	return;
}

/**
 * @brief �������p�֐��B���̈�����<int>�o�[�W�����B�ŏ��ɌĂ�łˁB
 * @param [in] texFilePath dds�t�@�C���p�X
 * @param [in] w �X�v���C�g�̉���
 * @param [in] h �X�v���C�g�̏c��
 * @param [in] pivot �s�{�b�g�i��_�j
 * @param [in] alphaBlendMode �A���t�@�u�����f�B���O���[�h
*/
void CSpriteRender::Init(
	const char* texFilePath,
	const int w,
	const int h,
	const Vector2& pivot,
	AlphaBlendMode alphaBlendMode
)
{
	//�������p�֐��̃��C���R�A
	//int��float�ɃL���X�g���Ă���n��
	InitMainCore(texFilePath, static_cast<UINT>(w), static_cast<UINT>(h), pivot, alphaBlendMode);

	return;
}

/**
 * @brief �������p�֐��̃��C���R�A�BInit�֐��̒��ŌĂ΂��B
 * @param [in] texFilePath dds�t�@�C���p�X
 * @param [in] w �X�v���C�g�̉���
 * @param [in] h �X�v���C�g�̏c��
 * @param [in] pivot �s�{�b�g�i��_�j
 * @param [in] alphaBlendMode �A���t�@�u�����f�B���O���[�h
*/
void CSpriteRender::InitMainCore(
	const char* texFilePath,
	const UINT w,
	const UINT h,
	const Vector2& pivot,
	AlphaBlendMode alphaBlendMode
)
{
	//�X�v���C�g�̏������p�f�[�^
	SpriteInitData initData;
	initData.m_ddsFilePath[0] = texFilePath;
	initData.m_width = w;
	initData.m_height = h;
	initData.m_fxFilePath = SHADER_FILEPATH_DEFAULT;
	initData.m_psEntryPoinFunc = ENTRY_POINT_FUNC_PS_DEFAULT;
	initData.m_vsEntryPointFunc = ENTRY_POINT_FUNC_VS_DEFAULT;
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

	return;
}

//��ԏ�ɕ`�悷��֐�
void CSpriteRender::PostRender(RenderContext& rc)
{
	//PostRenderFlag��false�Ȃ牽��������return
	if (!m_postRenderFlag)
		return;

	//�X�v���C�g�̕`��p�֐�
	m_sprite.Draw(rc);

	return;
}