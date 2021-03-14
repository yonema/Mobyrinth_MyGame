#pragma once
#include "StageMode.h"



class stage_proto02 : public IGameObject
{
public:
	bool Start() override final;
	~stage_proto02();
private:
	CStageMode m_stageMode;
};

