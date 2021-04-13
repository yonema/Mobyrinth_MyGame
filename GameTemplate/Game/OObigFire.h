#pragma once
#include "ObstacleObject.h"
#include "PointLight.h"

class OObigFire : public CObstacleObject
{
public:
	bool StartSub() override final;
	~OObigFire();
	void Damage();
private:
	int m_maxHp = 3;
	int m_hp = m_maxHp;
	CPointLight* m_pointLight = nullptr;
};

