#pragma once
#include "ObstacleObject.h"
#include "PointLight.h"
#include "effect/Effect.h"

//�u���v�I�u�W�F�N�g�̒萔�f�[�^
namespace flameConstData
{
	//�ő�HP
	constexpr int HP_MAX = 3;

	/*
	* �|�C���g���C�g�֘A�̒萔
	*/

	//�|�C���g���C�g�̏�֏グ��̍��W�̒���
	constexpr float LENGTH_UP_POSITION_POINT_LIGHT = 300.0f;
	//�|�C���g���C�g�̃J���[
	const Vector4 COLOR_POINT_LIGHT = { 500.0f,1.0f,1.0f,1.0f };
	//�|�C���g���C�g�̉e���͈�
	constexpr float RANGE_POINT_LIGHT = 500.0f;


	/*
	* �T�E���h�֘A�̒萔
	*/

	//���̏펞���Ă���T�E���h�̃t�@�C���p�X
	constexpr const wchar_t* const SOUND_FILEPATH_FLAME = L"Assets/sound/flame.wav";
	//���̏펞���Ă���T�E���h�̉���
	constexpr float SOUND_VOLUME_FLAME = 0.8f;

	//���Ɛ��̔����T�E���h�̃t�@�C���p�X
	constexpr const wchar_t* const SOUND_FILEPATH_FLAME_WATAER = L"Assets/sound/flame_water.wav";
	//���Ɛ��̔����T�E���h�̉���
	constexpr float SOUND_VOLUME_FLAME_WATER = 2.0f;


	/*
	* �G�t�F�N�g�֘A�̒萔
	*/

	//���Ɛ��̔����G�t�F�N�g�̃t�@�C���p�X
	constexpr const char16_t* const EFFECT_FILEPATH_FLAME = u"Assets/effect/flame_water.efk";
	//���Ɛ��̔����G�t�F�N�g�̊g�嗦
	const Vector3 EFFECT_SCALE_FLAME = { 200.0f,200.0f,200.0f };
	//���Ɛ��̔����G�t�F�N�g�̏�֏グ����W�̒���
	constexpr float LENGTH_UP_POSITION_EFFECT = 100.0f;


	//���̑傫���␳
	//�傫����1/3�ɂ���ƁA���������ăS�[���ł����Ⴄ����A
	//�傫����5/5��4/5��3/5��0�ƕω�������
	constexpr int SIZE_COMPLEMENT_FLAME = 3;

	//���̏펞���Ă���T�E���h����������ő勗��
	constexpr float DISTANCE_MAX_FLAME_SOUND = 1500.0f;
}



/// <summary>
/// ��Q�I�u�W�F�N�g
/// ���I�u�W�F�N�g
/// </summary>
class OOflame : public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;		//�X�^�[�g�֐�
	~OOflame();						//�f�X�g���N�^
	void UpdateSub() override final;	//�A�b�v�f�[�g�֐�

public:		//�����o�֐�
	void Damage();	//�_���[�W���󂯂�
	void Burn(); //�R����SE

private:	//private�ȃ����o�֐�

	/**
	 * @brief �|�C���g���C�g�̏���������
	*/
	void InitPointLight();

	/**
	 * @brief �T�E���h�̏���������
	*/
	void InitSound();

	/**
	 * @brief �G�t�F�N�g�̏���������
	*/
	void InitEffect();

private:	//�f�[�^�����o
	int m_hp = flameConstData::HP_MAX;		//���݂�HP
	CPointLight* m_pointLight = nullptr;	//�|�C���g���C�g
	CSoundCue* m_flameSE = nullptr;			//���̏펞���Ă���T�E���h
	Effect* m_flame_waterEF = nullptr;		//���Ɛ��̔����G�t�F�N�g
	CSoundCue* m_flame_waterSE = nullptr;	//���Ɛ��̔����T�E���h

};

