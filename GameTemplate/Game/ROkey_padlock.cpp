#include "stdafx.h"
#include "ROkey_padlock.h"


bool ROkey_padlock::StartSub()
{
	Init("Assets/modelData/kagi.tkm", enKey,
		"Assets/modelData/nankinjyo.tkm", enROPadlock);

	return true;
}

void ROkey_padlock::QuerySub()
{
	const float hitDot = 0.005f;

	if (GetObjectType() == enKey)
	{
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
	//else if (GetObjectType() == enROPadlock)
	//{
	//	QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
	//		{
	//			if (IsHitObject(*this, *bigFire, hitDot))
	//			{
	//				bigFire->Damage();
	//				DeleteGO(this);
	//			}
	//			return true;
	//		}
	//	);
	//}
}