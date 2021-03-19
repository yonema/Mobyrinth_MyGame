#include "stdafx.h"
#include "Pause.h"

//スタート関数
bool CPause::Start()
{
	//フォントレンダラーを生成
	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i] = NewGO<CFontRender>(0);
	}

	//フォントの左端の場所
	const float leftSidePos = -150.0f;

	//フォントレンダラーの初期化。テキストをセットする。
	//「Pause」をテキストにセット
	m_fontRender[enPause]
		->Init(L"Pause",
		{ leftSidePos, 300.0f },
		{ 1.0f,1.0f,1.0f,1.0f },
		0.0f,
		2.0f
	);
	//「Pause」をテキストにセット
	m_fontRender[enContinue]
		->Init(L"Continue",
		{ leftSidePos, 150.0f }
	);	
	m_fontRender[enRetry]
		->Init(L"Retry",
		{ leftSidePos, 0.0f }
	);
	m_fontRender[enQuit]
		->Init(L"Quit",
		{ leftSidePos, -150.0f }
	);

	
	for (int i = 0; i < enPauseStateNum; i++)
	{
		//PostRenderFlagをtrueにして一番上に表示するようにする
		m_fontRender[i]->SetPostRenderFlag(true);
		//フォントを無効化して表示しないようにする。
		m_fontRender[i]->Deactivate();
	}

	
	//フォントレンダラーの生成
	m_arrowFR = NewGO<CFontRender>(0);
	//フォントレンダラーの初期化
	//カーソルの矢印のテキストを設定
	m_arrowFR
		->Init(L"->",
			{ -200.0f,145.0f },
			{ 0.0f,0.0f,0.0f,1.0f }
	);
	//一番上に表示するようにする
	m_arrowFR->SetPostRenderFlag(true);
	//無効化して非表示にする。
	m_arrowFR->Deactivate();

	return true;
}

//デストラクタ
CPause::~CPause()
{
	//ポーズ状態を解除する
	UnPause();

	//フォントレンダラーの消去
	for (int i = 0; i < enPauseStateNum; i++)
	{
		DeleteGO(m_fontRender[i]);
	}
	DeleteGO(m_arrowFR);
}


/// <summary>
/// ポーズ中でも呼ばれるアップデート関数
/// ポーズ中でなくても常に呼ばれる
/// </summary>
void CPause::UpdateWhenPaused()
{
	//スタートボタンを押したら
	//（キーボードならEnterを押したら）
	if (g_pad[0]->IsTrigger(enButtonStart))
	{
		if (m_isPaused)
		{
			//ポーズ中なら
			//ポーズ状態を解除
			UnPause();
		}
		else
		{
			//ポーズ中でないなら
			//ポーズ状態にする
			ToPause();
		}
	}
}


/// <summary>
/// ポーズ中にだけ呼ばれるアップデート関数
/// </summary>
void CPause::UpdateOnlyPaused()
{
	//カーソルを選択させる
	Choose();
	//選択したカーソルを決定する
	Decision();

	//カーソルの座標
	Vector2 arrowPos;
	//左端の数値を入れる
	arrowPos.x = -200.0f;
	//上端から、幅のおおきさ*現在のステート分下に下げる
	arrowPos.y = 300.0f - m_pauseState * 150.0f - 5.0f;
	//カーソルの位置を設定する
	m_arrowFR->SetPosition(arrowPos);
}

/// <summary>
/// ポーズ状態にする関数
/// </summary>
void CPause::ToPause()
{
	//ポーズできない状態なら、何もしないでreturn
	if (m_canPause == false) 
	{
		return;
	}

	//ポーズ中にする
	m_isPaused = true;

	//IGameObjectにポーズ中だと伝える。
	//Update()が呼ばれなくなって、
	//UpdateOnlyPaused()が呼ばれるようになる
	SetGamePaused(m_isPaused);

	//ポーズ中に表示されるフォントを有効化して
	//表示できるようにする。
	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i]->Activate();
	}
	m_arrowFR->Activate();

}

/// <summary>
/// ポーズ状態から解除する関数
/// </summary>
void CPause::UnPause()
{
	//ポーズ中でない状態にする。
	m_isPaused = false;

	//IGameObjectにポーズ中を解除するように伝える
	//Update()が呼ばれるようになって、
	//UpdateOnlyPaused()が呼ばれなくなる。
	SetGamePaused(m_isPaused);

	//ポーズ中に表示されるフォントを無効化して
	//表示できなくなるようにする。
	for (int i = 0; i < enPauseStateNum; i++)
	{
		m_fontRender[i]->Deactivate();
	}
	m_arrowFR->Deactivate();

}

/// <summary>
///	ポーズ中にカーソル選択させる関数
/// </summary>
void CPause::Choose()
{
	//ボタンの入力を調べる
	if (g_pad[0]->GetLStickYF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//何も入力がない状態
		//ボタンを押すことができるようにする。
		m_buttonFlag = true;
	}
	if (g_pad[0]->GetLStickYF() < 0.0f && m_buttonFlag)
	{
		//下入力
		//ボタンを押すことができないようにする。（連続入力防止用）
		m_buttonFlag = false;
		//現在のポーズ中のステートを加算する
		m_pauseState++;
		//一番下のenQuitまで行ったら、
		//enQuitより下に行かないようにする
		if (m_pauseState > enQuit)
		{
			m_pauseState = enQuit;
		}
	}
	else if (g_pad[0]->GetLStickYF() > 0.0f && m_buttonFlag)
	{
		//上入力
		//ボタンを押すことができないようにする。（連続入力防止用）
		m_buttonFlag = false;
		//現在のポーズ中のステートを減算する
		m_pauseState--;
		//一番上のenContinueまで行ったら、
		//enContinueより上に行かないようにする
		if (m_pauseState < enContinue)
		{
			m_pauseState = enContinue;
		}
	}

}

/// <summary>
/// 選択したカーソルで決定する関数
/// </summary>
void CPause::Decision()
{
	//ボタンの入力を調べる
	if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//Aボタン入力
		//ボタンを押すことができないようにする。（連続入力防止用）
		m_buttonFlag = false;

		//現在のステートによって
		//決定の反応を振り分ける
		switch (m_pauseState)
		{
		case enContinue:
			//「続ける」
			//ポーズ中を解除する
			UnPause();
			break;
		case enRetry:
			//「リトライ」
			//StageBaseにリトライ状態だと伝える
			m_retryFlag = true;
			break;
		case enQuit:
			//「終了」
			//StageBaseに終了状態だと伝える
			m_quitFlag = true;
			break;
		default:
			break;
		}

	}
}