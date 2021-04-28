#include "stdafx.h"
#include "stage_kari.h"

bool stage_kari::StartSub()
{
	LoadLevel("Assets/level/stage_kari02.tkl");
	m_ufo = NewGO<CUFO>(0);
	m_ufo->SetPosition({ 0.0f,2000.0f,0.0f });
	return true;
}
stage_kari::~stage_kari()
{

}


