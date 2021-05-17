#pragma once
#include "ObstacleObject.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "GameCamera.h"
#include "UFO.h"
#include "SoundCue.h"
#include "effect/Effect.h"

class OOTransparentSwitch : public CObstacleObject
{
public:		//�I�[�o�[���C�h���Ăق��������o�֐�
	virtual bool StartSub();	//�X�^�[�g�֐�
	virtual void UpdateSub();	//�A�b�v�f�[�g�֐�

public:
	OOTransparentSwitch();		//�R���X�g���N�^
	~OOTransparentSwitch();		//�f�X�g���N�^

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

		//�^�C�}�[�̃t�H���g�̃C�e���[�^�[
		std::list<CFontRender*>::iterator itr = m_timerFR.begin();

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

			//�^�C�}�[�̃t�H���g�̃p�����[�^��ݒ肷��
			SetTimerFRParam(itr, levelObjects[i]);

			//�C�e���[�^�[��i�߂�
			itr++;

		}
	}

	/// <summary>
	/// �^�C�}�[�̃t�H���g��������̂��ݒ肷��
	/// </summary>
	/// <param name="num">����</param>
	void SetTimerFRNum(const int num);

private:	//private�ȃ����o�֐�

	/// <summary>
	/// �^�C�}�[�̃t�H���g�̍X�V
	/// </summary>
	void UpdateTimerFR();

	/// <summary>
	/// �^�C�}�[�̃t�H���g�̃p�����[�^�[��ݒ肷��
	/// </summary>
	/// <param name="itr">�^�C�}�[�̃t�H���g�̃C�e���[�^�[</param>
	/// <param name="levelObject">�Ή����铧���I�u�W�F�N�g</param>
	void SetTimerFRParam
	(std::list<CFontRender*>::iterator itr, const ILevelObjectBase* levelObject);

	void Switching();

private: //�����o�ϐ�
	float m_resetTimer = 0.0f; //�I�u�W�F�N�g�̏�Ԃ����Z�b�g����܂ł̃J�E���g�Ɏg�p���郊�Z�b�g�^�C�}�[
	float m_resetTimerStartValue = 10.0f; //���Z�b�g�^�C�}�[���쓮�����Ƃ��̒l��ۑ�����ϐ�
	bool m_flagSwitchOn = false; //�X�C�b�`��������āA�����I�u�W�F�N�g�������グ�����ԂɂȂ��Ă��邩�̃t���O
	std::list<CFontRender*> m_timerFR;			//�^�C�}�[�̃t�H���g
	Vector4 m_fontColor;						//�t�H���g�̃J���[
	Vector4 m_normalColor;						//�ʏ�̃t�H���g�̃J���[
	Vector4 m_blinkColor;						//�_�Ŏ��̃t�H���g�̃J���[
	float m_blinkTimer = FLT_MAX;				//�_�Ń^�C�}�[�A�ŏ��͍ő�l�����Ă���
	CModelRender* m_modelRender = nullptr;		//�{�^���������ꂽ�Ƃ��p�̃��f�������_���[
	GameCamera* m_gameCamera = nullptr;			//�J�����̃|�C���^
	CUFO* m_ufo = nullptr;						//UFO�̃|�C���^
	bool m_switchingFlag = false;				//�X�C�b�`�̐؂�ւ������H
	float m_switchingTimer = 0.0f;				//�X�C�b�`�̐؂�ւ����̃^�C�}�[
	CSpriteRender* m_fadeSR = nullptr;			//�t�F�[�h�C���A�A�E�g�Ŏg���X�v���C�g�����_���[
	CSoundCue* m_buttonpushSE = nullptr;		//m_buttonpushSE�̃T�E���h�L���[
	CSoundCue* m_activationSE = nullptr;		//m_activationSE�̃T�E���h�L���[
	CSoundCue* m_invalidationSE = nullptr;		//m_invalidationSE�̃T�E���h�L���[
	Effect* m_activation = nullptr;				//�I�u�W�F�N�g���L�����������̃G�t�F�N�g
};

