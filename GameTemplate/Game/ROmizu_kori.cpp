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
	QueryLOs<ROmizu_kori>(enIce, [&](ROmizu_kori* mizu_kori) -> bool
		{
			if (mizu_kori == this)
				return true;

			Vector3 diff = m_position - mizu_kori->GetPosition();
			if (diff.Length() <= 100.0f)
			{
				DeleteGO(mizu_kori);
			}
			return true;
		}
		);
}