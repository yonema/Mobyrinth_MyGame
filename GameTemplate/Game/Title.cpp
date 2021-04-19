#include "stdafx.h"
#include "Title.h"


//スタート関数
bool Title::Start()
{

	//背景
	m_stageTitle = NewGO<stage_title>(0, "stage_title");
	m_stageTitle->SetStartUpStartDirecting(false);
	m_stageTitle->SetTitlePlayer(true);
	m_stageTitle->SetStartBGM(false);
	m_stageTitle->SetWipeInFlag(m_wipeInFlag);


	//フォントの配置
	const float leftSide = -100.0f;		//左端
	const float UpSide = 300.0f;		//上端
	const float DownSide = -300.0f;		//下端
	const float BetweenLine = (DownSide - UpSide) / enStageNum;	//フォントの配置の幅


	m_level2D.Init("Assets/level2D/Stage_selection.casl", [&](Level2DObjectData& objdata)
		{
			//名前が一致でフックする
			//カーソル
			if (objdata.EqualObjectName("cursor"))
			{
				m_cursor = NewGO<CSpriteRender>(1);
				m_cursor->Init("Assets/level2D/cursor.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_cursor->SetScale(objdata.scale);
				m_cursor->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//stage_kari
			if (objdata.EqualObjectName("Stage_icon1"))
			{
				m_stageName[enStage_kari] = NewGO<CSpriteRender>(1);
				m_stageName[enStage_kari]->Init("Assets/level2D/Stage_icon1.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_stageName[enStage_kari]->SetScale(objdata.scale);
				m_stageName[enStage_kari]->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//stage_proto01
			if (objdata.EqualObjectName("Stage_icon2"))
			{
				m_stageName[enStageProto01] = NewGO<CSpriteRender>(1);
				m_stageName[enStageProto01]->Init("Assets/level2D/Stage_icon2.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_stageName[enStageProto01]->SetScale(objdata.scale);
				m_stageName[enStageProto01]->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//stage_proto02
			if (objdata.EqualObjectName("Stage_icon3"))
			{
				m_stageName[enStageProto02] = NewGO<CSpriteRender>(1);
				m_stageName[enStageProto02]->Init("Assets/level2D/Stage_icon3.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_stageName[enStageProto02]->SetScale(objdata.scale);
				m_stageName[enStageProto02]->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//ステージセレクト
			if (objdata.EqualObjectName("Stage_selection"))
			{
				m_stageSelection = NewGO<CSpriteRender>(1);
				m_stageSelection->Init("Assets/level2D/Stage_selection.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_stageSelection->SetScale(objdata.scale);
				m_stageSelection->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//ステージ選択背景
			if (objdata.EqualObjectName("Stage_selection_base"))
			{
				m_stageSelectionBase = NewGO<CSpriteRender>(1);
				m_stageSelectionBase->Init("Assets/level2D/Stage_selection_base.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_stageSelectionBase->SetScale(objdata.scale);
				m_stageSelectionBase->SetPosition(objdata.position);
				//スプライトに色を乗算する。
				//乗算だから、0.0fを掛けると色が消えるよ。
				//透過するだけだったら、RGBのところは1.0fを掛けてね。
				m_stageSelectionBase->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
				//フックしたらtrueを戻す
				return true;
			}
			

			//そのまま表示するからfalseを戻す
			return false;
		});

	//最初はステージ名は表示しないから、無効化して非表示にする
	for (int i = 0; i < enStageNum; i++)
	{
		m_stageName[i]->Deactivate();
	}

	m_cursor->Deactivate();
	m_stageSelection->Deactivate();
	m_stageSelectionBase->Deactivate();


	//タイトル画面の表示
	m_level2D.Init("Assets/level2D/Title.casl", [&](Level2DObjectData& objdata)
		{
			//名前が一致でフックする
			if (objdata.EqualObjectName("Title"))
			{
				m_title = NewGO<CSpriteRender>(1);
				m_title->Init("Assets/level2D/Title.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_title->SetScale(objdata.scale);
				m_title->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//名前が一致でフックする
			if (objdata.EqualObjectName("Press_A_Button"))
			{
				m_pressAButton = NewGO<CSpriteRender>(1);
				m_pressAButton->Init("Assets/level2D/Press_A_Button.dds", objdata.width, objdata.height, { 0.5f,0.5f }, AlphaBlendMode_Trans);
				m_pressAButton->SetScale(objdata.scale);
				m_pressAButton->SetPosition(objdata.position);
				//フックしたらtrueを戻す
				return true;
			}
			//そのまま表示するからfalseを戻す
			return false;
		});

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
Title::~Title()
{
	DeleteGO(m_stageTitle);

	for (int i = 0; i < enStageNum; i++)
	{
		DeleteGO(m_stageName[i]);
	}
	//DeleteGO(m_arrow);

	//画像データ
	DeleteGO(m_title);
	DeleteGO(m_pressAButton);
	DeleteGO(m_cursor);
	DeleteGO(m_stageSelection);
	DeleteGO(m_stageSelectionBase);

	DeleteGO(m_bgmTitle);

}

//アップデート関数
void Title::Update()
{
	//現在のステージのステート（状態）で処理を振り分ける
	switch (m_stageState)
	{
	case enTitleScreen:
		//タイトル画面
		TitleScreen();
		break;
	case enStageSelect:
		//ステージセレクト
		StageSelect();
		break;
	case enStageDecision:
		StageDecision();
		break;
	default:
		break;
	}
	

	//Start関数ではなくUpdate関数でBGMを初期化しているのは
	//ステージをロード中にBGMが再生しないようにするため。

	//BGMを初期化していなかったら
	if (!m_initedBGM)
	{
		//BGMを初期化する
		InitBGM();
	}

}

//BGMの初期化
void Title::InitBGM()
{
	//BGMのサウンドキューを生成する
	m_bgmTitle = NewGO<CSoundCue>(0);
	//BGMのサウンドキューを、waveファイルを指定して初期化する。
	m_bgmTitle->Init(L"Assets/sound/Title.wav");
	//BGMをループ再生をオンで再生する。
	m_bgmTitle->Play(true);
	m_bgmTitle->SetVolume(0.5f);

	m_initedBGM = true;
}

//タイトル画面
void Title::TitleScreen()
{
	//ボタンの入力を調べる
	if (g_pad[0]->GetLStickYF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//何も入力がない状態
		//ボタンを押すことができるようにする（連続入力防止用）
		m_buttonFlag = true;
	}
	else if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//Aボタンを入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;
		
		//buttonASEをループ再生をオフで再生する。
		m_buttonASE->Play(false);

		//ステージのステート（状態）をステージセレクトに移行する。
		m_stageState = enStageSelect;
		//ステージ選択状態を初期設定にする。
		m_stageSelectState = enStage_kari;

		//タイトル画面用のスプライトレンダラーを無効化して非表示にする
		m_title->Deactivate();
		m_pressAButton->Deactivate();


		//ステージセレクト用のスプライトレンダラーを有効化して表示できるようにする
		for (int i = 0; i < enStageNum; i++)
		{
			m_stageName[i]->Activate();
		}

		m_cursor->Activate();
		m_stageSelection->Activate();
		m_stageSelectionBase->Activate();
	}

}

//ステージセレクト
void Title::StageSelect()
{
	//ボタンの入力を調べる
	if (g_pad[0]->GetLStickXF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//何も入力がない状態
		//ボタンを押すことができるようにする（連続入力防止用）
		m_buttonFlag = true;
	}
	if (g_pad[0]->GetLStickXF() > 0.0f && m_buttonFlag)
	{
		//下を入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		//selectSEをループ再生をオフで再生する。
		m_selectSE->Play(false);
		
		//ステージセレクトのステートを加算する
		m_stageSelectState++;
		//ステートが最大の値になったら、それより大きくならないようにする
		if (m_stageSelectState > enStageProto02)
		{
			m_stageSelectState = enStageProto02;
		}
	}
	else if (g_pad[0]->GetLStickXF() < 0.0f && m_buttonFlag)
	{
		//上を入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		//selectSEをループ再生をオフで再生する。
		m_selectSE->Play(false);
		
		//ステージセレクトのステートを減算する
		m_stageSelectState--;
		//ステートが最小の値になったら、それより小さくならないようにする
		if (m_stageSelectState <= enStage_kari)
		{
			m_stageSelectState = enStage_kari;
		}
	}
	else if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//Aボタンを入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		//ランダムワイプアウトをする
		g_sceneChange->RandomWipeOut();

		//ステージのステートをステージを決定した状態にする
		m_stageState = enStageDecision;

		//buttonAをループ再生をオフで再生する。
		m_buttonASE->Play(false);
	}
	else if (g_pad[0]->IsTrigger(enButtonB) && m_buttonFlag)
	{
		//Bボタンを入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		
		//buttonBをループ再生をオフで再生する。
		m_buttonBSE->Play(false);

		//ステージのステートをタイトル画面にする
		m_stageState = enTitleScreen;

		//ステージセレクトの画像無効化して表示できないようにする
		for (int i = 0; i < enStageNum; i++)
		{
			m_stageName[i]->Deactivate();
		}
		m_cursor->Deactivate();
		m_stageSelection->Deactivate();
		m_stageSelectionBase->Deactivate();


		//タイトル画面の画像を有効化して表示する。
		m_title->Activate();
		m_pressAButton->Activate();
	}


	//フォントの配置
	const float leftSide = -100.0f;		//左端
	const float UpSide = 300.0f;		//上端
	const float DownSide = -300.0f;		//下端
	const float BetweenLine = (DownSide - UpSide) / enStageNum;	//フォントの配置の幅
	//カーソル用の画像の場所を設定する

	m_cursor->SetPosition({ -400.0f,m_stageName[m_stageSelectState]->GetPositionY(),0.0f });
  	m_cursor->SetPosition({ m_stageName[m_stageSelectState]->GetPositionX() - 100.0f,
							m_stageName[m_stageSelectState]->GetPositionY() + 100.0f,
							0.0f });
}

//ステージを決定した時の処理
void Title::StageDecision()
{
	//ワイプが終わるまで処理しない
	if (!g_sceneChange->IsWipeFinished())
		return;

	////////////////////////////////////////////////////////////
	//ステージを新しく作成した場合、ここでNewGOを行う。
	////////////////////////////////////////////////////////////
	//ステージセレクトのステートによって生成するステージを振り分ける
	switch (m_stageSelectState)
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
	//自身のオブジェクトを破棄する
	Release();


}