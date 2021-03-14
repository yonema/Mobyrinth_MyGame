#pragma once
#include "StageMode.h"

class stage_proto01 : public IGameObject
{
public:
	bool Start() override final;
	~stage_proto01();
private:
	CStageMode m_stageMode;
};

