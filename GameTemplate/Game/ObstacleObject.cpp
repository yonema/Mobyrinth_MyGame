#include "stdafx.h"
#include "ObstacleObject.h"

bool CObstacleObject::Init(const char* filePath, int objectType)
{
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init(filePath);
	SetObjectType(objectType);

	return StartSub();
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

	return;
}