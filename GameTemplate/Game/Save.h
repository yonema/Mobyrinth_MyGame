#pragma once


/// <summary>
/// �Z�[�u�f�[�^�̍\����
/// </summary>
struct SSaveData
{
	int highestClearStageNum;	//��ԍ����N���A�����X�e�[�W�̔ԍ�
};



/// <summary>
/// �Z�[�u�N���X
/// �R���X�g���N�^�Ń��[�h������
/// </summary>
class CSave
{
public:		//�����ŌĂ΂�郁���o�֐�
	CSave();	//�R���X�g���N�^

public:		//�����o�֐�

	/// <summary>
	/// �f�[�^�����o�̃Z�[�u�f�[�^���Z�[�u����
	/// </summary>
	void SaveData();

	/// <summary>
	/// �Z�[�u�f�[�^���f�[�^�����o�Ƀ��[�h����
	/// </summary>
	/// <returns>���[�h�ł������H</returns>
	const bool LoadData();

	/// <summary>
	/// �f�[�^�����o�ɓ����Ă���Z�[�u�f�[�^�𓾂�
	/// </summary>
	/// <returns>�Z�[�u�f�[�^</returns>
	const SSaveData& GetSaveData()const
	{
		return m_saveData;
	}

	/// <summary>
	/// �Z�[�u�f�[�^���f�[�^�����o�ɃZ�b�g����
	/// </summary>
	/// <param name="saveData">�Z�[�u�f�[�^��</param>
	void SetSaveData(const SSaveData& saveData)
	{
		m_saveData = saveData;
	}
	
private:	//�f�[�^�����o

	SSaveData m_saveData;	//�Z�[�u�f�[�^
};

