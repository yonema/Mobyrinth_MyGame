#include "stdafx.h"
#include "stage_proto01.h"


bool stage_proto01::StartSub()
{
	LoadLevel("Assets/level/stage_proto01.tkl");
	m_oneway = NewGO<OOoneway>(0);
	m_oneway->SetPosition({ -500.0f,-1400.0f,-100.0f });
	//m_oneway->SetLeftOrRight(OOoneway::enRight);
	//m_oneway->SetScale({ -1.0f,1.0f,1.0f });
	return true;
}
stage_proto01::~stage_proto01()
{
	DeleteGO(m_oneway);
}

