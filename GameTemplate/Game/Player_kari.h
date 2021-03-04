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

	const Quaternion GetFinalWPRot()const
	{
		return m_finalWPRot;
	}

private://private�ȃ����o�֐�

	/// <summary>
	/// �E�F�C�|�C���g�̍X�V����
	/// </summary>
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

	Vector3 m_moveSpeed = g_vec3Zero;		//�L�����N�^�[�̈ړ��X�s�[�h
	Vector3 m_position = g_vec3Zero;		//�L�����N�^�[�̍��W
	Quaternion m_rotation = g_quatIdentity;	//�L�����N�^�[�̉�]
	enum EnLeftOrRight
	{
		enLeft,		//��
		enRight,	//�E
	};
	int m_leftOrRight = enLeft;		//�L�����N�^�[�̍��E�̌���

	float m_padLStickXF = 0.0f;		//�p�b�h�̍��X�e�B�b�N��X���̓��͏��

	CModelRender* m_modelRender = nullptr;	//���f�������_���[

	/// <summary>
	/// �E�F�C�|�C���g�֘A�̃f�[�^�����o
	/// </summary>
	std::vector<Vector3> m_wayPointPos;		//�E�F�C�|�C���g�́u�ꏊ�v�̃R���e�i
	std::vector<Quaternion> m_wayPointRot;	//�E�F�C�|�C���g�́u��]�v�̃R���e�i
	int m_rpIndex = 0;				//�E�̃E�F�C�|�C���g�̃C���f�b�N�X
	int m_lpIndex = m_rpIndex + 1;	//���̃E�F�C�|�C���g�̃C���f�b�N�X
	int m_wayPointState = 0;		//���g���ǂ̃E�F�C�|�C���g�ɂ��邩�\���X�e�[�g
	int m_maxWayPointState = 0;		//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l
	Quaternion m_finalWPRot = g_quatIdentity;	//�⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]

};

