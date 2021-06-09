#include "stdafx.h"
#include "stage_title.h"

bool CStage_title::StartSub()
{
	LoadLevel("Assets/level/stage_title.tkl");

	return true;
}
CStage_title::~CStage_title()
{

}