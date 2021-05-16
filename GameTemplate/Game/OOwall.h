#pragma once
#include "ObstacleObject.h"
#include "SoundCue.h"
class ROrunning_stop;

/// <summary>
/// ��Q�I�u�W�F�N�g
/// �ǃI�u�W�F�N�g
/// </summary>
class OOwall :public CObstacleObject
{
public:		//�����ŌĂ΂�郁���o�֐�
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
	/// ���]�I�u�W�F�N�g�́u�ғ��A��~�I�u�W�F�N�g�v�𓾂�
	/// </summary>
	/// <returns></returns>
	ROrunning_stop* GetRun_stop()
	{
		return m_pRun_stop;
	}

	void MoveSE();

private:	//private�ȃ����o�֐�

	/// <summary>
	/// ���ڂ̃A�b�v�f�[�g�ł����Ă΂��֐�
	/// </summary>
	void FirstUpdate();

private:	//�f�[�^�����o
	bool m_moveFlag = false;				//�ғ���Ԃ��H
	float m_moveTimer = 0;					//�ғ����̃J�E���^�[
	Vector3 m_startPosition = g_vec3Zero;	//�ړ��O�̏����ʒu
	Vector3 m_endPosition = g_vec3Zero;		//�ړ���̏I�[�ʒu
	ROrunning_stop* m_pRun_stop = nullptr;	//�ғ��A��~�I�u�W�F�N�g�̃|�C���^
	bool m_firstUpdateFlag = true;			//���ڂ̃A�b�v�f�[�g���H
	CSoundCue* m_wallmoveSE = nullptr;	//wallmoveSE�̃T�E���h�L���[
};

