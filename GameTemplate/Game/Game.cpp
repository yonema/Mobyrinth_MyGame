#include "stdafx.h"
#include "Game.h"

bool Game::Start()
{
	//NewGO<stage_kari>(0, "stage_kari");
	//NewGO<stage_proto01>(0, "stage_proto01");
	//NewGO<stage_proto02>(0, "stage_proto02");

	m_spriteRender = NewGO<CSpriteRender>(0);
	m_spriteRender->Init("Assets/Image/sample.dds", 256.0f, 256.0f);
	//m_spriteRender->SetPosition({ 0.0f,0.0f,0.0f });
	NewGO<Title>(0, "Title");

	return true;
}

Game::~Game()
{
	//オブジェクトがないかもしれないからクエリしてあったら消す。
	QueryGOs<Title>("Title", [&](Title* title)->bool
		{
			DeleteGO(title);
			return true;
		});
}

void Game::PostRender(RenderContext& rc)
{
	//フォント
	Font font;
	wchar_t text[256];
	wsprintf(text, L"フレーム数%02d", m_numText);
	font.Begin(rc);

	font.Draw(text,				//表示したいテキスト
		{ -640.0f,350.0f },		//ポジション
		{ 1.0f,1.0f,1.0f,1.0f },	//カラー
		0.0f,						//回転
		1.0f,						//拡大
		{ 0.0f,0.0f }				//ピボット
	);

	font.End(rc);

	m_numText++;

	if (m_numText >= 100)
	{
		m_numText = 0;
	}
}