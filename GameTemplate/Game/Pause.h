#pragma once
#include "FontRender.h"

class CPause : public IGameObject
{
public:
	bool Start()override final;
	~CPause();
	/// <summary>
	/// ポーズ中でも呼ばれるUpdate（更新）
	/// </summary>
	void UpdateWhenPaused()override final;
	/// <summary>
	/// ポーズ中にだけ呼ばれるUpdate（更新）
	/// </summary>
	void UpdateOnlyPaused()override final;

public:
	const bool GetRetryFlag()const
	{
		return m_retryFlag;
	}
	const bool GetQuitFlag()const
	{
		return m_quitFlag;
	}
	void SetCanPause(const bool b)
	{
		m_canPause = b;
	}

private:
	void ToPause();
	void UnPause();
	void Choose();
	void Decision();
private:
	enum ENPauseState
	{
		enPause,
		enContinue,
		enRetry,
		enQuit,
		enPauseStateNum
	};
	CFontRender* m_fontRender[enPauseStateNum] = { nullptr };
	CFontRender* m_arrowFR = nullptr;

	bool m_isPaused = false;
	int m_pauseState = enContinue;
	bool m_buttonFlag = true;

	bool m_retryFlag = false;
	bool m_quitFlag = false;
	bool m_canPause = false;
};

