#pragma once
#include "ObstacleObject.h"


class OObigFire : public CObstacleObject
{
public:
	bool StartSub() override final;
	void Damage();
private:
	int m_maxHp = 3;
	int m_hp = m_maxHp;
};

