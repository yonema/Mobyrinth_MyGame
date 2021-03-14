#pragma once
#include "StageBase.h"


class stage_kari : public IStageBase
{
public:
	bool StartSub() override final;
	~stage_kari();
	void RetryStage()override final
	{
		NewGO<stage_kari>(0);
	}

private:	//データメンバ
};

