#include "stdafx.h"
#include "stage_kari.h"

bool stage_kari::StartSub()
{
	//LoadLevel("Assets/level/stage_kari02.tkl");
	//LoadLevel("Assets/level/Y_easy.tkl");
	LoadLevel("Assets/level/Y_normal.tkl");

	


	//デバック用
	//後で消す
	//m_ufo = NewGO<CUFO>(0, "UFO");
	//m_ufo->SetPosition({ -500.0f,-1500.0f,100.0f });
	//m_reverseALL = NewGO< OOReverseALL>(0);
	//m_reverseALL->SetPosition({ 0.0f,-1700.0f,0.0f });

	return true;
}
stage_kari::~stage_kari()
{

}


