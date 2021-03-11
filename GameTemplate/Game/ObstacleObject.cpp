#include "stdafx.h"
#include "ObstacleObject.h"

bool CObstacleObject::Init(const char* filePath, int objectType)
{
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init(filePath);
	SetObjectType(objectType);

	return true;
}

CObstacleObject::~CObstacleObject()
{
	Delete();

	DeleteGO(m_modelRender);

}

bool CObstacleObject::PureVirtualStart()
{
	CheckWayPoint();

	return StartSub();
}

void CObstacleObject::Update()
{
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->SetScale(m_scale);

	return;
}