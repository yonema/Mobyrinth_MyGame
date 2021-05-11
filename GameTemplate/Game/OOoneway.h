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

public:		//�����o�֐�
	
	/// <summary>
	/// �����E���H��ݒ肷��B
	/// </summary>
	/// <param name="leftOrRight">�����E���H</param>
	void SetLeftOrRight(const int leftOrRight)
	{
		//�����E���H��V�����ݒ�
		m_leftOrRight = leftOrRight;

		//���[��OBB����x��O�ɂ��Ă����B
		for (int i = 0; i < enLeftAndRightNum; i++)
		{
			m_sideOBB[i].SetExceptionFlag(true);
		}
	}

public:		//public�ȗ񋓌^

	/// <summary>
	/// �����E���H
	/// </summary>
	enum EnLeftOrRight
	{
		enLeft,				//��
		enRight,			//�E
		enLeftAndRightNum,	//���ƉE�̐�
	};

private:	//�f�[�^�����o

	COBB m_sideOBB[enLeftAndRightNum];	//���T�C�h��OBB
	int m_leftOrRight = enLeft;			//�����E���H
	CModelRender* m_canPassMR = nullptr;	//�ʂ�鎞�̃��f��

private:	//�f�o�b�N�p
#ifdef MY_DEBUG
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertMR[enLeftAndRightNum][m_vertNum];
#endif
};

