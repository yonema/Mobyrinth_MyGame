#pragma once
#include "ReversibleObject.h"

class CTransparentObject : public CReversibleObject
{
public:		//public�ȃI�[�o�[���C�h���Ăق��������o�֐�
	virtual bool StartSub() //�X�^�[�g�֐�
	{
		//�I�u�W�F�N�g�������グ���Ȃ��悤�ɂ���B
		SetFlagHeld(false);
		return true;
	};
	void UpdateSub()override final;		//�A�b�v�f�[�g�֐�

private:	//private�ȃI�[�o�[���C�h���Ăق��������o�֐�
	/// <summary>
	/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐��B
	/// ������QueryLOs���Ă�ŃI�u�W�F�N�g���m�̔����������B
	/// ���ROnail_bar���Q��
	/// </summary>
	virtual void QuerySub() {};

public: //Set�֐�
	/// <summary>
	/// �I�u�W�F�N�g�������ʒu�ɖ߂��B
	/// </summary>
	void ResetObject()
	{
		m_position = m_startPosition;
		m_rotation = m_startRotation;
		//�\���̏�Ԃ�������Ԃɖ߂��B

	}

	/// <summary>
	/// ���Z�b�g�^�C�}�[�̎��Ԃ��Z�b�g���A�^�C�}�[���X�^�[�g����B
	/// </summary>
	/// <param name="i">���Z�b�g�^�C�}�[�̐ݒ�l</param>
	//void SetANDStartResetTimer(int i)
	//{
	//	m_resetTimer = i;
	//	//�I�u�W�F�N�g�������グ����悤�ɂ���B
	//	SetFlagHeld(true);
	//}

protected: //�����o�ϐ�
	//bool m_flagTransparentObject = true; //��莞�Ԃ��������グ����I�u�W�F�N�g���ǂ����̃t���O
	int m_resetTimer = 0; //�I�u�W�F�N�g�̏�Ԃ����Z�b�g����܂ł̃J�E���g�Ɏg�p���郊�Z�b�g�^�C�}�[
	Vector3 m_startPosition = { 0.0f,0.0f,0.0f }; //�I�u�W�F�N�g�̏����ʒu��ۑ�����ʒu���ϐ�
	Quaternion m_startRotation = g_quatIdentity;


};

