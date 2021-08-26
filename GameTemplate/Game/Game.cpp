#include "stdafx.h"
#include "Game.h"

#define SOUND_OFF	//定義されていたらサウンドをオフにする。

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{

	//スタート関数
	bool CGame::Start()
	{
		//この関数内で
		//共通定数データを使用可能にする
		using namespace nsCommonData;

		//タイトルを生成する
		NewGO<nsTitle::CTitle>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_TITLE));


#ifdef SOUND_OFF
		//BGMの音量を消す
		nsSound::CSoundCue::SetBGMVolume(0.0f);
		//SEの音量を消す
		nsSound::CSoundCue::SetSEVolume(0.0f);
#endif

		return true;
	}


	//デストラクタ
	CGame::~CGame()
	{
		//この関数内で
		//共通定数データを使用可能にする
		using namespace nsCommonData;

		//オブジェクトがないかもしれないからクエリしてあったら消す。
		QueryGOs<nsTitle::CTitle>(GetGameObjectName(EN_GO_TYPE_TITLE), [&](nsTitle::CTitle* title)->bool
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
}