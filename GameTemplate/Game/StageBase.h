#pragma once

//��{�I�ɕK�v
#include "StageBaseConstData.h"
#include "GameCamera.h"
#include "SoundCue.h"
#include "Sky.h"

//�|�[�Y��ʗp
#include "Pause.h"

//Tips�p
#include "TipsController.h"

//�L���p�V�e�B��UI�p
#include "CapacityUI.h"

//�Z�[�u�f�[�^�p
#include "Save.h"



//���x���̃��[�h�ŕK�v
#include "Level.h"
#include "LevelObjectManager.h"

//��{�I�u�W�F�N�g
#include "Player.h"
#include "Mobius.h"

//�u���]�I�u�W�F�N�g�vReversibleObject
#include "ROwater_fire.h"
#include "ROrunning_stop.h"
#include "ROnail_hammer.h"
#include "ROleft_right.h"
#include "ROkey_padlock.h"

//�u��Q�I�u�W�F�N�g�vObstacleObject
#include "OOflame.h"
#include "OOwall.h"
#include "OOgoal.h"
#include "OOpadlock.h"
#include "OObox.h"
#include "OOTransparentSwitch.h"
#include "OOoneway.h"
#include "OOReverseALL.h"

//�uUFO�v
#include "UFO.h"

//�T�E���h
#include "SoundCue.h"


/// <summary>
///	�X�e�[�W�x�[�X�N���X
/// </summary>
class IStageBase : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;				//�X�^�[�g�֐�
	~IStageBase();							//�f�X�g���N�^
	void Update()override final;			//�A�b�v�f�[�g�֐�
	void UpdateOnlyPaused()override final;	//�|�[�Y���̂݌Ă΂��A�b�v�f�[�g�֐�

protected:	//�I�[�o�[���C�h���Ăق��������o�֐�

	/// <summary>
	/// �I�[�o�[���C�h���ق����X�^�[�g�֐�
	/// </summary>
	/// <returns></returns>
	virtual bool StartSub() = 0 { return true; };

	/// <summary>
	/// �I�[�o�[���C�h���Ăق����A
	/// ���g���C�������̏���
	/// </summary>
	virtual void RetryStage() = 0 {};

public: //�����o�֐�

	/**
	 * @brief �^�C�g�����[�h�ɂ���
	 * @attention Start�֐����Ă΂��O�ɌĂ΂Ȃ��ƈӖ����Ȃ��B
	*/
	void TitleMode()
	{
		m_isTitle = true;
	}


	/// <summary>
	/// ���C�v�C�����邩�ǂ����̃t���O��ݒ�
	/// </summary>
	/// <param name="wipeInFlag">���C�v�C������H</param>
	void SetWipeInFlag(const bool wipeInFlag)
	{
		m_wipeInFlag = wipeInFlag;
	}

	/// <summary>
	/// ��ԍ����N���A�����X�e�[�W�̔ԍ���߂�
	/// </summary>
	/// <returns> ��ԍ����N���A�����X�e�[�W</returns>
	const int GetHighestClearStageNum() const
	{
		return m_highestClearStageNum;
	}

	/// <summary>
	/// ��ԍ����N���A�����X�e�[�W�̔ԍ���ݒ肷��
	/// </summary>
	/// <returns> ��ԍ����N���A�����X�e�[�W</returns>
	void SetHighestClearStageNum(const int clearStageNum)
	{
		m_highestClearStageNum = clearStageNum;
	}

	/// <summary>
	/// �X�C�b�`�̃^�C����ݒ�
	/// </summary>
	/// <param name="switchTime"></param>
	void SetSwitchTime(const float switchTime)
	{
		m_switchTime = switchTime;
	}

	/// <summary>
	/// �X�C�b�`�̃^�C���̎擾
	/// </summary>
	/// <returns></returns>
	const float GetSwitchTime() const
	{
		return m_switchTime;
	}

protected:	//protected�ȃ����o�֐�
	/// <summary>
	/// ���x���̃��[�h
	/// </summary>
	/// <param name="filePath">tkl�̃t�@�C���p�X</param>
	void LoadLevel(const char* tklFilePath);

	/// <summary>
	/// �����[�X�֐�
	/// </summary>
	void Release()
	{
		DeleteGO(this);
	}

	/// <summary>
	/// �X�e�[�W�̔ԍ���ݒ肷��
	/// </summary>
	/// <param name="stageNum">�X�e�[�W�̔ԍ�</param>
	void SetStageNum(const int stageNum)
	{
		m_stageNum = stageNum;
	}

	/// <summary>
	/// �X�e�[�W�̔ԍ����擾����
	/// </summary>
	/// <returns>�X�e�[�W�̔ԍ�</returns>
	const int GetStageNum() const
	{
		return m_stageNum;
	}



