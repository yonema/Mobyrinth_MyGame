#pragma once

//基本的に必要
#include "DirectionLight.h"
#include "GameCamera.h"

//レベルのロードで必要
#include "Level.h"
#include "LevelObjectManager.h"

//基本オブジェクト
#include "Player.h"
#include "Mobius.h"

//「反転オブジェクト」ReversibleObject
#include "ROmizu_kori.h"
#include "RObird_fish.h"
#include "ROrunning_stop.h"
#include "ROwire_string.h"
#include "ROnail_bar.h"
#include "ROaxe_pickaxe.h"
#include "ROkeymold_empty.h"

//「障害オブジェクト」ObstacleObject
#include "OObigFire.h"
#include "OOwall.h"
#include "OOgoal.h"
#include "OOpadlock.h"
#include "OObox.h"



class CStageMode
{
public:
	CStageMode();
	~CStageMode();
	/// <summary>
	/// レベルのロード
	/// </summary>
	/// <param name="filePath">tklのファイルパス</param>
	void LoadLevel(const char* tklFilePath);
	CDirectionLight* GetStageDirectionLight()
	{
		return m_stageDirectionLight;
	}
private:
	CLevel m_level;
	CDirectionLight* m_stageDirectionLight = nullptr;
};

