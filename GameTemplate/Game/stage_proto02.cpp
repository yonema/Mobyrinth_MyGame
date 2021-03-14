#include "stdafx.h"
#include "stage_proto02.h"

bool stage_proto02::Start()
{
	m_stageMode.LoadLevel("Assets/level/stage_proto02.tkl");

	return true;
}
stage_proto02::~stage_proto02()
{

}