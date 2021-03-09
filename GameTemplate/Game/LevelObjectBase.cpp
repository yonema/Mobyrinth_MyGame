#include "stdafx.h"
#include "LevelObjectBase.h"

bool CLevelObjectBase::Init
(const char* filePath_front, const int type_front,
	const char* filePath_back, const int type_back)
{
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_modelRender[i] = NewGO<CModelRender>(0);
	}
	m_modelRender[enFront]->Init(filePath_front);
	m_modelRender[enBack]->Init(filePath_back);
	m_objectType[enFront] = type_front;
	m_objectType[enBack] = type_back;

	return true;
}
bool CLevelObjectBase::Start()
{
	StartSub();
	SetFrontOrBack(m_frontOrBack);

	return true;
}

void CLevelObjectBase::SetFrontOrBack(bool frontOrBack)
{
	m_frontOrBack = frontOrBack;
	if (m_modelRender[m_frontOrBack])
	m_modelRender[m_frontOrBack]->Activate();
	if (m_modelRender[!m_frontOrBack])
	m_modelRender[!m_frontOrBack]->Deactivate();

}

void CLevelObjectBase::Update()
{
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_modelRender[i]->SetPosition(m_position);
		m_modelRender[i]->SetRotation(m_rotation);
	}
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		SetFrontOrBack(!m_frontOrBack);
	}
	return;
}

CLevelObjectBase::~CLevelObjectBase()
{
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		DeleteGO(m_modelRender[i]);
	}
}