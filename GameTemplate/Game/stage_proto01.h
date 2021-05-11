#pragma once
#include "StageBase.h"
#include "OOoneway.h"

class stage_proto01 : public IStageBase
{
public:
	bool StartSub() override final;
	~stage_proto01();
	void RetryStage()override final
	{
		NewGO<stage_proto01>(0);
	}

private:
	OOoneway* m_oneway = nullptr;
};

