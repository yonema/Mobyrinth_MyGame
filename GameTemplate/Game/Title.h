#pragma once
#include "FontRender.h"

//�X�e�[�W
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
	/// �X�e�[�W�̔ԍ�
	/// �V�����X�e�[�W��������ꍇ�A�����ɔԍ���ǉ�
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

