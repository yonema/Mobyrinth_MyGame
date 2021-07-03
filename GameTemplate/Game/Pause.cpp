#include "stdafx.h"
#include "Pause.h"

//スタート関数
bool CPause::Start()
{
	//画像の表示
	m_level2D.Init("Assets/level2D/Pause_2.casl", [&](Level2DObjectData& objdata)
		{
			//名前が一致でフックする
			//カーソル
			if (objdata.EqualObjectName("Pause"))
			{
				m_pause = NewGO<CSpriteRender>(3);
				m_pause->Init("Assets/level2D/Pause.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_pause->SetScale(objdata.scale);
				m_pause->SetPosition(objdata.position);
				m_pause->SetPostRenderFlag(true);
				//フックしたらtrueを戻す
				return true;
			}
			//ゲームに戻る
			else if (objdata.EqualObjectName("Continue"))
			{
				m_choices[enContinue] = NewGO<CSpriteRender>(3);
				m_choices[enContinue]->Init("Assets/level2D/Continue.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_choices[enContinue]->SetScale(objdata.scale);
				m_choices[enContinue]->SetPosition(objdata.position);
				m_choices[enContinue]->SetPostRenderFlag(true);
				//フックしたらtrueを戻す
				return true;
			}
			//リトライ
			else if (objdata.EqualObjectName("Retry"))
			{
				m_choices[enRetry] = NewGO<CSpriteRender>(3);
				m_choices[enRetry]->Init("Assets/level2D/Retry.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_choices[enRetry]->SetScale(objdata.scale);
				m_choices[enRetry]->SetPosition(objdata.position);
				m_choices[enRetry]->SetPostRenderFlag(true);
				//フックしたらtrueを戻す
				return true;
			}
			//ステージから出る
			else if (objdata.EqualObjectName("Quit"))
			{
				m_choices[enQuit] = NewGO<CSpriteRender>(3);
				m_choices[enQuit]->Init("Assets/level2D/Quit.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_choices[enQuit]->SetScale(objdata.scale);
				m_choices[enQuit]->SetPosition(objdata.position);
				m_choices[enQuit]->SetPostRenderFlag(true);
				//フックしたらtrueを戻す
				return true;
			}
			//カーソル
			else if (objdata.EqualObjectName("cursor"))
			{
				m_cursor = NewGO<CSpriteRender>(3);
				m_cursor->Init("Assets/level2D/cursor.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_cursor->SetScale(objdata.scale);
				m_cursor->SetPosition(objdata.position);
				m_cursor->SetPostRenderFlag(true);
				//フックしたらtrueを戻す
				return true;
			}
			//ポーズ中の背景
			else if (objdata.EqualObjectName("Pause_background"))
			{
				m_background = NewGO<CSpriteRender>(2);
				m_background->Init(
					objdata.ddsFilePath,
					static_cast<float>(g_FRAME_BUFFER_W),
					static_cast<float>(g_FRAME_BUFFER_H),
					{ 0.5f,0.5f },
					AlphaBlendMode_Trans
				);
				m_background->SetScale(objdata.scale);
				m_background->SetPosition(objdata.position);
				m_background->SetPostRenderFlag(true);
				//フックしたらtrueを戻す
				return true;
			}


			//そのまま表示するからfalseを戻す
			return false;
		});


	for (int i = 0; i < enPauseNum; i++) {
		m_choices[i]->Deactivate();
	}
	m_pause->Deactivate();
	m_cursor->Deactivate();
	m_background->Deactivate();

	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->UpdateSprite();
	}
	m_pause->UpdateSprite();
	m_cursor->UpdateSprite();
	m_background->UpdateSprite();


	//buttonASEのサウンドキューを生成する
	m_buttonASE = NewGO<CSoundCue>(0);
	//buttonASEのサウンドキューを、waveファイルを指定して初期化する。
	m_buttonASE->Init(L"Assets/sound/buttonA.wav");
	//音量調節
	m_buttonASE->SetVolume(0.5f);

	//buttonBのサウンドキューを生成する
	m_buttonBSE = NewGO<CSoundCue>(0);
	//buttonBのサウンドキューを、waveファイルを指定して初期化する。
	m_buttonBSE->Init(L"Assets/sound/buttonB.wav");
	//音量調節
	m_buttonBSE->SetVolume(0.5f);

	//selectSEのサウンドキューを生成する
	m_selectSE = NewGO<CSoundCue>(0);
	//selectSEのサウンドキューを、waveファイルを指定して初期化する。
	m_selectSE->Init(L"Assets/sound/select.wav");
	//音量調節
	m_selectSE->SetVolume(0.5f);

	return true;
}

//デストラクタ
CPause::~CPause()
{
	//ポーズ状態を解除する
	UnPause();

	//フォントレンダラーの消去
	for (int i = 0; i < enPauseNum; i++)
	{
		DeleteGO(m_choices[i]);
	}
	DeleteGO(m_pause);
	DeleteGO(m_cursor);
	DeleteGO(m_background);
}


/// <summary>
/// ポーズ中でも呼ばれるアップデート関数
/// ポーズ中でなくても常に呼ばれる
/// </summary>
void CPause::AlwaysUpdate()
{
	//ポーズ画面の操作ができない場合、この関数の処理を行わない。
	if (m_flagOperation == false) {
		return;
	}

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
	//ポーズ画面の操作ができない場合、この関数の処理を行わない。
	if (m_flagOperation == false) {
		return;
	}

	//カーソルを選択させる
	Choose();
	//選択したカーソルを決定する
	Decision();


	//カーソル用の画像の場所を設定する
	m_cursor->SetPosition({ m_cursor->GetPosition().x,
							m_choices[m_pauseState]->GetPosition().y + 25,
							0.0f });


	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->UpdateSprite();
	}
	m_pause->UpdateSprite();
	m_cursor->UpdateSprite();
	m_background->UpdateSprite();
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
	
	//buttonASEをループ再生をオフで再生する。
	m_buttonASE->Play(false);

	//IGameObjectにポーズ中だと伝える。
	//Update()が呼ばれなくなって、
	//UpdateOnlyPaused()が呼ばれるようになる
	SetGamePaused(m_isPaused);

	//ポーズ中に表示されるフォントを有効化して
	//表示できるようにする。
	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->Activate();
	}
	m_pause->Activate();
	m_cursor->Activate();
	m_background->Activate();

}

/// <summary>
/// ポーズ状態から解除する関数
/// </summary>
void CPause::UnPause()
{
	//ポーズ中でない状態にする。
	m_isPaused = false;

	if (g_pad[0]->IsTrigger(enButtonStart)) {
		
		//buttonBをループ再生をオフで再生する。
		m_buttonBSE->Play(false);

	}

	//IGameObjectにポーズ中を解除するように伝える
	//Update()が呼ばれるようになって、
	//UpdateOnlyPaused()が呼ばれなくなる。
	SetGamePaused(m_isPaused);

	//ポーズ中に表示されるフォントを無効化して
	//表示できなくなるようにする。
	for (int i = 0; i < enPauseNum; i++)
	{
		m_choices[i]->Deactivate();
	}
	m_pause->Deactivate();
	m_cursor->Deactivate();
	m_background->Deactivate();

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
		
		//selectSEをループ再生をオフで再生する。
		m_selectSE->Play(false);

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

		//selectSEをループ再生をオフで再生する。
		m_selectSE->Play(false);

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
			//buttonBをループ再生をオフで再生する。
			m_buttonBSE->Play(false);
			break;

		case enRetry:
			//「リトライ」
			//StageBaseにリトライ状態だと伝える
			m_retryFlag = true;
			//buttonASEをループ再生をオフで再生する。
			m_buttonASE->Play(false);
			//ポーズ画面の操作をできないようにする。
			m_flagOperation = false;
			break;
		case enQuit:
			//「終了」
			//StageBaseに終了状態だと伝える
			m_quitFlag = true;
			//buttonASEをループ再生をオフで再生する。
			m_buttonASE->Play(false);
			//ポーズ画面の操作をできないようにする。
			m_flagOperation = false;
			break;
		default:
			break;
		}

	}
}