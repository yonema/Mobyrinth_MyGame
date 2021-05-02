#include "stdafx.h"
#include "Game.h"
#include "testModel.h"


//スタート関数
bool Game::Start()
{
	//タイトルを生成する
	NewGO<Title>(0, "Title");

	//デバック用
	//後で消す
	//NewGO<testModel>(0);


	return true;
}


//デストラクタ
Game::~Game()
{
	//オブジェクトがないかもしれないからクエリしてあったら消す。
	QueryGOs<Title>("Title", [&](Title* title)->bool
		{
			//タイトルを破棄
			DeleteGO(title);
			return true;
		});
}

////一番上に描画する関数
//void Game::PostRender(RenderContext& rc)
//{
//	Font font;			//フォント
//	wchar_t text[256];	//表示するテキスト
//	//テキストをセット
//	//フレーム数をセットする
//	wsprintf(text, L"フレーム数%02d");
//
//	//描画開始
//	font.Begin(rc);
//
//	//描画処理
//	font.Draw(text,				//表示したいテキスト
//		{ -640.0f,350.0f },		//ポジション
//		{ 1.0f,1.0f,1.0f,1.0f },	//カラー
//		0.0f,						//回転
//		1.0f,						//拡大
//		{ 0.0f,0.0f }				//ピボット
//	);
//
//	//描画終了
//	font.End(rc);
//}

//アップデート関数
void Game::Update()
{

	//デバック用
	//後で消す
	m_lb = CLevelObjectManager::GetInstance()->GetLevelObjects();

	//ブレイクポイント用
	int a = 4;


	//if (g_pad[0]->IsTrigger(enButtonB))
	//{
	//	m_textType++;
	//	if (m_textType >= enObjectTypeNum)
	//		m_textType = enEmpty + 1;

	//	m_tips.SetText(m_textType);
	//}
	//else if (g_pad[0]->IsTrigger(enButtonX))
	//{
	//	m_tips.ClearText();
	//}


	//if (g_pad[0]->IsTrigger(enButtonB))
	//{

	//	//g_sceneChange->SetInOrOut(CSceneChange::enIn);
	//	//g_sceneChange->SetInOrOut(m_inorout);
	//	if (m_inorout)
	//		g_sceneChange->RandomWipeOut();
	//	else
	//		g_sceneChange->WipeIn();
	//	m_inorout = !m_inorout;
	//	//g_sceneChange->WipeStart();
	//}
	//else if (g_pad[0]->IsTrigger(enButtonX))
	//{
	//	m_wipeNum++;
	//	if (m_wipeNum > CSceneChange::enCheckerboardWipe)
	//	{
	//		m_wipeNum = 0;
	//	}
	//}
	//デバック用ここまで

	return;
}