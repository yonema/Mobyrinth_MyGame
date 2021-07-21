#pragma once
#include "StartDirectingConstData.h"
#include "Player.h"
#include "Pause.h"
#include "UFO.h"
class CGameCamera;

/**
 * @brief �J�n���o�N���X
*/
class CStartDirecting : private Noncopyable
{
public:		//�����ŌĂ΂�郁���o�֐�
	CStartDirecting() {};				//�R���X�g���N�^
	~CStartDirecting() {};				//�f�X�g���N�^

private:	//private�ȃ����o�֐�

	/**
	 * @brief �X�e�[�W�̎�������邭���鏈��
	*/
	void GoAroundStage();

	/**
	 * @brief �t�F�[�h�̏���
	*/
	void FadeDirectingCamera();

	/**
	 * @brief �v���C���[�̉��o���̗�������
	*/
	void FallPlayer();

public:		//�����o�֐�

	/**
	 * @brief �J�n���o�̍X�V�B�g���Ƃ��ɃA�b�v�f�[�g�֐��ŌĂԁB
	*/
	void StartDirectingUpdate();

	/**
	 * @brief �X�e�[�W�̊J�n���o���s���Ă��邩�H�𓾂�
	 * @return �J�n���o���s���Ă��邩�H
	*/
	bool GetStartDirectingFlag()const
	{
		return m_startDirectingFlag;
	}

	/**
	 * @brief �X�e�[�W�̊J�n���o���s���Ă��邩�H��ݒ肷��
	 * @param [in] startDirecting �X�e�[�W�̊J�n���o���s���Ă��邩�H
	*/
	void SetStartDirectingFlag(const bool startDirecting)
	{
		m_startDirectingFlag = startDirecting;
	}

	/**
	 * @brief ���C�v���I��������H��ݒ�
	 * @param [in] wipeEndFlag ���C�v���I��������H
	*/
	void SetWipeEndFlag(const bool wipeEndFlag)
	{
		m_wipeEndFlag = wipeEndFlag;
	}

	/**
	 * @brief �Q�[���J�����̃A�N�Z�X���Z�b�g
	 * @param [in] gameCamera �Q�[���J�����̃A�h���X
	*/
	void SetGameCamera(CGameCamera* gameCamera)
	{
		m_gameCamera = gameCamera;
	}

private:	//�f�[�^�����o

	/*
	* �t���O�֘A
	*/
	bool m_startDirectingFlag = true;	//�J�n���o�����H�iGameCamera�N���X�Ŏg�p�j
	bool m_wipeEndFlag = false;			//���C�v���I������H
	bool m_goAroundStageFlag = true;	//�X�e�[�W�̎������邩�H
	bool m_fadeFlag = false;			//�t�F�[�h���邩�H
	bool m_fallPlayerFlag = false;		//�v���C���[�������Ă��鉉�o���s�����H

	/*
	* �^�C�}�[�֘A
	*/
	float m_firstWaitTimer = 0;			//���o�J�n�܂ł̃^�C�}�[
	float m_fadeTimer = 0.0f;			//�t�F�[�h���̃^�C�}�[
	float m_waitTimerAfterFell = 0.0f;	//�����ォ��J�n�܂ł̎���


	float m_goAroundStageAngle = 0.0f;	//�J�n���o���ɃX�e�[�W�̎��������Ă���p�x

	Player* m_player = nullptr;			//�v���C���[
	CGameCamera* m_gameCamera = nullptr;//�Q�[���J����
};