private:	//private�ȃ����o�֐�

	/**
	 * @brief ���]�I�u�W�F�N�g�𐶐�����֐�
	 * @note CLevelObjectManager::GetInstance()->AllDeleteLOs()�Ń��\�[�X�͈�C�ɊJ���ł���
	 * @tparam C �������锽�]�I�u�W�F�N�g�̌^
	 * @param [in] objData ���[�h���ꂽ�I�u�W�F�N�g�̃f�[�^
	 * @param [in] frontOrBack �\�������H
	 * @param [in] Transparen �������H�f�t�H���g��false
	 * @param [in] objectName �I�u�W�F�N�g�̖��O�B�f�t�H���g�̓J���̖��O
	 * @param [in] priority �Q�[���I�u�W�F�N�g�̗D��x�B�f�t�H���g�� PRIORITY_FIRST
	 * @return �����������]�I�u�W�F�N�g�̎Q��
	*/
	template<class C>
	C* NewReversibleObject(
		const LevelObjectData& objData,
		const bool frontOrBack,
		const bool transparent = false,
		const char* const objectName = NAME_EMPTY,
		const int priority = PRIORITY_FIRST
	);

	/**
	 * @brief ��Q�I�u�W�F�N�g�𐶐�����֐�
	 * @note CLevelObjectManager::GetInstance()->AllDeleteLOs()�Ń��\�[�X�͈�C�ɊJ���ł���
	 * @tparam C ���������Q�I�u�W�F�N�g�̌^
	 * @param [in] objData ���[�h���ꂽ�I�u�W�F�N�g�̃f�[�^
	 * @param [in] Transparent �������H�f�t�H���g��false
	 * @param [in] objectName �I�u�W�F�N�g�̖��O�B�f�t�H���g�̓J���̖��O
	 * @param [in] priority �Q�[���I�u�W�F�N�g�̗D��x�B�f�t�H���g�� PRIORITY_FIRST
	 * @return ����������Q�I�u�W�F�N�g�̎Q��
	*/
	template<class C>
	C* NewObstacleObject(
		const LevelObjectData& objData,
		const bool transparent = false,
		const char* const objectName = NAME_EMPTY,
		const int priority = PRIORITY_FIRST
	);

	/// <summary>
	/// �N���A�������̏���
	/// </summary>
	void Clear();

	/// <summary>
	/// ���g���C�������̏���
	/// </summary>
	void Retry();

	/// <summary>
	///	�I���������̏���
	/// </summary>
	void Quit();

	/// <summary>
	/// �S�[���������̏���
	/// </summary>
	void Goal();

	/// <summary>
	/// �S�[�����Ă��邩�ǂ������ׂ�
	/// </summary>
	void CheckGoal();

	/// <summary>
	/// �^�C�g���֑J�ڂ���
	/// </summary>
	void GoTitle();

	/// <summary>
	/// ���C�v�C������
	/// </summary>
	void WipeIn();

	/// <summary>
	/// ���C�v�A�E�g����
	/// </summary>
	/// <returns>���C�v�A�E�g���I���������H</returns>
	bool WipeOut();

	void BGMInteractive();

	/// <summary>
	/// BGM�̏�����
	/// </summary>
	void InitBGM();

private:	//�f�[�^�����o

	/*
	* ���x�����[�h�֘A
	*/

	CLevel m_level;									//���x�����[�h�p�N���X
	CSky* m_sky = nullptr;							//��N���X�̃|�C���^
	CGameCamera* m_gameCamea = nullptr;				//�Q�[���J����
	Player* m_player = nullptr;						//�v���C���[�N���X�̃|�C���^
	CPause* m_pause = nullptr;						//�|�[�Y�N���X�̃|�C���^
	OOgoal* m_goal = nullptr;						//�S�[���N���X�̃|�C���^
	OOTransparentSwitch* m_switch = nullptr;		//�X�C�b�`�̃|�C���^
	CUFO* m_ufo = nullptr;							//UFO�̃|�C���^



	float m_goalTimer = 0;							//�S�[��������̑҂����Ԃ��͂���

	bool m_isTitle = false;							//�^�C�g����ʂ��H

	bool m_wipeInFlag = false;						//���C�v�C������H

	float m_switchTime = stageBaseConstData::SWITCH_TIME_DEFAULT;	//�X�C�b�`�̃^�C��

	/*
	* �L���p�V�e�B�֘A
	*/
	CTipsController* m_tipsController = nullptr;	//Tips�̃R���g���\���N���X
	CCapacityUI* m_capacityUI = nullptr;			//�L���p�V�e�B��UI�N���X


	/*
	* �Z�[�u�֘A
	*/
	CSave m_save;									//�Z�[�u�f�[�^�N���X
	int m_highestClearStageNum = -1;				//��ԍ����N���A�X�e�[�W
	int m_stageNum = -1;							//�X�e�[�W�̔ԍ�


	/*
	* �T�E���h�֘A
	*/
	CSoundCue* m_bgmStage1 = nullptr;				//BGM�̃T�E���h�L���[
	CSoundCue* m_bgmStage2 = nullptr;				//BGM�̃T�E���h�L���[
	CSoundCue* m_loop_bgmStage1 = nullptr;			//BGM�̃T�E���h�L���[
	CSoundCue* m_loop_bgmStage2 = nullptr;			//BGM�̃T�E���h�L���[
	bool m_check_loop1 = false;						//���[�v�m�F
	bool m_check_loop2 = false;						//���[�v�m�F
	bool m_initedBGM = false;						//BGM���������ς݂��H
};

