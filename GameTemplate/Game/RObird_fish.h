#pragma once
#include "ReversibleObject.h"
#include "OObigFire.h"

class RObird_fish : public CReversibleObject
{
public:
	bool StartSub() override final;
	void QuerySub()override final;
	void UpdateSub()override final;
	void SetFrontOrBackSub()override final;
private:
	CModelRender* m_otherModelRender[enFrontAndBackNum] = { nullptr };
	int m_reversibleType[enFrontAndBackNum] = { enEnpty };
};

