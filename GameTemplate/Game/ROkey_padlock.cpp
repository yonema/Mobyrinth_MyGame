#include "stdafx.h"
#include "ROkey_padlock.h"

//スタート関数
bool ROkey_padlock::StartSub()
{
	//初期化用関数
	Init("Assets/modelData/key.tkm", EN_RO_TYPE_KEY,
		"Assets/modelData/padlock.tkm", EN_RO_TYPE_PADLOCK);



	//padlockbreakSEのサウンドキューを生成する
	m_padlockbreakSE = NewGO<CSoundCue>(0);
	//padlockbreakSEのサウンドキューを、waveファイルを指定して初期化する。
	m_padlockbreakSE->Init(L"Assets/sound/padlockbreak.wav");
	//音量調節
	m_padlockbreakSE->SetVolume(1.0f);

	return true;
}

/// <summary>
/// クエリしてほしいタイミングで呼ばれる関数
/// </summary>
void ROkey_padlock::QuerySub()
{
	//自身が「鍵の金型」の時
	if (GetObjectType() == EN_RO_TYPE_KEY)
	{
		//障害オブジェクトの「持てない南京錠」をクエリ
		QueryLOs<OOpadlock>(EN_OO_TYPE_BIG_PADLOCK, [&](OOpadlock* padlock) -> bool
			{
				//自身と「持てない南京錠」が衝突したら
				if (IsHitObject(*this, *padlock))
				{
					//「持てない南京錠」を破棄
					DeleteGO(padlock);

					//padlockbreakSEをループ再生をオフで再生する。
					m_padlockbreakSE->Play(false);

					//自身のオブジェクトを破棄
					DeleteGO(this);
				}
				return true;
			}
		);
	}
	//else if (GetObjectType() == enROPadlock)
	//{
	//	QueryLOs<OObigFire>(EN_OO_TYPE_FLAME, [&](OObigFire* bigFire) -> bool
	//		{
	//			if (IsHitObject(*this, *bigFire, hitDot))
	//			{
	//				bigFire->Damage();
	//				DeleteGO(this);
	//			}
	//			return true;
	//		}
	//	);
	//}
}