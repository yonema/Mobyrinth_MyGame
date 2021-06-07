#include "stdafx.h"
#include "Game.h"
#include "testModel.h"

//#define SOUND_OFF	//定義されていたらサウンドをオフにする。

//スタート関数
bool Game::Start()
{
	//タイトルを生成する
	NewGO<Title>(0, "Title");

	//デバック用
	//後で消す
	//NewGO<testModel>(0);

#ifdef SOUND_OFF
	//BGMの音量を消す
	CSoundCue::SetBGMVolume(0.0f);
	//SEの音量を消す
	CSoundCue::SetSEVolume(0.0f);
#endif

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

//デバック用
#ifdef MY_DEBUG
//アップデート関数
void Game::Update()
{

	//デバック用
	//後で消す

	//ブレイクポイントを置いて現在生成されているオブジェクトを確認できる
	m_lb = CLevelObjectManager::GetInstance()->GetLevelObjects();

	//ブレイクポイント用
	int a = 4;

	//デバック用ここまで

	return;
}
#endif