#pragma once
#include "ReversibleObject.h"
#include "OObox.h"

class ROaxe_pickaxe : public CReversibleObject
{
public:
	bool StartSub() override final;
	void QuerySub()override final;
};

