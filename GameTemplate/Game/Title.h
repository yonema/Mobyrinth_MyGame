#pragma once
#include "FontRender.h"
#include "SpriteRender.h"

//背景
#include "Mobius.h"
#include "Player.h"

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

	CSpriteRender* m_title = nullptr;

private: //背景
	Mobius* m_mobius = nullptr;
	Player* player = nullptr;
};

