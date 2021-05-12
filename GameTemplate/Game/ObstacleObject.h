#pragma once
#include "LevelObjectBase.h"
#include "ModelRender.h"
#include "OBBWorld.h"


/// <summary>
/// �I�u�X�e�[�N���I�u�W�F�N�g�i��Q�I�u�W�F�N�g�j�i�ǂݕ������Ă�H�j
/// ��Q�I�u�W�F�N�g�����Ƃ��͂��̃N���X���p�����Ă�
/// </summary>
class CObstacleObject : public ILevelObjectBase
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool PureVirtualStart()override final;		//�X�^�[�g�֐�
	virtual ~CObstacleObject();					//�f�X�g���N�^
	void PureVirtualUpdate()override final;		//�A�b�v�f�[�g�֐�

public:		//�I�[�o�[���C�h���Ăق��������o�֐�
	virtual bool StartSub() { return true; };	//�X�^�[�g�֐�
	virtual void UpdateSub() {};				//�A�b�v�f�[�g�֐�

protected:	//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// �������֐�
	/// �ŏ��ɌĂ�łˁBtrue��߂��ĂˁB
	/// ���f���̃t�@�C���p�X�ƃI�u�W�F�N�g�̃^�C�v��
	/// �ݒ肷��
	/// �^�C�v�ꗗ��ObjectType.h���Q��
	/// </summary>
	/// <param name="filePath">���f����tkm�t�@�C���p�X</param>
	/// <param name="objectType">�^�C�v</param>
	/// <returns>true��߂��Ă�</returns>
	bool Init(const char* filePath, int objectType);

	/// <summary>
	/// ���Ȕ����F��ݒ肷��
	/// </summary>
	/// <param name="color">���Ȕ����F</param>
	void SetEmissionColor(const Vector4& color)
	{
		m_modelRender->SetEmissionColor(color);
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���
	/// </summary>
	void Activate()
	{
		m_modelRender->Activate();
	}

	/// <summary>
	/// �Q�[���I�u�W�F�N�g���A�N�e�B�u�ɂ���
	/// </summary>
	void Deactivate()
	{
		m_modelRender->Deactivate();
	}

private:	//�f�[�^�����o
	CModelRender* m_modelRender = nullptr;	//���f�������_���[
};

