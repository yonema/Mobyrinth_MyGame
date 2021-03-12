#pragma once
#include "ReversibleObject.h"
class ROkeymold_empty : public CReversibleObject
{
public:
	bool StartSub() override final;
	void QuerySub()override final;
};

