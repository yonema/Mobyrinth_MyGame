#include "stdafx.h"
#include "Pause.h"

//�X�^�[�g�֐�
bool CPause::Start()
{
	//�摜�̕\��
	m_level2D.Init("Assets/level2D/Pause_2.casl", [&](Level2DObjectData& objdata)
		{
			//���O����v�Ńt�b�N����
			//�J�[�\��
			if (objdata.EqualObjectName("Pause"))
			{
				m_pause = NewGO<CSpriteRender>(3);
				m_pause->Init("Assets/level2D/Pause.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_pause->SetScale(objdata.scale);
				m_pause->SetPosition(objdata.position);
				m_pause->SetPostRenderFlag(true);
				//�t�b�N������true��߂�
				return true;
			}
			//�Q�[���ɖ߂�
			else if (objdata.EqualObjectName("Continue"))
			{
				m_choices[enContinue] = NewGO<CSpriteRender>(3);
				m_choices[enContinue]->Init("Assets/level2D/Continue.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_choices[enContinue]->SetScale(objdata.scale);
				m_choices[enContinue]->SetPosition(objdata.position);
				m_choices[enContinue]->SetPostRenderFlag(true);
				//�t�b�N������true��߂�
				return true;
			}
			//���g���C
			else if (objdata.EqualObjectName("Retry"))
			{
				m_choices[enRetry] = NewGO<CSpriteRender>(3);
				m_choices[enRetry]->Init("Assets/level2D/Retry.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_choices[enRetry]->SetScale(objdata.scale);
				m_choices[enRetry]->SetPosition(objdata.position);
				m_choices[enRetry]->SetPostRenderFlag(true);
				//�t�b�N������true��߂�
				return true;
			}
			//�X�e�[�W����o��
			else if (objdata.EqualObjectName("Quit"))
			{
				m_choices[enQuit] = NewGO<CSpriteRender>(3);
				m_choices[enQuit]->Init("Assets/level2D/Quit.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_choices[enQuit]->SetScale(objdata.scale);
				m_choices[enQuit]->SetPosition(objdata.position);
				m_choices[enQuit]->SetPostRenderFlag(true);
				//�t�b�N������true��߂�
				return true;
			}
			//�J�[�\��
			else if (objdata.EqualObjectName("cursor"))
			{
				m_cursor = NewGO<CSpriteRender>(3);
				m_cursor->Init("Assets/level2D/cursor.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_cursor->SetScale(objdata.scale);
				m_cursor->SetPosition(objdata.position);
				m_cursor->SetPostRenderFlag(true);
				//�t�b�N������true��߂�
				return true;
			}
			//�|�[�Y���̔w�i
			else if (objdata.EqualObjectName("Pause_background"))
			{
				m_background = NewGO<CSpriteRender>(2);
				m_background->Init(
					objdata.ddsFilePath,
					static_cast<float>(g_FRAME_BUFFER_W),
					static_cast<float>(g_FRAME_BUFFER_H),
					{ 0.5f,0.5f },
					AlphaBlendMode_Trans
				);
				m_background->SetScale(objdata.scale);
				m_background->SetPosition(objdata.position);
				m_background->SetPostRenderFlag(true);
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
	m_background->Deactivate();

	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->UpdateSprite();
	}
	m_pause->UpdateSprite();
	m_cursor->UpdateSprite();
	m_background->UpdateSprite();


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
	DeleteGO(m_background);
}


/// <summary>
/// �|�[�Y���ł��Ă΂��A�b�v�f�[�g�֐�
/// �|�[�Y���łȂ��Ă���ɌĂ΂��
/// </summary>
void CPause::AlwaysUpdate()
{
	//�|�[�Y��ʂ̑��삪�ł��Ȃ��ꍇ�A���̊֐��̏������s��Ȃ��B
	if (m_flagOperation == false) {
		return;
	}

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
	//�|�[�Y��ʂ̑��삪�ł��Ȃ��ꍇ�A���̊֐��̏������s��Ȃ��B
	if (m_flagOperation == false) {
		return;
	}

	//�J�[�\����I��������
	Choose();
	//�I�������J�[�\�������肷��
	Decision();


	//�J�[�\���p�̉摜�̏ꏊ��ݒ肷��
	m_cursor->SetPosition({ m_cursor->GetPosition().x,
							m_choices[m_pauseState]->GetPosition().y + 25,
							0.0f });


	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->UpdateSprite();
	}
	m_pause->UpdateSprite();
	m_cursor->UpdateSprite();
	m_background->UpdateSprite();
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
	m_background->Activate();

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
	m_background->Deactivate();

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
			//�|�[�Y��ʂ̑�����ł��Ȃ��悤�ɂ���B
			m_flagOperation = false;
			break;
		case enQuit:
			//�u�I���v
			//StageBase�ɏI����Ԃ��Ɠ`����
			m_quitFlag = true;
			//buttonASE�����[�v�Đ����I�t�ōĐ�����B
			m_buttonASE->Play(false);
			//�|�[�Y��ʂ̑�����ł��Ȃ��悤�ɂ���B
			m_flagOperation = false;
			break;
		default:
			break;
		}

	}
}