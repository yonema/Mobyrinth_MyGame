#pragma once
#include "ObstacleObject.h"
#include "SoundCue.h"
class ROrunning_stop;

/**
 * @brief �u�ǁv�̒萔�f�[�^
*/
namespace wallConstData
{
	//�X�e�[�W����ǂꂭ�炢��̈ʒu�ɂ��邩
	constexpr float LENGHT_POSITION_FROM_STAGE = 50.0f;

	//�ǂ��ړ�����Г����̎���
	constexpr float TIME_MOVE = 3.0f;
	//�ǂ��ړ�����Г����̒���
	constexpr float LENGTH_MOVE = 200.0f;

	//�ǂ������Ƃ��̃T�E���h�̃t�@�C���p�X
	constexpr const wchar_t* const SOUND_FILEPATH_WALL_MOVE = L"Assets/sound/wallmove.wav";
	//�ǂ������Ƃ��̃T�E���h�̉���
	constexpr float SOUND_VALUME_WALL_MOVE = 1.0f;

	//�ǂ������Ƃ��̃T�E���h����������ő勗��
	constexpr float DISTANCE_MAX_WALL_MOVE_SOUND = 1500.0f;
}

/// <summary>
/// ��Q�I�u�W�F�N�g
/// �ǃI�u�W�F�N�g
/// </summary>
class OOwall :public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	OOwall();						//�R���X�g���N�^
	bool StartSub() override final;	//�X�^�[�g�֐�
	~OOwall();						//�f�X�g���N�^
	void UpdateSub()override final;	//�A�b�v�f�[�g�֐�

public:		//�����o�֐�

	/// <summary>
	/// �ғ����[�h
	/// </summary>
	void Running()
	{
		m_moveFlag = true;
	}

	/// <summary>
	/// ��~���[�h
	/// </summary>
	void Stop()
	{
		m_moveFlag = false;
	}

	/// <summary>
	/// �����Ă���
	/// ���]�I�u�W�F�N�g�́u�ғ��A��~�I�u�W�F�N�g�v��ݒ肷��
	/// </summary>
	/// <param name="run_stop">�ғ��A��~�I�u�W�F�N�g</param>
	void SetRun_stop(ROrunning_stop* run_stop)
	{
		m_pRun_stop = run_stop;
	}

	/// <summary>
	/// �����Ă���
	/// ���]�I�u�W�F�N�g�́u�ғ��A��~�I�u�W�F�N�g�v�̃|�C���^�𓾂�
	/// </summary>
	/// <returns></returns>
	ROrunning_stop* GetRun_stop()
	{
		return m_pRun_stop;
	}

private:	//private�ȃ����o�֐�

	/**
	 * @brief �ړ��O�̏����ʒu�ƈړ���̏I�[�ʒu�̏���������
	*/
	void InitStartAndEndPos();

	/**
	 * @brief �ғ��A��~�I�u�W�F�N�g�̏���������
	*/
	void InitRun_stopObject();

	/**
	 * @brief �T�E���h�̏���������
	*/
	void InitSound();

	/// <summary>
	/// ���ڂ̃A�b�v�f�[�g�ł����Ă΂��֐�
	/// </summary>
	void FirstUpdate();

	/**
	 * @brief ���W�̍X�V����
	*/
	void PositionUpdate();

	/**
	 * @brief �T�E���h�̍X�V����
	*/
	void SEUpdate();

private:	//�f�[�^�����o
	bool m_moveFlag = false;				//�ғ���Ԃ��H
	float m_moveTimer = 0.0f;				//�ғ����̃J�E���^�[
	Vector3 m_startPosition = g_VEC3_ZERO;	//�ړ��O�̏����ʒu
	Vector3 m_endPosition = g_VEC3_ZERO;	//�ړ���̏I�[�ʒu
	ROrunning_stop* m_pRun_stop = nullptr;	//�ғ��A��~�I�u�W�F�N�g�̃|�C���^
	bool m_firstUpdateFlag = true;			//���ڂ̃A�b�v�f�[�g���H
	CSoundCue* m_wallmoveSE = nullptr;		//�ǂ������Ƃ��̃T�E���h
};

