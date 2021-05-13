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

	/// <summary>
	/// �I�u�W�F�N�g�𔼓����ɂ���
	/// </summary>
	void ChangeTransparent()
	{
		//���x���I�u�W�F�N�g������Ă���
		std::vector<ILevelObjectBase*> levelObjects
			= CLevelObjectManager::GetInstance()->GetLevelObjects();
		//�S�Ẵ��x���I�u�W�F�N�g�Ɍ���
		for (int i = 0; i < levelObjects.size(); i++)
		{
			//�����I�u�W�F�N�g�ł͂Ȃ��ꍇ�A���̃I�u�W�F�N�g������
			if (levelObjects[i]->GetFlagTransparentObject() == false) {
				continue;
			}

			//���f���̎Q�Ƃ𓾂Ă���ASetMulColor()���Ă�
			//Obstacle�̏ꍇ�͖��ʂɓ��Ă΂�邯�ǁA�䖝���悤�B
			levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
			levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });

			levelObjects[i]->TransparentSwitchOff();
			//�I�u�W�F�N�g�������Ă���ꍇ
			levelObjects[i]->SetFlagHeldPlayer(false);
		}
	}

	/// <summary>
	/// �I�u�W�F�N�g�����̂ɂ���
	/// </summary>
	void ChangeEntity()
	{
		//���x���I�u�W�F�N�g������Ă���
		std::vector<ILevelObjectBase*> levelObjects
			= CLevelObjectManager::GetInstance()->GetLevelObjects();
		//�S�Ẵ��x���I�u�W�F�N�g�Ɍ���
		for (int i = 0; i < levelObjects.size(); i++)
		{
			//�����I�u�W�F�N�g�ł͂Ȃ��ꍇ�A���̃I�u�W�F�N�g������
			if (levelObjects[i]->GetFlagTransparentObject() == false) {
				continue;
			}

			//���f���̎Q�Ƃ𓾂Ă���ASetMulColor()���Ă�
			//Obstacle�̏ꍇ�͖��ʂɓ��Ă΂�邯�ǁA�䖝���悤�B
			levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
			levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });

			levelObjects[i]->TransparentSwitchOn();
		}
	}

private: //�����o�ϐ�
	float m_resetTimer = 0.0f; //�I�u�W�F�N�g�̏�Ԃ����Z�b�g����܂ł̃J�E���g�Ɏg�p���郊�Z�b�g�^�C�}�[
	float m_resetTimerStartValue = 10.0f; //���Z�b�g�^�C�}�[���쓮�����Ƃ��̒l��ۑ�����ϐ�
	bool m_flagSwitchOn = false; //�X�C�b�`��������āA�����I�u�W�F�N�g�������グ�����ԂɂȂ��Ă��邩�̃t���O



};

