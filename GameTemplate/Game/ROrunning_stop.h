#pragma once
#include "ReversibleObject.h"
#include "OOwall.h"

/// <summary>
/// ���]�I�u�W�F�N�g
/// �ғ��A��~�I�u�W�F�N�g
/// </summary>
class ROrunning_stop : public CReversibleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�

	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
	/// </summary>
	void QuerySub()override final;

	void UpdateSub()override final;	//�A�b�v�f�[�g�֐�

public:

	/// <summary>
	/// �ǂ̃|�C���^��ݒ肷��
	/// </summary>
	/// <param name="wall">�ǂ̃|�C���^</param>
	void SetWall(const OOwall* wall)
	{
		m_pWall = wall;
	}

	/**
	 * @brief �u�ǁv�ƏՓ˂������̏���
	 * @param wall �u�ǁv�̃|�C���^
	*/
	void HitWall(OOwall* wall);

private:	//�f�[�^�����o
	const OOwall* m_pWall = nullptr;		//�ǂ̃|�C���^
	bool m_actionFlag = true;				//�s���ł��邩�H
};

