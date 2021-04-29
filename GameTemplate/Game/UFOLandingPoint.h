#pragma once
#include "ObstacleObject.h"

class CUFOLandingPoint : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;	//スタート関数

public:
	COBB& GetSideOBB(const int leftOrRight)
	{
		return m_sideOBB[leftOrRight];
	}

	void UpdateSideOBB();

private:
	enum LeftOrRight
	{
		enLeft,
		enRight,
		enLeftAndRightNum,
	};
private:	//データメンバ
	COBB m_sideOBB[enLeftAndRightNum];

private:	//デバック用
	//後で消す
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };
	CModelRender* m_dbgSidePosMR[enLeftAndRightNum][m_vertNum];
};

