#pragma once
#include "StageMode.h"


class stage_kari : public IGameObject
{
public:
	bool Start() override final;
	~stage_kari();

private:	//データメンバ
	CStageMode m_stageMode;

};

