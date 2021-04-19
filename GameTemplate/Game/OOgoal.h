#pragma once
#include "ObstacleObject.h"
#include "FontRender.h"

/// <summary>
/// ��Q�I�u�W�F�N�g
/// �S�[���I�u�W�F�N�g
/// </summary>
class OOgoal : public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub()override final;	//�X�^�[�g�֐�
	~OOgoal();						//�f�X�g���N�^
	void UpdateSub()override final;	//�A�b�v�f�[�g�֐�

public:		//�����o�֐�

	/// <summary>
	/// �S�[�����Ă��邩�ǂ����𓾂�
	/// </summary>
	/// <returns>�S�[�����Ă��邩�H</returns>
	const bool GetIsGoal()const
	{
		return m_isGoal;
	}

private:	//�f�[�^�����o
	CFontRender* m_fontRender = nullptr;	//�S�[���������̃X�v���C�g
	bool m_isGoal = false;					//�S�[������
};

