#pragma once
#include "ObstacleObject.h"

/// <summary>
/// 障害オブジェクト
/// 一方通行オブジェクト
/// </summary>
class OOoneway : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub() override final;		//スタート関数
	~OOoneway();						//デストラクタ
	void UpdateSub() override final;	//アップデート関数
	void SetLeftOrRight(const int leftOrRight)
	{
		m_leftOrRight = leftOrRight;
		for (int i = 0; i < enLeftAndRightNum; i++)
		{
			m_sideOBB[i].SetExceptionFlag(true);
		}
	}

public:
	enum EnLeftOrRight
	{
		enLeft,
		enRight,
		enLeftAndRightNum,
	};
private:
	COBB m_sideOBB[enLeftAndRightNum];
	int m_leftOrRight = enLeft;

private:	//デバック用
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertMR[enLeftAndRightNum][m_vertNum];
};

