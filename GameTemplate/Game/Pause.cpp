#include "stdafx.h"
#include "Pause.h"

//�X�^�[�g�֐�
bool CPause::Start()
{
	//�t�H���g�����_���[�𐶐�
	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i] = NewGO<CFontRender>(0);
	}

	//�t�H���g�̍��[�̏ꏊ
	const float leftSidePos = -150.0f;

	//�t�H���g�����_���[�̏������B�e�L�X�g���Z�b�g����B
	//�uPause�v���e�L�X�g�ɃZ�b�g
	m_fontRender[enPause]
		->Init(L"Pause",
		{ leftSidePos, 300.0f },
		{ 1.0f,1.0f,1.0f,1.0f },
		0.0f,
		2.0f
	);
	//�uPause�v���e�L�X�g�ɃZ�b�g
	m_fontRender[enContinue]
		->Init(L"Continue",
		{ leftSidePos, 150.0f }
	);	
	m_fontRender[enRetry]
		->Init(L"Retry",
		{ leftSidePos, 0.0f }
	);
	m_fontRender[enQuit]
		->Init(L"Quit",
		{ leftSidePos, -150.0f }
	);

	
	for (int i = 0; i < enPauseStateNum; i++)
	{
		//PostRenderFlag��true�ɂ��Ĉ�ԏ�ɕ\������悤�ɂ���
		m_fontRender[i]->SetPostRenderFlag(true);
		//�t�H���g�𖳌������ĕ\�����Ȃ��悤�ɂ���B
		m_fontRender[i]->Deactivate();
	}

	
	//�t�H���g�����_���[�̐���
	m_arrowFR = NewGO<CFontRender>(0);
	//�t�H���g�����_���[�̏�����
	//�J�[�\���̖��̃e�L�X�g��ݒ�
	m_arrowFR
		->Init(L"->",
			{ -200.0f,145.0f },
			{ 0.0f,0.0f,0.0f,1.0f }
	);
	//��ԏ�ɕ\������悤�ɂ���
	m_arrowFR->SetPostRenderFlag(true);
	//���������Ĕ�\���ɂ���B
	m_arrowFR->Deactivate();

	return true;
}

//�f�X�g���N�^
CPause::~CPause()
{
	//�|�[�Y��Ԃ���������
	UnPause();

	//�t�H���g�����_���[�̏���
	for (int i = 0; i < enPauseStateNum; i++)
	{
		DeleteGO(m_fontRender[i]);
	}
	DeleteGO(m_arrowFR);
}


/// <summary>
/// �|�[�Y���ł��Ă΂��A�b�v�f�[�g�֐�
/// �|�[�Y���łȂ��Ă���ɌĂ΂��
/// </summary>
void CPause::UpdateWhenPaused()
{
	//�X�^�[�g�{�^������������
	//�i�L�[�{�[�h�Ȃ�Enter����������j
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		if (m_isPaused)
		{
			//�|�[�Y���Ȃ�
			//�|�[�Y��Ԃ�����
			UnPause();
		}
		else
		{
			//�|�[�Y���łȂ��Ȃ�
			//�|�[�Y��Ԃɂ���
			ToPause();
		}
	}
}


/// <summary>
/// �|�[�Y���ɂ����Ă΂��A�b�v�f�[�g�֐�
/// </summary>
void CPause::UpdateOnlyPaused()
{
	//�J�[�\����I��������
	Choose();
	//�I�������J�[�\�������肷��
	Decision();

	//�J�[�\���̍��W
	Vector2 arrowPos;
	//���[�̐��l������
	arrowPos.x = -200.0f;
	//��[����A���̂�������*���݂̃X�e�[�g�����ɉ�����
	arrowPos.y = 300.0f - m_pauseState * 150.0f - 5.0f;
	//�J�[�\���̈ʒu��ݒ肷��
	m_arrowFR->SetPosition(arrowPos);
}

