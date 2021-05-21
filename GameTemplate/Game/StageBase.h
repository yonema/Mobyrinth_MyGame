#pragma once

//��{�I�ɕK�v
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
#include "ROmizu_kori.h"
#include "RObird_fish.h"
#include "ROrunning_stop.h"
#include "ROwire_string.h"
#include "ROnail_bar.h"
#include "ROaxe_pickaxe.h"
#include "ROkeymold_empty.h"
#include "ROleft_right.h"

//�u��Q�I�u�W�F�N�g�vObstacleObject
#include "OObigFire.h"
#include "OOwall.h"
#include "OOgoal.h"
#include "OOpadlock.h"
#include "OObox.h"
#include "TransparentSwitch.h"
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

private:	//private�ȃ����o�֐�

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

public: //�C�����C���֐�
	void SetStartUpStartDirecting(const bool b)
	{
		m_startUpStartDirecting = b;
	}
	void SetTitlePlayer(const bool b)
	{
		m_titlePlayer = b;
	}
	void SetStartBGM(const bool b)
	{
		m_startBGM = b;
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


private:	//�f�[�^�����o
	CLevel m_level;									//���x�����[�h�p�N���X
	Player* m_player = nullptr;						//�v���C���[�N���X�̃|�C���^
	CPause* m_pause = nullptr;						//�|�[�Y�N���X�̃|�C���^
	OOgoal* m_goal = nullptr;						//�S�[���N���X�̃|�C���^
	CSky* m_sky = nullptr;							//��N���X�̃|�C���^
	float m_goalTimer = 0;							//�S�[��������̑҂����Ԃ��͂���

	GameCamera* m_gameCamea = nullptr;				//�Q�[���J����

	StartDirecting* m_startDirecting = nullptr;
	bool m_startUpStartDirecting = true;
	bool m_titlePlayer = false;	

	CSoundCue* m_bgmStage1 = nullptr;				//BGM�̃T�E���h�L���[
	CSoundCue* m_bgmStage2 = nullptr;				//BGM�̃T�E���h�L���[
	CSoundCue* m_loop_bgmStage1 = nullptr;			//BGM�̃T�E���h�L���[
	CSoundCue* m_loop_bgmStage2 = nullptr;			//BGM�̃T�E���h�L���[
	bool m_check_loop1 = false;						//���[�v�m�F
	bool m_check_loop2 = false;						//���[�v�m�F
	bool m_startBGM = true;							//�^�C�g����ʂ�BGM�Đ��̃t���O
	bool m_initedBGM = false;						//BGM���������ς݂��H
	bool m_wipeInFlag = false;						//���C�v�C������H

	CTipsController* m_tipsController = nullptr;	//Tips�̃R���g���\���N���X
	CCapacityUI* m_capacityUI = nullptr;			//�L���p�V�e�B��UI�N���X
	CSoundCue* m_goalSE = nullptr;					//m_goalSE�̃T�E���h�L���[
	int m_goalSEcount = 0;							//m_goalSE�̃T�|�[�g

	CSave m_save;									//�Z�[�u�f�[�^�N���X
	int m_highestClearStageNum = -1;				//��ԍ����N���A�X�e�[�W
	int m_stageNum = -1;							//�X�e�[�W�̔ԍ�
	
	CSpriteRender* m_goalSR = nullptr;				//�S�[���̃X�v���C�g

};

