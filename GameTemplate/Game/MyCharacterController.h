#pragma once
#include "OBB.h"
#include "OBBWorld.h"

/// <summary>
/// �����ō�����L�����N�^�[�R���g���[���[
/// ���̃Q�[���ł����g�p���邱�Ƃ��l���ĂȂ��B
/// </summary>
class CMyCharacterController
{
public:		//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// �������p�֐�
	/// </summary>
	/// <param name="initOBBData">OBB�̏������p�f�[�^�\����</param>
	void Init(const SInitOBBData& initOBBData);

	/// <summary>
	/// ���s�֐��B
	/// �����X�s�[�h�ƃf���^�^�C����n���ƁA
	/// OBBWorld�ɓo�^���Ă���OBB�Ƃ̏Փˉ���������
	/// �ړ���̍��W��߂��B
	/// </summary>
	/// <param name="moveSpeed">�����X�s�[�h</param>
	/// <param name="deltaTime">�f���^�^�C��</param>
	/// <returns>�ړ���̍��W</returns>
	const Vector3 Execute(const Vector3& moveSpeed, const float deltaTime);

	/// <summary>
	/// �Փˉ������C�ɂ��Ȃ��ŁA�ꏊ���ړ�������֐�
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector3& pos)
	{
		m_obb.SetPosition(pos);
	}

	/// <summary>
	/// ���݂̏ꏊ���擾
	/// </summary>
	/// <returns>�ꏊ</returns>
	const Vector3& GetPosition()
	{
		return m_obb.GetPosition();
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	/// <param name="rot">��]</param>
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

	const int GetTag() const
	{
		return m_tag;
	}

	const COBB* GetHitOBB() const
	{
		return m_hitOBB;
	}

private:	//�f�[�^�����o
	COBB m_obb;					//OBB
	int m_tag = COBB::EB_NONE_SIDE;	//�^�O
	const COBB* m_hitOBB = nullptr;
	std::list<COBB*> m_exceptionalOBB;
};

