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

#include "ROwire_string.h"
#include "ROnail_bar.h"
#include "ROaxe_pickaxe.h"
#include "OOpadlock.h"
#include "OObox.h"
#include "ROkeymold_empty.h"


class CLevel;
class Player_kari;
class background_kari;

class stage_kari : public IGameObject
{
public:
	bool Start() override final;
	~stage_kari();
	void Update() override final;

private:	//データメンバ
	Player_kari* m_Player_kari = nullptr;
	background_kari* m_background_kari = nullptr;
	CLevel m_level;

	CModelRender* m_modelRender = nullptr;

};

