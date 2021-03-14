#include "stdafx.h"
#include "Game.h"

bool Game::Start()
{


	m_spriteRender = NewGO<CSpriteRender>(0);
	m_spriteRender->Init("Assets/Image/sample.dds", 256.0f, 256.0f);
	//m_spriteRender->SetPosition({ 0.0f,0.0f,0.0f });
	NewGO<Title>(0, "Title");

	return true;
}

Game::~Game()
{
	//�I�u�W�F�N�g���Ȃ���������Ȃ�����N�G�����Ă�����������B
	QueryGOs<Title>("Title", [&](Title* title)->bool
		{
			DeleteGO(title);
			return true;
		});
}

void Game::PostRender(RenderContext& rc)
{
	//�t�H���g
	Font font;
	wchar_t text[256];
	wsprintf(text, L"�t���[����%02d", m_numText);
	font.Begin(rc);

	font.Draw(text,				//�\���������e�L�X�g
		{ -640.0f,350.0f },		//�|�W�V����
		{ 1.0f,1.0f,1.0f,1.0f },	//�J���[
		0.0f,						//��]
		1.0f,						//�g��
		{ 0.0f,0.0f }				//�s�{�b�g
	);

	font.End(rc);

	m_numText++;

	if (m_numText >= 100)
	{
		m_numText = 0;
	}
}

void Game::Update()
{
	m_lb = CLevelObjectManager::GetInstance()->GetLevelObjects();


	int a = 4;

	return;
}