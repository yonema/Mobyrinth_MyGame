#include "stdafx.h"
#include "LOmizu_kori.h"

bool LOmizu_kori::StartSub()
{
	Init("Assets/modelData/mizu.tkm", enWater,
		"Assets/modelData/koori.tkm", enIce);

	return true;
}