/// <summary>
/// �|�[�Y��Ԃɂ���֐�
/// </summary>
void CPause::ToPause()
{
	//�|�[�Y�ł��Ȃ���ԂȂ�A�������Ȃ���return
	if (m_canPause == false) 
	{
		return;
	}

	//�|�[�Y���ɂ���
	m_isPaused = true;

	//IGameObject�Ƀ|�[�Y�����Ɠ`����B
	//Update()���Ă΂�Ȃ��Ȃ��āA
	//UpdateOnlyPaused()���Ă΂��悤�ɂȂ�
	SetGamePaused(m_isPaused);

	//�|�[�Y���ɕ\�������t�H���g��L��������
	//�\���ł���悤�ɂ���B
	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i]->Activate();
	}
	m_arrowFR->Activate();

}

/// <summary>
/// �|�[�Y��Ԃ����������֐�
/// </summary>
void CPause::UnPause()
{
	//�|�[�Y���łȂ���Ԃɂ���B
	m_isPaused = false;

	//IGameObject�Ƀ|�[�Y������������悤�ɓ`����
	//Update()���Ă΂��悤�ɂȂ��āA
	//UpdateOnlyPaused()���Ă΂�Ȃ��Ȃ�B
	SetGamePaused(m_isPaused);

	//�|�[�Y���ɕ\�������t�H���g�𖳌�������
	//�\���ł��Ȃ��Ȃ�悤�ɂ���B
	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i]->Deactivate();
	}
	m_arrowFR->Deactivate();

}

/// <summary>
///	�|�[�Y���ɃJ�[�\���I��������֐�
/// </summary>
void CPause::Choose()
{
	//�{�^���̓��͂𒲂ׂ�
	if (g_pad[0]->GetLStickYF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//�������͂��Ȃ����
		//�{�^�����������Ƃ��ł���悤�ɂ���B
		m_buttonFlag = true;
	}
	if (g_pad[0]->GetLStickYF() < 0.0f && m_buttonFlag)
	{
		//������
		//�{�^�����������Ƃ��ł��Ȃ��悤�ɂ���B�i�A�����͖h�~�p�j
		m_buttonFlag = false;
		//���݂̃|�[�Y���̃X�e�[�g�����Z����
		m_pauseState++;
		//��ԉ���enQuit�܂ōs������A
		//enQuit��艺�ɍs���Ȃ��悤�ɂ���
		if (m_pauseState > enQuit)
		{
			m_pauseState = enQuit;
		}
	}
	else if (g_pad[0]->GetLStickYF() > 0.0f && m_buttonFlag)
	{
		//�����
		//�{�^�����������Ƃ��ł��Ȃ��悤�ɂ���B�i�A�����͖h�~�p�j
		m_buttonFlag = false;
		//���݂̃|�[�Y���̃X�e�[�g�����Z����
		m_pauseState--;
		//��ԏ��enContinue�܂ōs������A
		//enContinue����ɍs���Ȃ��悤�ɂ���
		if (m_pauseState < enContinue)
		{
			m_pauseState = enContinue;
		}
	}

}

/// <summary>
/// �I�������J�[�\���Ō��肷��֐�
/// </summary>
void CPause::Decision()
{
	//�{�^���̓��͂𒲂ׂ�
	if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//A�{�^������
		//�{�^�����������Ƃ��ł��Ȃ��悤�ɂ���B�i�A�����͖h�~�p�j
		m_buttonFlag = false;

		//���݂̃X�e�[�g�ɂ����
		//����̔�����U�蕪����
		switch (m_pauseState)
		{
		case enContinue:
			//�u������v
			//�|�[�Y������������
			UnPause();
			break;
		case enRetry:
			//�u���g���C�v
			//StageBase�Ƀ��g���C��Ԃ��Ɠ`����
			m_retryFlag = true;
			break;
		case enQuit:
			//�u�I���v
			//StageBase�ɏI����Ԃ��Ɠ`����
			m_quitFlag = true;
			break;
		default:
			break;
		}

	}
}