#pragma once
#include "FontRender.h"

//ステージ
#include "stage_kari.h"
#include "stage_proto01.h"
#include "stage_proto02.h"


class Title : public IGameObject
{
public:
	bool Start()override final;
	~Title();
	void Update()override final;
private:
	void TitleScreen();
	void StageSelect();
	void Release()
	{
		DeleteGO(this);
	}
private:
	enum EnState
	{
		enTitleScreen,
		enStageSelect,
	};
	int m_state = enTitleScreen;
	CFontRender* m_titleFR = nullptr;
	CFontRender* m_pushAButtonFR = nullptr;
	/// <summary>
	/// ステージの番号
	/// 新しいステージを作った場合、ここに番号を追加
	/// </summary>
	enum EnStageList
	{
		enStage_kari,
		enStageProto01,
		enStageProto02,
		enStageNum,
	};
	CFontRender* m_stageName[enStageNum] = { nullptr };
	CFontRender* m_arrow = nullptr;
	int m_stageState = enStage_kari;

	bool m_buttonFlag = true;
};

