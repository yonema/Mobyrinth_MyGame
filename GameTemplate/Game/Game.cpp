#include "stdafx.h"
#include "Game.h"

//#define SOUND_OFF	//定義されていたらサウンドをオフにする。

//スタート関数
bool CGame::Start()
{
	//タイトルを生成する
	NewGO<CTitle>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_TITLE));


#ifdef SOUND_OFF
	//BGMの音量を消す
	CSoundCue::SetBGMVolume(0.0f);
	//SEの音量を消す
	CSoundCue::SetSEVolume(0.0f);
#endif

	return true;
}


//デストラクタ
CGame::~CGame()
{
	//オブジェクトがないかもしれないからクエリしてあったら消す。
	QueryGOs<CTitle>(GetGameObjectName(EN_GO_TYPE_TITLE), [&](CTitle* title)->bool
		{
			//タイトルを破棄
			DeleteGO(title);
			return true;
		});

	return;
}

//デバック用
#ifdef MY_DEBUG
//アップデート関数
void CGame::Update()
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