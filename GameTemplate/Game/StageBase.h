#pragma once

//��{�I�ɕK�v
#include "DirectionLight.h"
#include "GameCamera.h"

//�|�[�Y��ʗp
#include "Pause.h"

//���x���̃��[�h�ŕK�v
#include "Level.h"
#include "LevelObjectManager.h"

//��{�I�u�W�F�N�g
#include "Player.h"
#include "Mobius.h"

//�u���]�I�u�W�F�N�g�vReversibleObject
#include "ROmizu_kori.h"
#include "RObird_fish.h"
#include "ROrunning_stop.h"
#include "ROwire_string.h"
#include "ROnail_bar.h"
#include "ROaxe_pickaxe.h"
#include "ROkeymold_empty.h"

//�u��Q�I�u�W�F�N�g�vObstacleObject
#include "OObigFire.h"
#include "OOwall.h"
#include "OOgoal.h"
#include "OOpadlock.h"
#include "OObox.h"



class IStageBase : public IGameObject
{
public:
	bool Start()override final;
	~IStageBase();
	void Update()override final;
	void UpdateOnlyPaused()override final;

private:
	void Clear();
	void Retry();
	void Quit();

	void Goal();
protected:
	/// <summary>
	/// ���x���̃��[�h
	/// </summary>
	/// <param name="filePath">tkl�̃t�@�C���p�X</param>
	void LoadLevel(const char* tklFilePath);
	CDirectionLight* GetStageDirectionLight()
	{
		return m_stageDirectionLight;
	}
	virtual bool StartSub() = 0 { return true; };
	virtual void RetryStage() = 0 {};
	void Release()
	{
		DeleteGO(this);
	}
private:
	CLevel m_level;
	CDirectionLight* m_stageDirectionLight = nullptr;
	CPause* m_pause = nullptr;
	OOgoal* m_goal = nullptr;
	int m_goalCounter = 0;
};

