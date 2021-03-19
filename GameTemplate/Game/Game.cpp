#include "stdafx.h"
#include "Game.h"


//スタート関数
bool Game::Start()
{
	//タイトルを生成する
	NewGO<Title>(0, "Title");

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

//一番上に描画する関数
void Game::PostRender(RenderContext& rc)
{
	Font font;			//フォント
	wchar_t text[256];	//表示するテキスト
	//テキストをセット
	//フレーム数をセットする
	wsprintf(text, L"フレーム数%02d", m_frameNum);

	//描画開始
	font.Begin(rc);

	//描画処理
	font.Draw(text,				//表示したいテキスト
		{ -640.0f,350.0f },		//ポジション
		{ 1.0f,1.0f,1.0f,1.0f },	//カラー
		0.0f,						//回転
		1.0f,						//拡大
		{ 0.0f,0.0f }				//ピボット
	);

	//描画終了
	font.End(rc);

	//フレーム数をカウントする
	m_frameNum++;

	//0～99の間でカウントさせる
	if (m_frameNum >= 100)
	{
		m_frameNum = 0;
	}
}

//アップデート関数
void Game::Update()
{

	//デバック用
	//後で消す
	m_lb = CLevelObjectManager::GetInstance()->GetLevelObjects();

	//ブレイクポイント用
	int a = 4;

	return;
}