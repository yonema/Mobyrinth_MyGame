#pragma once
#include "ReversibleObject.h"
#include "OOwall.h"

class ROrunning_stop : public CReversibleObject
{
public:
	bool StartSub() override final;
	void QuerySub()override final;
	void UpdateSub()override final;
private:
	OOwall* m_pWall = nullptr;
};

