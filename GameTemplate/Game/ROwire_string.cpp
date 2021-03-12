#include "stdafx.h"
#include "ROwire_string.h"


bool ROwire_string::StartSub()
{
	Init("Assets/modelData/wire.tkm", enWire,
		"Assets/modelData/string.tkm", enString);

	return true;
}

void ROwire_string::QuerySub()
{
	const float hitDot = 0.005f;

	if (GetObjectType() == enWire)
	{
		QueryLOs<ROkeymold_empty>(enKeymold, [&](ROkeymold_empty* keymold) -> bool
			{
				if (IsHitObject(*this, *keymold, hitDot))
				{
					//NewGO<>(0, );
					DeleteGO(this);
				}
				return true;
			}
		);
	}
	else if (GetObjectType() == enString)
	{
		//QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
		//	{
		//		if (IsHitObject(*this, *bigFire, hitDot))
		//		{
		//			bigFire->Damage();
		//			DeleteGO(this);
		//		}
		//		return true;
		//	}
		//);
	}
}