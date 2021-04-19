#include "stdafx.h"
#include "Pause.h"

//�X�^�[�g�֐�
bool CPause::Start()
{
	////�t�H���g�����_���[�𐶐�
	//for (int i = 0; i < enPauseStateNum; i++)
	//{
	//	m_fontRender[i] = NewGO<CFontRender>(0);
	//}

	//�t�H���g�̍��[�̏ꏊ
	//const float leftSidePos = -150.0f;



	//�摜�̕\��
	m_level2D.Init("Assets/level2D/Pause.casl", [&](Level2DObjectData& objdata)
		{
			//���O����v�Ńt�b�N����
			//�J�[�\��
			if (objdata.EqualObjectName("Pause"))
			{
				m_pause = NewGO<CSpriteRender>(1);
				m_pause->Init("Assets/level2D/Pause.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_pause->SetScale(objdata.scale);
				m_pause->SetPosition(objdata.position);
				//�t�b�N������true��߂�
				return true;
			}
			//�Q�[���ɖ߂�
			if (objdata.EqualObjectName("Continue"))
			{
				m_choices[enContinue] = NewGO<CSpriteRender>(1);
				m_choices[enContinue]->Init("Assets/level2D/Continue.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_choices[enContinue]->SetScale(objdata.scale);
				m_choices[enContinue]->SetPosition(objdata.position);
				//�t�b�N������true��߂�
				return true;
			}
			//���g���C
			if (objdata.EqualObjectName("Retry"))
			{
				m_choices[enRetry] = NewGO<CSpriteRender>(1);
				m_choices[enRetry]->Init("Assets/level2D/Retry.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_choices[enRetry]->SetScale(objdata.scale);
				m_choices[enRetry]->SetPosition(objdata.position);
				//�t�b�N������true��߂�
				return true;
			}
			//�X�e�[�W����o��
			if (objdata.EqualObjectName("Quit"))
			{
				m_choices[enQuit] = NewGO<CSpriteRender>(1);
				m_choices[enQuit]->Init("Assets/level2D/Quit.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_choices[enQuit]->SetScale(objdata.scale);
				m_choices[enQuit]->SetPosition(objdata.position);
				//�t�b�N������true��߂�
				return true;
			}
			//�J�[�\��
			if (objdata.EqualObjectName("cursor"))
			{
				m_cursor = NewGO<CSpriteRender>(1);
				m_cursor->Init("Assets/level2D/cursor.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_cursor->SetScale(objdata.scale);
				m_cursor->SetPosition(objdata.position);
				//�t�b�N������true��߂�
				return true;
			}
			


			//���̂܂ܕ\�����邩��false��߂�
			return false;
		});


	for (int i = 0; i < enPauseNum; i++) {
		m_choices[i]->Deactivate();
	}
	m_pause->Deactivate();
	m_cursor->Deactivate();


	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->UpdateSprite();
	}
	m_pause->UpdateSprite();
	m_cursor->UpdateSprite();


	////�t�H���g�����_���[�̏������B�e�L�X�g���Z�b�g����B
	////�uPause�v���e�L�X�g�ɃZ�b�g
	//m_fontRender[enPause]
	//	->Init(L"Pause",
	//	{ leftSidePos, 300.0f },
	//	{ 1.0f,1.0f,1.0f,1.0f },
	//	0.0f,
	//	2.0f
	//);
	////�uPause�v���e�L�X�g�ɃZ�b�g
	//m_fontRender[enContinue]
	//	->Init(L"Continue",
	//	{ leftSidePos, 150.0f }
	//);	
	//m_fontRender[enRetry]
	//	->Init(L"Retry",
	//	{ leftSidePos, 0.0f }
	//);
	//m_fontRender[enQuit]
	//	->Init(L"Quit",
	//	{ leftSidePos, -150.0f }
	//);

	
	//for (int i = 0; i < enPauseStateNum; i++)
	//{
	//	//PostRenderFlag��true�ɂ��Ĉ�ԏ�ɕ\������悤�ɂ���
	//	m_fontRender[i]->SetPostRenderFlag(true);
	//	//�t�H���g�𖳌������ĕ\�����Ȃ��悤�ɂ���B
	//	m_fontRender[i]->Deactivate();
	//}

	
	////�t�H���g�����_���[�̐���
	//m_arrowFR = NewGO<CFontRender>(0);
	////�t�H���g�����_���[�̏�����
	////�J�[�\���̖��̃e�L�X�g��ݒ�
	//m_arrowFR
	//	->Init(L"->",
	//		{ -200.0f,145.0f },
	//		{ 0.0f,0.0f,0.0f,1.0f }
	//);
	////��ԏ�ɕ\������悤�ɂ���
	//m_arrowFR->SetPostRenderFlag(true);
	////���������Ĕ�\���ɂ���B
	//m_arrowFR->Deactivate();

	//buttonASE�̃T�E���h�L���[�𐶐�����
	m_buttonASE = NewGO<CSoundCue>(0);
	//buttonASE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_buttonASE->Init(L"Assets/sound/buttonA.wav");
	//���ʒ���
	m_buttonASE->SetVolume(0.5f);

	//buttonB�̃T�E���h�L���[�𐶐�����
	m_buttonBSE = NewGO<CSoundCue>(0);
	//buttonB�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_buttonBSE->Init(L"Assets/sound/buttonB.wav");
	//���ʒ���
	m_buttonBSE->SetVolume(0.5f);

	//selectSE�̃T�E���h�L���[�𐶐�����
	m_selectSE = NewGO<CSoundCue>(0);
	//selectSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_selectSE->Init(L"Assets/sound/select.wav");
	//���ʒ���
	m_selectSE->SetVolume(0.5f);

	return true;
}

//�f�X�g���N�^
CPause::~CPause()
{
	//�|�[�Y��Ԃ���������
	UnPause();

	//�t�H���g�����_���[�̏���
	for (int i = 0; i < enPauseNum; i++)
	{
		DeleteGO(m_choices[i]);
	}
	DeleteGO(m_pause);
	DeleteGO(m_cursor);
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

	////�J�[�\���̍��W
	//Vector2 arrowPos;
	////���[�̐��l������
	//arrowPos.x = -200.0f;
	////��[����A���̂�������*���݂̃X�e�[�g�����ɉ�����
	//arrowPos.y = 300.0f - m_pauseState * 150.0f - 5.0f;
	////�J�[�\���̈ʒu��ݒ肷��
	//m_cursor->SetPosition({ arrowPos.x, arrowPos.y, 0.0f });
		//�J�[�\���p�̉摜�̏ꏊ��ݒ肷��
	m_cursor->SetPosition({ -360.0f,
							m_choices[m_pauseState]->GetPositionY() + 25,
							0.0f });


	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->UpdateSprite();
	}
	m_pause->UpdateSprite();
	m_cursor->UpdateSprite();
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
	
	//buttonASE�����[�v�Đ����I�t�ōĐ�����B
	m_buttonASE->Play(false);

	//IGameObject�Ƀ|�[�Y�����Ɠ`����B
	//Update()���Ă΂�Ȃ��Ȃ��āA
	//UpdateOnlyPaused()���Ă΂��悤�ɂȂ�
	SetGamePaused(m_isPaused);

	//�|�[�Y���ɕ\�������t�H���g��L��������
	//�\���ł���悤�ɂ���B
	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->Activate();
	}
	m_pause->Activate();
	m_cursor->Activate();

}

/// <summary>
/// �|�[�Y��Ԃ����������֐�
/// </summary>
void CPause::UnPause()
{
	//�|�[�Y���łȂ���Ԃɂ���B
	m_isPaused = false;

	if (g_pad[0]->IsTrigger(enButtonStart)) {
		
		//buttonB�����[�v�Đ����I�t�ōĐ�����B
		m_buttonBSE->Play(false);

	}

	//IGameObject�Ƀ|�[�Y������������悤�ɓ`����
	//Update()���Ă΂��悤�ɂȂ��āA
	//UpdateOnlyPaused()���Ă΂�Ȃ��Ȃ�B
	SetGamePaused(m_isPaused);

	//�|�[�Y���ɕ\�������t�H���g�𖳌�������
	//�\���ł��Ȃ��Ȃ�悤�ɂ���B
	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->Deactivate();
	}
	m_pause->Deactivate();
	m_cursor->Deactivate();

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
		
		//selectSE�����[�v�Đ����I�t�ōĐ�����B
		m_selectSE->Play(false);

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

		//selectSE�����[�v�Đ����I�t�ōĐ�����B
		m_selectSE->Play(false);

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
			//buttonB�����[�v�Đ����I�t�ōĐ�����B
			m_buttonBSE->Play(false);
			break;

		case enRetry:
			//�u���g���C�v
			//StageBase�Ƀ��g���C��Ԃ��Ɠ`����
			m_retryFlag = true;
			//buttonASE�����[�v�Đ����I�t�ōĐ�����B
			m_buttonASE->Play(false);
			break;
		case enQuit:
			//�u�I���v
			//StageBase�ɏI����Ԃ��Ɠ`����
			m_quitFlag = true;
			//buttonASE�����[�v�Đ����I�t�ōĐ�����B
			m_buttonASE->Play(false);
			break;
		default:
			break;
		}

	}
}