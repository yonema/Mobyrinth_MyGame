#pragma once
#include "ObstacleObject.h"
#include "PointLight.h"
#include "effect/Effect.h"

/// <summary>
/// ��Q�I�u�W�F�N�g
/// ���I�u�W�F�N�g
/// </summary>
class OObigFire : public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;		//�X�^�[�g�֐�
	~OObigFire();						//�f�X�g���N�^
	void UpdateSub() override final;	//�A�b�v�f�[�g�֐�

public:		//�����o�֐�
	void Damage();	//�_���[�W���󂯂�
	void Burn(); //�R����SE

private:
	int m_maxHp = 3;						//�ő�HP
	int m_hp = m_maxHp;						//���݂�HP
	CPointLight* m_pointLight = nullptr;	//�|�C���g���C�g

	CSoundCue* m_flameSE = nullptr;			//flame�̃T�E���h�L���[
	Effect* m_flame_water = nullptr;		//���Ɛ��̔���
	CSoundCue* m_flame_waterSE = nullptr;	//���Ɛ��̔���

};

