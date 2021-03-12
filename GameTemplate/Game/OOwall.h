#pragma once
#include "ObstacleObject.h"
class ROrunning_stop;

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
	void SetRun_stop(ROrunning_stop* run_stop)
	{
		m_pRun_stop = run_stop;
	}
	ROrunning_stop* GetRun_stop()
	{
		return m_pRun_stop;
	}
private:
	bool m_moveFlag = false;
	int m_moveCounter = 0;
	ROrunning_stop* m_pRun_stop = nullptr;
};

