#pragma once
#include "LightData.h"
#include "DirectionLight.h"
#include "ModelRender.h"

class Player_kari : public IGameObject
{
public://public�ȃ����o�֐�
	bool Start() override final;
	~Player_kari();
	void Update() override final;
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	const Vector3& GetPosition()const
	{
		return m_position;
	}


	void InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap);
	void InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap);

private://private�ȃ����o�֐�

	void CheckWayPoint();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// ���f���̉�]����
	/// </summary>
	void Rotation();

public://�f�o�b�N�p
	void PostRender(RenderContext& rc)override final;
	Font m_font;

private:	//�f�[�^�����o
	/// <summary>
	/// �A�j���[�V�����N���b�v�B
	/// </summary>
	enum EnAnimationClip {
		enAnimClip_Idle,	//�����̃A�j���[�V�����N���b�v
		enAnimClip_Run,		//����̃A�j���[�V�����N���b�v
		enAnimClip_Num,		//�A�j���[�V�����N���b�v�̑���
	};

	AnimationClip m_animationClips[enAnimClip_Num];	//�A�j���[�V�����N���b�v�B
	CharacterController m_charaCon;		//�L�����N�^�[�R���g���[���[

	Vector3 m_moveSpeed = g_vec3Zero;		//�L�����N�^�[�̈ړ��X�s�[�h
	Vector3 m_position = g_vec3Zero;		//�L�����N�^�[�̍��W
	Quaternion m_rotation = g_quatIdentity;	//�L�����N�^�[�̉�]


	CModelRender* m_modelRender = nullptr;


	std::vector<Vector3> m_wayPointPos;
	std::vector<Quaternion> m_wayPointRot;
	int m_lpIndex = 0;
	int m_rpIndex = m_lpIndex + 1;
	int m_wayPointState = 0;
	int m_maxWayPointState = 0;

};

