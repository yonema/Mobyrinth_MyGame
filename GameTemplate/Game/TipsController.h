#pragma once
#include "Tips.h"
#include "LevelObjectManager.h"

class CTipsController : public IGameObject
{
public:
	bool Start() override final;
	void Update() override final;
private:
	CTips m_tips;
};

