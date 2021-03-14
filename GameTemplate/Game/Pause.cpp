#include "stdafx.h"
#include "Pause.h"


bool CPause::Start()
{
	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i] = NewGO<CFontRender>(0);
	}
	m_fontRender[enPause]
		->Init(L"Pause",
		{ -150.0f, 300.0f },
		{ 1.0f,1.0f,1.0f,1.0f },
		0.0f,
		2.0f
	);
	m_fontRender[enContinue]
		->Init(L"Continue",
		{ -150.0f, 150.0f }
	);	
	m_fontRender[enRetry]
		->Init(L"Retry",
		{ -150.0f, 0.0f }
	);
	m_fontRender[enQuit]
		->Init(L"Quit",
		{ -150.0f, -150.0f }
	);

	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i]->Deactivate();
	}

	m_arrowFR = NewGO<CFontRender>(0);
	m_arrowFR
		->Init(L"->",
			{ -200.0f,145.0f },
			{ 0.0f,0.0f,0.0f,1.0f }
	);
	m_arrowFR->Deactivate();

	return true;
}

CPause::~CPause()
{
	for (int i = 0; i < enPauseStateNum; i++)
	{
		DeleteGO(m_fontRender[i]);
	}
	DeleteGO(m_arrowFR);
}

void CPause::UpdateWhenPaused()
{
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		if (m_isPaused)
		{
			//ポーズ中なら
			UnPause();
		}
		else
		{
			//ポーズ中でないなら
			ToPause();
		}
	}
}

void CPause::UpdateOnlyPaused()
{

	Choose();
	Decision();

	Vector2 arrowPos;
	arrowPos.x = -200.0f;
	arrowPos.y = 300.0f - m_pauseState * 150.0f - 5.0f;
	m_arrowFR->SetPosition(arrowPos);
}
void CPause::ToPause()
{
	m_isPaused = true;
	SetGamePaused(m_isPaused);
	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i]->Activate();
	}
	m_arrowFR->Activate();
}
void CPause::UnPause()
{
	m_isPaused = false;
	SetGamePaused(m_isPaused);
	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i]->Deactivate();
	}
	m_arrowFR->Deactivate();
}
void CPause::Choose()
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
		m_pauseState++;
		if (m_pauseState >= enQuit)
		{
			m_pauseState = enQuit;
		}
	}
	else if (g_pad[0]->GetLStickYF() > 0.0f && m_buttonFlag)
	{
		//上入力
		m_buttonFlag = false;
		m_pauseState--;
		if (m_pauseState <= enContinue)
		{
			m_pauseState = enContinue;
		}
	}

}

void CPause::Decision()
{
	if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//Aボタン入力
		m_buttonFlag = false;

		switch (m_pauseState)
		{
		case enContinue:
			UnPause();
			break;
		case enRetry:
			break;
		case enQuit:
			break;
		default:
			break;
		}




	}
}