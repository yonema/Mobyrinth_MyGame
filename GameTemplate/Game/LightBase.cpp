#include "stdafx.h"
#include "LightBase.h"
#include "LightManager.h"

CLightBase::CLightBase()
{
	//CLightManager::GetInstance()->AddLight(this);
	StartSub();
}

CLightBase::~CLightBase()
{
	//CLightManager::GetInstance()->RemoveLight(this);
	OnDestroySub();
}