#pragma once
#include "ReversibleObject.h"
#include "OOpadlock.h"

/// <summary>
/// ���]�I�u�W�F�N�g
/// ���A�싞���I�u�W�F�N�g
/// </summary>
class ROkey_padlock : public CReversibleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�

	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
	/// </summary>
	void QuerySub()override final;

	CSoundCue* m_padlockbreakSE = nullptr; //padlockbreakSE�̃T�E���h�L���[
};

