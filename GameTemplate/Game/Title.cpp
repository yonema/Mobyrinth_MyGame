#include "stdafx.h"
#include "Title.h"


//スタート関数
bool Title::Start()
{
	//フォントレンダラーの生成と初期化
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
	m_title = NewGO<CSpriteRender>(0);
	m_title->Init("Assets/sprite/Title.dds", 1400.0f, 600.0f);

	//フォントの配置
	const float leftSide = -100.0f;		//左端
	const float UpSide = 300.0f;		//上端
	const float DownSide = -300.0f;		//下端
	const float BetweenLine = (DownSide - UpSide) / enStageNum;	//フォントの配置の幅

	//フォントレンダラーの生成と初期化
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

	//最初はステージ名は表示しないから、無効化して非表示にする
	for (int i = 0; i < enStageNum; i++)
	{
		m_stageName[i]->Deactivate();
	}
	m_arrow = NewGO<CFontRender>(0);
	m_arrow->Init(L"->",
		{ leftSide - 50.0f,UpSide - 5.0f }
	);
	m_arrow->Deactivate();


	//スプライトのレベルを初期化
	m_level2D.Init("Assets/level2D/test2d.casl", [&](Level2DObjectData& objdata)
		{
			//名前が一致でフックする
			if (objdata.EqualObjectName("title"))
			{
				//フックしたらtrueを戻す
				return true;
			}
			//そのまま表示するからfalseを戻す
			return false;
		});


	return true;
}

//デストラクタ
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
	default:
		break;
	}
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
		//ステージのステート（状態）をステージセレクトに移行する。
		m_stageState = enStageSelect;

		//タイトル画面用のフォントレンダラーを無効化して非表示にする
		m_titleFR->Deactivate();
		m_pushAButtonFR->Deactivate();

		//ステージセレクト用のフォントレンダラーを有効化して表示できるようにする
		for (int i = 0; i < enStageNum; i++)
		{
			m_stageName[i]->Activate();
		}
		m_arrow->Activate();
	}

}

//ステージセレクト
void Title::StageSelect()
{
	//ボタンの入力を調べる
	if (g_pad[0]->GetLStickYF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//何も入力がない状態
		//ボタンを押すことができるようにする（連続入力防止用）
		m_buttonFlag = true;
	}
	if (g_pad[0]->GetLStickYF() < 0.0f && m_buttonFlag)
	{
		//下を入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		//ステージセレクトのステートを加算する
		m_stageSelectState++;
		//ステートが最大の値になったら、それより大きくならないようにする
		if (m_stageSelectState > enStageProto02)
		{
			m_stageSelectState = enStageProto02;
		}
	}
	else if (g_pad[0]->GetLStickYF() > 0.0f && m_buttonFlag)
	{
		//上を入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

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
	else if (g_pad[0]->IsTrigger(enButtonB) && m_buttonFlag)
	{
		//Bボタンを入力
		//ボタンを押すことができないようにする（連続入力防止用）
		m_buttonFlag = false;

		//ステージのステートをタイトル画面にする
		m_stageState = enTitleScreen;
		//タイトル画面用のフォントレンダラーを有効化して表示できるようにする
		m_titleFR->Activate();
		m_pushAButtonFR->Activate();
		//ステージセレクトのフォントレンダラーを無効化して表示できないようにする
		for (int i = 0; i < enStageNum; i++)
		{
			m_stageName[i]->Deactivate();
		}
		m_arrow->Deactivate();
	}


	//フォントの配置
	const float leftSide = -100.0f;		//左端
	const float UpSide = 300.0f;		//上端
	const float DownSide = -300.0f;		//下端
	const float BetweenLine = (DownSide - UpSide) / enStageNum;	//フォントの配置の幅
	//カーソル用のフォントレンダラーの場所を設定する
	m_arrow->SetPosition({ leftSide - 50.0f , UpSide + BetweenLine * m_stageSelectState - 5.0f });
}