#pragma once
#include "ObstacleObject.h"
#include "FontRender.h"

class OOgoal : public CObstacleObject
{
public:
	bool StartSub()override final;
	~OOgoal();
	void UpdateSub()override final;
	const bool GetIsGoal()const
	{
		return m_isGoal;
	}
private:
	CFontRender* m_fontRender = nullptr;
	bool m_isGoal = false; //ÉSÅ[ÉãîªíË
};

