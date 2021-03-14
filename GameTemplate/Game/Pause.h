#pragma once
#include "FontRender.h"

template <class StageClass>
std::function<void(StageClass* stage)> NewStage;


class CPause : public IGameObject
{
public:
	bool Start()override final;
	~CPause();
	void UpdateWhenPaused()override final;
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

};

