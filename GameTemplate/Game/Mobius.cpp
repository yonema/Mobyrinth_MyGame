#include "stdafx.h"
#include "Mobius.h"

bool Mobius::Start()
{

	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init("Assets/modelData/Mobius.tkm");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	return true;
}

Mobius::~Mobius()
{
	DeleteGO(m_modelRender);
}

