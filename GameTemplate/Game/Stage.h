#pragma once
#include "StageBase.h"

/// <summary>
/// �X�e�[�W�N���X
/// �X�e�[�W�𐶐�����Ƃ��Ɏg���N���X
/// </summary>
class CStage : public IStageBase
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool StartSub() override final;	//�X�^�[�g�֐�
	~CStage();						//�f�X�g���N�^

	/// <summary>
	/// ���g���C�������̏���
	/// </summary>
	void RetryStage()override final
	{
		//�X�e�[�W�𐶐�������
		CStage* stage = NewGO<CStage>(0);
		//�X�C�b�`�̃^�C����ݒ肷��
		stage->SetSwitchTime(GetSwitchTime());
		//�������x���̃t�@�C���p�X�ŏ���������
		stage->Init(GetLevelFilePath(), GetHighestClearStageNum());
	}


public:		//�����o�֐�

	/// <summary>
	/// �������֐��ANewGO������ŕK�����łˁB
	/// </summary>
	/// <param name="levelFilePath">���x���̃t�@�C���p�X</param>
	void Init(const char* levelFilePath, const int stageNum);

	/// <summary>
	/// ���x���̃t�@�C���p�X�𓾂�
	/// </summary>
	/// <returns>���x���̃t�@�C���p�X</returns>
	const char* GetLevelFilePath() const
	{
		return m_levelFilePath;
	}

private:	//�f�[�^�����o
	char m_levelFilePath[256];		//���x���̃t�@�C���p�X
};
