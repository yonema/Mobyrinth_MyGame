#pragma once
#include "ReversibleObject.h"

/// <summary>
/// ���]�I�u�W�F�N�g
/// ���̋��^�A����ۃI�u�W�F�N�g
/// </summary>
class ROkeymold_empty : public CReversibleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�

	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
	/// </summary>
	void QuerySub()override final;
};

