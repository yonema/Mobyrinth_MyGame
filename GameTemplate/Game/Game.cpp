#include "stdafx.h"
#include "Game.h"
#include "testModel.h"

#define SOUND_OFF	//��`����Ă�����T�E���h���I�t�ɂ���B

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

////��ԏ�ɕ`�悷��֐�
//void Game::PostRender(RenderContext& rc)
//{
//	Font font;			//�t�H���g
//	wchar_t text[256];	//�\������e�L�X�g
//	//�e�L�X�g���Z�b�g
//	//�t���[�������Z�b�g����
//	wsprintf(text, L"�t���[����%02d");
//
//	//�`��J�n
//	font.Begin(rc);
//
//	//�`�揈��
//	font.Draw(text,				//�\���������e�L�X�g
//		{ -640.0f,350.0f },		//�|�W�V����
//		{ 1.0f,1.0f,1.0f,1.0f },	//�J���[
//		0.0f,						//��]
//		1.0f,						//�g��
//		{ 0.0f,0.0f }				//�s�{�b�g
//	);
//
//	//�`��I��
//	font.End(rc);
//}

//�A�b�v�f�[�g�֐�
void Game::Update()
{

	//�f�o�b�N�p
	//��ŏ���
	m_lb = CLevelObjectManager::GetInstance()->GetLevelObjects();

	//�u���C�N�|�C���g�p
	int a = 4;

	//�f�o�b�N�p�����܂�

	return;
}