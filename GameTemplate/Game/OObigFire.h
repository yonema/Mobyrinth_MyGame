#pragma once
#include "ObstacleObject.h"
#include "PointLight.h"
#include "effect/Effect.h"

/// <summary>
/// 障害オブジェクト
/// 炎オブジェクト
/// </summary>
class OObigFire : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;		//スタート関数
	~OObigFire();						//デストラクタ
	void UpdateSub() override final;	//アップデート関数

public:		//メンバ関数
	void Damage();	//ダメージを受ける
	void Burn(); //燃えるSE

private:
	int m_maxHp = 3;						//最大HP
	int m_hp = m_maxHp;						//現在のHP
	CPointLight* m_pointLight = nullptr;	//ポイントライト

	CSoundCue* m_flameSE = nullptr;			//flameのサウンドキュー
	Effect* m_flame_water = nullptr;		//炎と水の反応
	CSoundCue* m_flame_waterSE = nullptr;	//炎と水の反応

};

