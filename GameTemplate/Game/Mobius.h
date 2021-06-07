#pragma once
#include "ModelRender.h"

/// <summary>
/// ���r�E�X�̗փN���X
/// </summary>
class CMobius : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;		//�X�^�[�g�֐�
	~CMobius();						//�f�X�g���N�^

public:		//�����̃����o�֐�����Ɏg���B

	/// <summary>
	/// �ꏊ�̐ݒ�
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// ���f���̎Q�Ƃ��擾����
	/// </summary>
	/// <returns>���f���̎Q��</returns>
	Model* GetModel()
	{
		return &m_modelRender->GetModel();
	}

private:	//�f�[�^�����o
	CModelRender* m_modelRender = nullptr;	//���f�������_���[
	Vector3 m_position = g_vec3Zero;		//�ꏊ
	Quaternion m_rotation = g_quatIdentity;	//��]
};

