#pragma once
#include "OBB.h"
#include "OBBWorld.h"

class CMyCharacterController
{
public:		//�����̃����o�֐�����Ɏg��
	void Init(const SInitOBBData& initOBBData);
	const Vector3& Execute(const Vector3& moveSpeed, const float deltaTime);
	void SetPosition(const Vector3& pos)
	{
		m_obb.SetPosition(pos);
	}
	const Vector3& GetPosition()
	{
		return m_obb.GetPosition();
	}
	void SetRotation(const Quaternion& rot)
	{
		m_obb.SetRotation(rot);
	}

	/// <summary>
	/// OBB�̎Q�Ƃ�߂�
	/// </summary>
	/// <returns>OBB�̎Q��</returns>
	COBB& GetOBB()
	{
		return m_obb;
	}

private:	//�f�[�^�����o
	COBB m_obb;
};

