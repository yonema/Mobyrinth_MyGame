#pragma once
#include "StageBase.h"

class stage_proto02 : public IStageBase
{
public:
	bool StartSub() override final;
	~stage_proto02();
	void RetryStage()override final
	{
		NewGO<stage_proto02>(0);
	}
private:
};

