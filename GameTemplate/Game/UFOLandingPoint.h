#pragma once
#include "ObstacleObject.h"

class CUFOLandingPoint : public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�

private:	//�f�o�b�N�p
	//��ŏ���
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };
};

