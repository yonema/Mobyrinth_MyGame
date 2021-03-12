#pragma once
#include "Level.h"
#include "ModelRender.h"
#include "GameCamera.h"

#include "OOgoal.h"

#include "ROwire_string.h"
#include "ROnail_bar.h"
#include "ROaxe_pickaxe.h"
#include "OOpadlock.h"
#include "OObox.h"
#include "ROkeymold_empty.h"



class stage_proto02 : public IGameObject
{
public:
	bool Start() override final;
	~stage_proto02();
private:
	CLevel m_level;
	CModelRender* m_modelRender = nullptr;
};

