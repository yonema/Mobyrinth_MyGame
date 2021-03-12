#include "stdafx.h"
#include "ROkeymold_empty.h"

bool ROkeymold_empty::StartSub()
{
	Init("Assets/modelData/key mold.tkm", enKeymold,
		"Assets/modelData/key mold.tkm", enKeymold);

	return true;
}

void ROkeymold_empty::QuerySub()
{
	const float hitDot = 0.005f;

	if (GetObjectType() == enKeymold)
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
	//else if (GetObjectType() == enKeymold)
	//{
	//	//QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
	//	//	{
	//	//		if (IsHitObject(*this, *bigFire, hitDot))
	//	//		{
	//	//			bigFire->Damage();
	//	//			DeleteGO(this);
	//	//		}
	//	//		return true;
	//	//	}
	//	//);
	//}
}