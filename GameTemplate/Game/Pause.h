#pragma once
#include "FontRender.h"

class CPause : public IGameObject
{
public:
	bool Start()override final;
	~CPause();
	void UpdateWhenPaused()override final;
	void UpdateOnlyPaused()override final;
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
};

