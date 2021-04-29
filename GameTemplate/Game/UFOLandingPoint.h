#pragma once
#include "ObstacleObject.h"

class CUFOLandingPoint : public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�

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
private:	//�f�[�^�����o
	COBB m_sideOBB[enLeftAndRightNum];

private:	//�f�o�b�N�p
	//��ŏ���
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };
	CModelRender* m_dbgSidePosMR[enLeftAndRightNum][m_vertNum];
};

