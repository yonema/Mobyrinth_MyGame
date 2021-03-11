#include "stdafx.h"
#include "ROrunning_stop.h"

bool ROrunning_stop::StartSub()
{
	Init("Assets/modelData/kadou.tkm", enRunning,
		"Assets/modelData/teishi.tkm", enStop);

	return true;
}

void ROrunning_stop::QuerySub()
{
	const float hitLen = 100.0f;

	if (GetObjectType() == enRunning)
	{
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{
				Vector3 diff = m_position - wall->GetPosition();
				if (diff.Length() <= hitLen)
				{
					wall->Running();
				}
				return true;
			}
		);
	}
	else if (GetObjectType() == enStop)
	{
		QueryLOs<OOwall>(enWall, [&](OOwall* wall) -> bool
			{

				Vector3 diff = m_position - wall->GetPosition();
				if (diff.Length() <= hitLen)
				{
					wall->Stop();
				}
				return true;
			}
		);
	}
}