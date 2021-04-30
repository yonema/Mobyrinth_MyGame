#pragma once
#include "ObstacleObject.h"

class OOTransparentSwitch : public CObstacleObject
{
public:		//�I�[�o�[���C�h���Ăق��������o�֐�
	virtual bool StartSub();	//�X�^�[�g�֐�
	virtual void UpdateSub();	//�A�b�v�f�[�g�֐�

public: //Set�֐�
	/// <summary>
	/// ���Z�b�g�^�C�}�[���쓮�����Ƃ��̒l��ύX
	/// </summary>
	/// <param name="i">���Z�b�g�^�C�}�[���쓮�����Ƃ��̒l</param>
	void SetResetTimerStartValue(float f)
	{
		m_resetTimerStartValue = f;
	}

private: //�����o�ϐ�
	float m_resetTimer = 0.0f; //�I�u�W�F�N�g�̏�Ԃ����Z�b�g����܂ł̃J�E���g�Ɏg�p���郊�Z�b�g�^�C�}�[
	float m_resetTimerStartValue = 10.0f; //���Z�b�g�^�C�}�[���쓮�����Ƃ��̒l��ۑ�����ϐ�
	bool m_flagSwitchOn = false; //�X�C�b�`��������āA�����I�u�W�F�N�g�������グ�����ԂɂȂ��Ă��邩�̃t���O
};

