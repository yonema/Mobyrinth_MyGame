#pragma once
#include "ObstacleObject.h"
class OOgoal : public CObstacleObject
{
public:
	bool StartSub()override final;
	void UpdateSub()override final;
	void PostRender(RenderContext& rc)override final;
private:
	Font m_font;
	bool m_goalFlag = false;
	float m_dbg = 0.0f;
};

