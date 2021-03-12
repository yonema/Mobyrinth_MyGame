#pragma once
#include "ReversibleObject.h"
#include "OObox.h"
#include "OOpadlock.h"

class ROnail_bar : public CReversibleObject
{
public:
	bool StartSub() override final;
	void QuerySub()override final;
};

