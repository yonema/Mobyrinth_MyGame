#pragma once
#include "StageMode.h"


class stage_kari : public IGameObject
{
public:
	bool Start() override final;
	~stage_kari();

private:	//�f�[�^�����o
	CStageMode m_stageMode;

};

