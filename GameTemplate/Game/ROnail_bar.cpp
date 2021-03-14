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
		QueryLOs<OOpadlock>(enBigPadlock, [&](OOpadlock* padlock) -> bool
			{
				if (IsHitObject(*this, *padlock, hitDot))
				{
					if (m_actionFlag)
					{
						DeleteGO(padlock);
						DeleteGO(this);
						m_actionFlag = false;
					}
				}
				return true;
			}
		);
		QueryLOs<OObox>(enBox, [&](OObox* box) -> bool
			{
				if (IsHitObject(*this, *box, hitDot))
				{
					if (m_actionFlag)
					{
						DeleteGO(box);
						DeleteGO(this);
						m_actionFlag = false;
					}
				}
				return true;
			}
		);

	}
}