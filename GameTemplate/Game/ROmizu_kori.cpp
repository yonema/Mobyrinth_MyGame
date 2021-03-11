#include "stdafx.h"
#include "ROmizu_kori.h"

bool ROmizu_kori::StartSub()
{
	Init("Assets/modelData/mizu.tkm", enWater,
		"Assets/modelData/koori.tkm", enIce);

	return true;
}

void ROmizu_kori::QuerySub()
{
	const float hitLen = 100.0f;

	if (GetObjectType() == enWater)
	{
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				Vector3 diff = m_position - bigFire->GetPosition();
				if (diff.Length() <= hitLen)
				{
					bigFire->Damage();
					DeleteGO(this);
				}
				return true;
			}
		);
	}
	else if (GetObjectType() == enIce)
	{
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{

				Vector3 diff = m_position - bigFire->GetPosition();
				if (diff.Length() <= hitLen)
				{
					bigFire->Damage();
					DeleteGO(this);
				}
				return true;
			}
		);
	}
}