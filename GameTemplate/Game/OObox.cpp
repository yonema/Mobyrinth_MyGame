#include "stdafx.h"
#include "OObox.h"

bool OObox::StartSub()
{
	Init("Assets/modelData/box.tkm", enBox);
	//OBBÇÃÉTÉCÉYÇê›íË
	Vector3 obbSize;
	obbSize = { 200.0f,200.0f,200.0f };
	GetOBB().SetDirectionLength(obbSize);

	return true;
}