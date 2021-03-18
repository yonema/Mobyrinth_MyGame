#pragma once
#include "FontRender.h"

/// <summary>
/// ポーズクラス
/// </summary>
class CPause : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;		//スタート関数
	~CPause();						//デストラクタ
	/// <summary>
	/// ポーズ中でも呼ばれるアップデート関数
	/// </summary>
	void UpdateWhenPaused()override final;
	/// <summary>
	/// ポーズ中にだけ呼ばれるアップデート関数
	/// </summary>
	void UpdateOnlyPaused()override final;

public:		//ここのメンバ関数を主に使う
	/// <summary>
	/// リトライが選択されたかどうかを戻す関数
	/// リトライが選択されたらtrueを戻す。
	/// </summary>
	/// <returns>リトライが選択されたか？</returns>
	const bool GetRetryFlag()const
	{
		return m_retryFlag;
	}

	/// <summary>
	/// 終了が選択されたかどうかを戻す関数。
	/// 終了が選択されたらtrueを戻す。
	/// </summary>
	/// <returns></returns>
	const bool GetQuitFlag()const
	{
		return m_quitFlag;
	}

	/// <summary>
	/// ポーズできるかできないかを設定する関数
	/// falseを設定したらポーズできない
	/// </summary>
	/// <param name="b">ポーズできるか？</param>
	void SetCanPause(const bool b)
	{
		m_canPause = b;
	}

private:	//privateなメンバ関数
	/// <summary>
	/// ポーズ状態にする関数
	/// </summary>
	void ToPause();
	/// <summary>
	/// ポーズ状態から解除する関数
	/// </summary>
	void UnPause();
	/// <summary>
	///	ポーズ中にカーソル選択させる関数
	/// </summary>
	void Choose();
	/// <summary>
	/// 選択したカーソルで決定する関数
	/// </summary>
	void Decision();
private:	//データメンバ

	/// <summary>
	///	フォントに使う文字の種類と
	/// ポーズ中のステート（状態）
	/// </summary>
	enum ENPauseState
	{
		enPause,			//ポーズ。フォントのみで使用
		enContinue,			//続ける、を選択した状態
		enRetry,			//リトライ、を選択した状態
		enQuit,				//終了、を選択した状態
		enPauseStateNum		//ステートの数。フォントのみで使用
	};


	CFontRender* m_fontRender[enPauseStateNum] = { nullptr };	//フォントレンダラー
	CFontRender* m_arrowFR = nullptr;							//フォントレンダラー

	int m_pauseState = enContinue;		//現在のポーズ中のステート（状態）
	bool m_isPaused = false;			//ポーズ中かどうか？
	bool m_buttonFlag = true;			//ボタンを入力できるかどうか？
	bool m_retryFlag = false;			//リトライが選択されているか？
	bool m_quitFlag = false;			//終了が選択されているか？
	bool m_canPause = false;			//ポーズできるか？
};

