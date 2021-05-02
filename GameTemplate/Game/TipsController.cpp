#include "stdafx.h"
#include "TipsController.h"

bool CTipsController::Start()
{
	return true;
}

void CTipsController::Update()
{
	int objectType = CLevelObjectManager::GetInstance()->GetNearestObjectType();

	m_tips.SetText(objectType);
	
}