#include "stdafx.h"
#include "stage_kari.h"
#include "Player_kari.h"
#include "background_kari.h"
#include "DirectionLight.h"

bool stage_kari::Start()
{
	CDirectionLight* d1 = NewGO<CDirectionLight>(0, "directionLight1");
	d1->SetDirection({ 1.0f,1.0f,-1.0f });
	d1->SetColor({ 0.1f,0.1f,0.1f,1.0f });


	m_level.Init("Assets/level/stage_kari01.tkl", [&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"player_kari") == true)
			{
				Player_kari* pJimen_kari;
				pJimen_kari = NewGO<Player_kari>(0);
				pJimen_kari->SetPosition(objData.position);
				return true;
			}
			
			return false;
		
		});

	g_camera3D->SetPosition({ 0.0f,1800.0f,1000.0f });
	g_camera3D->SetTarget({ 0.0f,1800.0f,0.0f });
	g_camera3D->SetNear(0.5f);
	g_camera3D->SetFar(50000.0f);
	return true;
}
stage_kari::~stage_kari()
{
	//DeleteGO(m_Player_kari);
	//DeleteGO(m_background_kari);
}
void stage_kari::Update()
{
}
void stage_kari::Render(RenderContext& rc)
{
	//m_level.Draw(rc);
	////フォント
	//Font font;
	//wchar_t text[256];
	//wsprintf(text, L"バンドリ！%d", m_numText);
	//font.Begin(rc);

	//font.Draw(text,				//表示したいテキスト
	//	{ 0.0f,0.0f },		//ポジション
	//	{ 1.0f,1.0f,1.0f,1.0f },	//カラー
	//	0.0f,						//回転
	//	1.0f,						//拡大
	//	{ 0.0f,0.0f }				//ピボット
	//);

	//font.End(rc);

	m_numText++;
	if (m_numText >= 100)
	{
		m_numText = 0;
	}
}

void stage_kari::PostRender(RenderContext& rc)
{
	//フォント
	Font font;
	wchar_t text[256];
	wsprintf(text, L"バンドリ！%d", m_numText);
	font.Begin(rc);

	font.Draw(text,				//表示したいテキスト
		{ 0.0f,0.0f },		//ポジション
		{ 1.0f,1.0f,1.0f,1.0f },	//カラー
		0.0f,						//回転
		1.0f,						//拡大
		{ 0.0f,0.0f }				//ピボット
	);

	font.End(rc);
}