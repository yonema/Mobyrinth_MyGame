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

	/**
	 * @brief ���r�E�X�̗ւƃ��C�̌����_�����߂�
	 * @param [in] startPos ���C�̎n�_
	 * @param [in] endPos ���C�̏I�_
	 * @param [out] IntersectPos �����_
	 * @return �������Ă��邩�H
	*/
	const bool GetIntersectPosWithMobius(
		const Vector3& startPos, const Vector3& endPos, Vector3* const IntersectPos = nullptr
	);

private:	//�f�[�^�����o
	CModelRender* m_modelRender = nullptr;	//���f�������_���[
	Vector3 m_position = g_VEC3_ZERO;		//�ꏊ
	Quaternion m_rotation = g_QUAT_IDENTITY;	//��]
};

