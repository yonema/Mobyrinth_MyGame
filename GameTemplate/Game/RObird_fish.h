#pragma once
#include "ReversibleObject.h"

/// <summary>
/// ���]�I�u�W�F�N�g�B
/// ���A�X�I�u�W�F�N�g
/// </summary>
class RObird_fish : public CReversibleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;				//�X�^�[�g�֐�
	~RObird_fish();								//�f�X�g���N�^
	void QuerySub()override final;				//�N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
	void UpdateSub()override final;				//�A�b�v�f�[�g�֐�
	void SetFrontOrBackSub()override final;		//�\�Ɨ����Z�b�g����T�u�֐�

private:	//�f�[�^�����o

	/// <summary>
	/// ������̗��\�̃I�u�W�F�N�g�̃��f�������_���[
	/// </summary>
	CModelRender* m_otherModelRender[enFrontAndBackNum] = { nullptr };
	int m_reversibleType[enFrontAndBackNum] = { enEnpty };				//�I�u�W�F�N�g�̃^�C�v
};

