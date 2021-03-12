#include "stdafx.h"
#include "ROaxe_pickaxe.h"

bool ROaxe_pickaxe::StartSub()
{
	Init("Assets/modelData/axe.tkm", enAxe,
		"Assets/modelData/pickaxe.tkm", enPickaxe);

	return true;
}

void ROaxe_pickaxe::QuerySub()
{
	const float hitDot = 0.005f;

	if (GetObjectType() == enAxe)
	{
		QueryLOs<OObox>(enBox, [&](OObox* box) -> bool
			{
				if (IsHitObject(*this, *box, hitDot))
				{
					DeleteGO(box);
					DeleteGO(this);
				}
				return true;
			}
		);
	}
	else if (GetObjectType() == enPickaxe)
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