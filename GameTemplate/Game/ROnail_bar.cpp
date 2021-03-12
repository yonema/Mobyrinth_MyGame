#include "stdafx.h"
#include "ROnail_bar.h"

bool ROnail_bar::StartSub()
{
	Init("Assets/modelData/nail.tkm", enNail,
		"Assets/modelData/bar.tkm", enBar);

	return true;
}

void ROnail_bar::QuerySub()
{
	const float hitDot = 0.005f;

	if (GetObjectType() == enNail)
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
	else if (GetObjectType() == enBar)
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
		QueryLOs<OOpadlock>(enPadlock, [&](OOpadlock* padlock) -> bool
			{
				if (IsHitObject(*this, *padlock, hitDot))
				{
					DeleteGO(padlock);
					DeleteGO(this);
				}
				return true;
			}
		);
	}
}