#pragma once
#include "ObstacleObject.h"

class OOwall :public CObstacleObject
{
public:
	bool StartSub() override final;
	void UpdateSub()override final;
	void Running()
	{
		m_moveFlag = true;
	}
	void Stop()
	{
		m_moveFlag = false;
	}
private:
	bool m_moveFlag = false;
	int m_moveCounter = 0;
};

