#pragma once
#include "ReversibleObject.h"
#include "OObox.h"

/// <summary>
/// ���]�I�u�W�F�N�g�B
/// ���A��͂��I�u�W�F�N�g
/// </summary>
class ROaxe_pickaxe : public CReversibleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�

	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
	/// </summary>
	void QuerySub()override final;

private:	//�f�[�^�����o

};

