#pragma once
#include "StageBase.h"


class stage_title : public IStageBase
{
public:
	bool StartSub() override final;
	~stage_title();
	void RetryStage()override final
	{
		NewGO<stage_title>(0);
	}


private:	//データメンバ
};

