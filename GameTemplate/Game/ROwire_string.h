#pragma once
#include "ReversibleObject.h"
#include "ROkeymold_empty.h"
#include "ROkey_padlock.h"


class ROwire_string : public CReversibleObject
{
public:
	bool StartSub() override final;
	void QuerySub()override final;
};
