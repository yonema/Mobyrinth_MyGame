#include "stdafx.h"
#include "Game.h"


bool Game::Start()
{
	//NewGO<stage_proto01>(0, "stage_proto01");
	NewGO<stage_kari>(0, "stage_kari");

	return true;
}

Game::~Game()
{
	//DeleteGO(FindGO<stage_proto01>("stage_proto01"));
	DeleteGO(FindGO<stage_kari>("stage_kari"));
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