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

private:	//�f�[�^�����o
	OOwall* m_pWall = nullptr;		//�ǂ̃|�C���^
};

