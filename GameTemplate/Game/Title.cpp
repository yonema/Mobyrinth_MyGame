#include "stdafx.h"
#include "Title.h"



bool Title::Start()
{
	m_titleFR = NewGO<CFontRender>(0);
	m_titleFR->Init(L"メビリンス",
		{ -200.0f,300.0f },
		{ 0.5f,0.5f,1.0f,1.0f },
		0.0f,
		3.0f
	);

	m_pushAButtonFR = NewGO<CFontRender>(0);
	m_pushAButtonFR->Init(L"Aボタンを押してね。",
		{ -150.0f,0.0f },
		{ 1.0f,0.0f,0.0f,1.0f }
	);

	m_title = NewGO<CSpriteRender>(1);
	m_title->Init("Assets/sprite/Title.dds", 700.0f, 300.0f);
	m_title->SetPosition({ -200.0f,150.0f,0.0f });


	const float leftSide = -100.0f;
	const float UpSide = 300.0f;
	const float DownSide = -300.0f;
	const float BetweenLine = (DownSide - UpSide) / enStageNum;

	for (int i = 0; i < enStageNum; i++)
	{
		m_stageName[i] = NewGO<CFontRender>(0);
	}
	m_stageName[enStage_kari]->Init(L"stage_kari",
		{ leftSide ,UpSide + BetweenLine * enStage_kari }
	);
	m_stageName[enStageProto01]->Init(L"stage_proto01",
		{ leftSide ,UpSide + BetweenLine * enStageProto01 }
	);
	m_stageName[enStageProto02]->Init(L"stage_proto02",
		{ leftSide ,UpSide + BetweenLine * enStageProto02 }
	);
	for (int i = 0; i < enStageNum; i++)
	{
		m_stageName[i]->Deactivate();
	}
	m_arrow = NewGO<CFontRender>(0);
	m_arrow->Init(L"->",
		{ leftSide - 50.0f,UpSide - 5.0f }
	);
	m_arrow->Deactivate();

	return true;
}

Title::~Title()
{
	DeleteGO(m_titleFR);
	DeleteGO(m_pushAButtonFR);
	for (int i = 0; i < enStageNum; i++)
	{
		DeleteGO(m_stageName[i]);
	}
	DeleteGO(m_arrow);

}

void Title::Update()
{
	switch (m_state)
	{
	case enTitleScreen:
		TitleScreen();
		break;
	case enStageSelect:
		StageSelect();
		break;
	default:
		break;
	}
}

void Title::TitleScreen()
{
	if (g_pad[0]->GetLStickYF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//何も入力がない状態
		m_buttonFlag = true;
	}
	else if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		m_buttonFlag = false;
		m_state = enStageSelect;
		m_titleFR->Deactivate();
		m_pushAButtonFR->Deactivate();
		for (int i = 0; i < enStageNum; i++)
		{
			m_stageName[i]->Activate();
		}
		m_arrow->Activate();
	}

}

void Title::StageSelect()
{
	if (g_pad[0]->GetLStickYF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//何も入力がない状態
		m_buttonFlag = true;
	}
	if (g_pad[0]->GetLStickYF() < 0.0f && m_buttonFlag)
	{
		//下入力
		m_buttonFlag = false;
		m_stageState++;
		if (m_stageState >= enStageProto02)
		{
			m_stageState = enStageProto02;
		}
	}
	else if (g_pad[0]->GetLStickYF() > 0.0f && m_buttonFlag)
	{
		//上入力
		m_buttonFlag = false;
		m_stageState--;
		if (m_stageState <= enStage_kari)
		{
			m_stageState = enStage_kari;
		}
	}



	else if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//Aボタン入力
		m_buttonFlag = false;

		////////////////////////////////////////////////////////////
		//ステージを新しく作成した場合、ここでNewGOを行う。
		////////////////////////////////////////////////////////////
		switch (m_stageState)
		{
		case enStage_kari:
			NewGO<stage_kari>(0, "stage_kari");
			break;
		case enStageProto01:
			NewGO<stage_proto01>(0, "stage_proto01");
			break;
		case enStageProto02:
			NewGO<stage_proto02>(0, "stage_proto02");
			break;
		default:
			break;
		}
		Release();
	}

	else if (g_pad[0]->IsTrigger(enButtonB) && m_buttonFlag)
	{
		m_buttonFlag = false;
		m_state = enTitleScreen;
		m_titleFR->Activate();
		m_pushAButtonFR->Activate();
		for (int i = 0; i < enStageNum; i++)
		{
			m_stageName[i]->Deactivate();
		}
		m_arrow->Deactivate();
	}

	const float leftSide = -100.0f;
	const float UpSide = 300.0f;
	const float DownSide = -300.0f;
	const float BetweenLine = (DownSide - UpSide) / enStageNum;
	m_arrow->SetPosition({ leftSide - 50.0f , UpSide + BetweenLine * m_stageState - 5.0f });
}