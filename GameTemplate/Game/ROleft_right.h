#pragma once
#include "ReversibleObject.h"
#include "OOoneway.h"

/// <summary>
/// ���]�I�u�W�F�N�g
/// ���A�E�I�u�W�F�N�g
/// </summary>
class ROleft_right : public CReversibleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�

	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
	/// </summary>
	void QuerySub()override final;

private:	//private�ȃ����o�֐�
	
	/// <summary>
	/// ���������I�u�W�F�N�g�����E���]������
	/// </summary>
	void ObjectReverse();

private:	//�f�[�^�����o
	//��x�ɕ����̃I�u�W�F�N�g��j�����邱�Ƃ�h�����߂̃t���O
	bool m_actionFlag = true;	//�s���ł��邩�H
};

