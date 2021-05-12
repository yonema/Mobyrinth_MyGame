#pragma once
#include "Tips.h"
#include "LevelObjectManager.h"

/// <summary>
/// Tips�̃R���g���[���[�N���X
/// </summary>
class CTipsController : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start() override final;	//�X�^�[�g�֐�
	void Update() override final;	//�A�b�v�f�[�g�֐�

public:		//�����o�֐�
	void SetTitleMode()
	{
		m_tips.SetTitleMode();
		this->Deactivate();
	}

private:
	CTips m_tips;		//Tips�\���N���X
};

