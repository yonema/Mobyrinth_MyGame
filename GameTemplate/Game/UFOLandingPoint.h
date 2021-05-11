#pragma once
#include "ObstacleObject.h"

/// <summary>
/// UFO�̒��n�_�N���X
/// </summary>
class CUFOLandingPoint : public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�
	~CUFOLandingPoint();			//�f�X�g���N�^

public:		//�����o�֐�

	/// <summary>
	/// �[��OBB�̎Q�Ƃ𓾂�
	/// </summary>
	/// <param name="leftOrRight">���[���E�[���H</param>
	/// <returns></returns>
	COBB& GetSideOBB(const int leftOrRight)
	{
		return m_sideOBB[leftOrRight];
	}

	/// <summary>
	/// �[��OBB�̃A�b�v�f�[�g
	/// </summary>
	void UpdateSideOBB();

private:
	//�����E��
	enum LeftOrRight
	{
		enLeft,				//��
		enRight,			//�E
		enLeftAndRightNum,	//���ƉE�̐�
	};

private:	//�f�[�^�����o
	COBB m_sideOBB[enLeftAndRightNum];		//�[��OBB


private:	//�f�o�b�N�p
#ifdef MY_DEBUG
	//��ŏ���
	static const int m_vertNum = 8;		//���_�̐�
	CModelRender* m_dbgVertPosMR[m_vertNum] = { nullptr };	//���_�����邽�߂̃��f��
	CModelRender* m_dbgSidePosMR[enLeftAndRightNum][m_vertNum];	//�[��OBB�����邽�߂̃��f��
#endif
};

