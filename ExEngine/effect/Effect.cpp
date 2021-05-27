/*!
 * @brief	�G�t�F�N�g
 */

#include "stdafx.h"
#include "Effect.h"


 //�R���X�g���N�^
Effect::Effect()
{
}
//�f�X�g���N�^
Effect::~Effect()
{
	
}

//�X�^�[�g�֐�
bool Effect::Start()
{
	return true;
}

//�A�b�v�f�[�g�֐�
void Effect::UpdateWhenPaused()
{
	EffectEngine::GetInstance()->UpdateEffectWorldMatrix(
		m_handle,
		m_position,
		m_rotation,
		m_scale
	);
}
		
/// <summary>
/// �������B
/// </summary>
/// <remark>
/// �G�t�F�N�g�Đ��̂��߂̏������s���܂��B
/// </remark>
/// <param name="filePath">�t�@�C���p�X</param>
void Effect::Init(const char16_t* filePath)
{
	m_effect = EffectEngine::GetInstance()->LoadEffect(filePath);
	
}

/// <summary>
/// �Đ��J�n
/// </summary>
/// <remark>
/// ���̊֐����Ăяo���ƃG�t�F�N�g�̍Đ����J�n����܂��B
/// </remark>
void Effect::Play()
{
	//�Đ����̃G�t�F�N�g���~����B
	EffectEngine::GetInstance()->Stop(m_handle);
	//�V�����Đ��B
	m_handle = EffectEngine::GetInstance()->Play(m_effect);
}

