#pragma once
#include "ReversibleObject.h"
#include "OOpadlock.h"

/**
 * @brief 「鍵」「南京錠」の定数データ
*/
namespace key_padlockConstData
{
	//南京錠が開くときのサウンドのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_BREAK_PADLOCK = L"Assets/sound/padlockbreak.wav";
	//南京錠が開くときのサウンドの音量
	constexpr const float SOUND_VOLUME_BREAK_PADLOCK = 1.0f;
}

/// <summary>
/// 反転オブジェクト
/// 鍵、南京錠オブジェクト
/// </summary>
class ROkey_padlock : public CReversibleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

	/// <summary>
	/// クエリしてほしいタイミングで呼ばれる関数
	/// </summary>
	void QuerySub()override final;

private:	//privateなメンバ関数

	/**
	 * @brief サウンドの初期化処理
	*/
	void InitSound();

	/**
	 * @brief 自身が「鍵」で、「大きな南京錠」と衝突した時の処理
	 * @param [in] bigPadlock 「大きな南京錠」のポインタ
	*/
	void KeyHitBigPadlock(OOpadlock* bigPadlock);

private:	//データメンバ
	CSoundCue* m_padlockbreakSE = nullptr;	//南京錠が開くときのサウンド
	bool m_actionFlag = true;				//行動できるか？
};

