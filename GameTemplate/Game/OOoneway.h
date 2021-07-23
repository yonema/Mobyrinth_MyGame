#pragma once
#include "ObstacleObject.h"

/**
 * @brief �u����ʍs�v�̒萔�f�[�^
*/
namespace onewayConstData
{
	//�T�C�h��OBB�ւ̒���
	constexpr float LENGHT_TO_SIDE_OBB = 300.0f;
}

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
		for (int i = 0; i < EN_LEFT_AND_RIGHT_NUM; i++)
		{
			m_sideOBB[i].SetExceptionFlag(true);
		}

		if (m_leftOrRight == EN_LEFT)
		{
			if (m_rightCanPassMR)
				m_rightCanPassMR->Deactivate();
			if (GetModelRender())
				GetModelRender()->Activate();
		}
		else 
		{
			if (m_rightCanPassMR)
				m_rightCanPassMR->Activate();
			if (GetModelRender())
				GetModelRender()->Deactivate();
		}
	}

private:	//private�ȃ����o�֐�

	/**
	 * @brief ���f���̏���������
	*/
	void InitModel();

	/**
	 * @brief �E���������������ɍ��킹�ď���������
	*/
	void InitLeftOrRight();

	/**
	 * @brief OBB�̏���������
	*/
	void InitOBB();

	/**
	 * @brief �ʂ�郂�f���̗L��������
	*/
	void CanPassModelActivate();

	/**
	 * @brief �ʂ�Ȃ����f����L��������
	*/
	void CannotPassModelActivate();

	/**
	 * @brief ���f���̃g�����X�t�H�[���̍X�V
	*/
	void ModelTransformUpdate();

private:	//�f�[�^�����o

	COBB m_sideOBB[EN_LEFT_AND_RIGHT_NUM];		//���T�C�h��OBB
	int m_leftOrRight = EN_LEFT;				//�����E���H
	CModelRender* m_canPassMR = nullptr;		//�������̒ʂ�鎞�̃��f��
	CModelRender* m_rightMR = nullptr;			//�E�����̃��f��
	CModelRender* m_rightCanPassMR = nullptr;	//�E�����̒ʂ�鎞�̃��f��



private:	//�f�o�b�N�p
#ifdef MY_DEBUG
	static const int m_vertNum = 8;
	CModelRender* m_dbgVertMR[enLeftAndRightNum][m_vertNum];
#endif
};

