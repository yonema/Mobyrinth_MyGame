#include "stdafx.h"
#include "Game.h"

//#define SOUND_OFF	//��`����Ă�����T�E���h���I�t�ɂ���B

//�X�^�[�g�֐�
bool CGame::Start()
{
	//�^�C�g���𐶐�����
	NewGO<CTitle>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_TITLE));


#ifdef SOUND_OFF
	//BGM�̉��ʂ�����
	CSoundCue::SetBGMVolume(0.0f);
	//SE�̉��ʂ�����
	CSoundCue::SetSEVolume(0.0f);
#endif

	return true;
}


//�f�X�g���N�^
CGame::~CGame()
{
	//�I�u�W�F�N�g���Ȃ���������Ȃ�����N�G�����Ă�����������B
	QueryGOs<CTitle>(GetGameObjectName(EN_GO_TYPE_TITLE), [&](CTitle* title)->bool
		{
			//�^�C�g����j��
			DeleteGO(title);
			return true;
		});

	return;
}

//�f�o�b�N�p
#ifdef MY_DEBUG
//�A�b�v�f�[�g�֐�
void CGame::Update()
{

	//�f�o�b�N�p
	//��ŏ���

	//�u���C�N�|�C���g��u���Č��ݐ�������Ă���I�u�W�F�N�g���m�F�ł���
	m_lb = CLevelObjectManager::GetInstance()->GetLevelObjects();

	//�u���C�N�|�C���g�p
	int a = 4;

	//�f�o�b�N�p�����܂�

	return;
}
#endif