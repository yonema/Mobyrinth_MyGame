#include "stdafx.h"
#include "Game.h"
#include "testModel.h"

//#define SOUND_OFF	//��`����Ă�����T�E���h���I�t�ɂ���B

//�X�^�[�g�֐�
bool Game::Start()
{
	//�^�C�g���𐶐�����
	NewGO<Title>(0, "Title");

	//�f�o�b�N�p
	//��ŏ���
	//NewGO<testModel>(0);

#ifdef SOUND_OFF
	//BGM�̉��ʂ�����
	CSoundCue::SetBGMVolume(0.0f);
	//SE�̉��ʂ�����
	CSoundCue::SetSEVolume(0.0f);
#endif

	return true;
}


//�f�X�g���N�^
Game::~Game()
{
	//�I�u�W�F�N�g���Ȃ���������Ȃ�����N�G�����Ă�����������B
	QueryGOs<Title>("Title", [&](Title* title)->bool
		{
			//�^�C�g����j��
			DeleteGO(title);
			return true;
		});
}

//�f�o�b�N�p
#ifdef MY_DEBUG
//�A�b�v�f�[�g�֐�
void Game::Update()
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