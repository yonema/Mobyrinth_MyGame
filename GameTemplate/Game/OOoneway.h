#pragma once
#include "ObstacleObject.h"

/// <summary>
/// ��Q�I�u�W�F�N�g
/// ����ʍs�I�u�W�F�N�g
/// </summary>
class OOoneway : public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;		//�X�^�[�g�֐�
	~OOoneway();						//�f�X�g���N�^
	void UpdateSub() override final;	//�A�b�v�f�[�g�֐�
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

private:	//�f�o�b�N�p
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertMR[enLeftAndRightNum][m_vertNum];
};

