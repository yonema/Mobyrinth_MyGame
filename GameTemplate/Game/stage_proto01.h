#pragma once
#include "level.h"
#include "font/Font.h"
#include "ModelRender.h"
#include "GameCamera.h"
#include "Mobius.h"
#include "ROmizu_kori.h"
#include "LevelObjectManager.h"
#include "OObigFire.h"
#include "RObird_fish.h"
#include "ROrunning_stop.h"
#include "OOwall.h"
#include "OOgoal.h"
class stage_proto01 : public IGameObject
{
public:
	bool Start() override final;
	~stage_proto01();
private:
	CLevel m_level;
	CModelRender* m_modelRender = nullptr;
};

