#pragma once
#include "ReversibleObject.h"
#include "ROkeymold_empty.h"
#include "ROkey_padlock.h"

/// <summary>
/// ���]�I�u�W�F�N�g
/// �R�A�j���I�u�W�F�N�g
/// </summary>
class ROwire_string : public CReversibleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�

	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
	/// </summary>
	void QuerySub()override final;
public:		//�����o�֐�

	/// <summary>
	/// �u���v�𐶐����Ď��g�̃I�u�W�F�N�g��j������
	/// </summary>
	void ChangeToKey();
};

