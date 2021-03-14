#include "stdafx.h"
#include "stage_proto01.h"


bool stage_proto01::Start()
{
	m_stageMode.LoadLevel("Assets/level/stage_proto01.tkl");

	return true;
}
stage_proto01::~stage_proto01()
{
	
}

