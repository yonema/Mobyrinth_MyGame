#pragma once
#include "ObstacleObject.h"
class OObox : public CObstacleObject
{
public:
	bool StartSub() override final;
	const bool GetBlocked()const
	{
		return m_blocked;
	}
	void SetBlocked(const bool blocked)
	{
		m_blocked = blocked;
	}
private:
	bool m_blocked = false;
};

