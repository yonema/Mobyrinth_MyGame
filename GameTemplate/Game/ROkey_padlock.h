#pragma once
#include "ReversibleObject.h"
#include "OOpadlock.h"

class ROkey_padlock : public CReversibleObject
{
public:
	bool StartSub() override final;
	void QuerySub()override final;
};

