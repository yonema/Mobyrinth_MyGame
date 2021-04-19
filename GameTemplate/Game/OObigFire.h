#pragma once
#include "ObstacleObject.h"
#include "PointLight.h"

/// <summary>
/// 障害オブジェクト
/// 炎オブジェクト
/// </summary>
class OObigFire : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数
	~OObigFire();					//デストラクタ

public:		//メンバ関数
	void Damage();	//ダメージを受ける
private:
	int m_maxHp = 3;						//最大HP
	int m_hp = m_maxHp;						//現在のHP
	CPointLight* m_pointLight = nullptr;	//ポイントライト
};

