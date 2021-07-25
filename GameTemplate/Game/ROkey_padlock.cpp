#include "stdafx.h"
#include "ROkey_padlock.h"

//ReversibleObjectのモデルのファイルパスの定数データを使用可能にする
using namespace ROsFilepathConstdata;
//「鍵」「南京錠」の定数データを使用可能にする
using namespace key_padlockConstData;

//スタート関数
bool ROkey_padlock::StartSub()
{
	//初期化用関数
	Init(MODEL_FILEPATH_KEY, EN_RO_TYPE_KEY,
		MODEL_FILEPATH_PADLOCK, EN_RO_TYPE_PADLOCK);

	//サウンドの初期化処理
	InitSound();

	return true;
}

/**
 * @brief サウンドの初期化処理
*/
void ROkey_padlock::InitSound()
{
	//南京錠が開くときのサウンドを生成する
	m_padlockbreakSE = NewGO<CSoundCue>(PRIORITY_FIRST);
	//南京錠が開くときのサウンドを、waveファイルを指定して初期化する。
	m_padlockbreakSE->Init(SOUND_FILEPATH_BREAK_PADLOCK);
	//音量調節
	m_padlockbreakSE->SetVolume(SOUND_VOLUME_BREAK_PADLOCK);

	return;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROkey_padlock::QuerySub()
{
	//自身が「鍵の金型」の時
	if (GetObjectType() == EN_RO_TYPE_KEY)
	{
		//障害オブジェクトの「大きな南京錠」をクエリ
		QueryLOs<OOpadlock>(EN_OO_TYPE_BIG_PADLOCK, [&](OOpadlock* padlock) -> bool
			{
				//自身と「大きな南京錠」が衝突したら
				if (IsHitLevelObject(*this, *padlock))
				{
					//「大きな南京錠」を破棄
					DeleteGO(padlock);

					//南京錠が開くときのサウンドをワンショット再生で再生する
					m_padlockbreakSE->Play(false);

					//自身のオブジェクトを破棄
					DeleteGO(this);
				}
				return true;
			}
		);
	}

	return;

}