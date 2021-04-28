#pragma once
#include "ObstacleObject.h"

class CUFOLandingPoint : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

private:	//デバック用
	//後で消す
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };
};

