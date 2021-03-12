#include "stdafx.h"
#include "OObox.h"

bool OObox::StartSub()
{
	Init("Assets/modelData/box.tkm", enBox);

	//m_scale = { 3.0f,3.0f,3.0f };

	return true;
}