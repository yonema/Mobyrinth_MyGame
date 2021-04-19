#pragma once
#include "ObstacleObject.h"
#include "PointLight.h"

/// <summary>
/// ��Q�I�u�W�F�N�g
/// ���I�u�W�F�N�g
/// </summary>
class OObigFire : public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�
	~OObigFire();					//�f�X�g���N�^

public:		//�����o�֐�
	void Damage();	//�_���[�W���󂯂�
private:
	int m_maxHp = 3;						//�ő�HP
	int m_hp = m_maxHp;						//���݂�HP
	CPointLight* m_pointLight = nullptr;	//�|�C���g���C�g
};

