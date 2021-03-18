#pragma once
#include "Mobius.h"
class ILevelObjectBase;

class StartDirecting : public IGameObject
{
public:
	bool Start() override final;
	~StartDirecting();
	void Update() override final;

	void SetWayPointPos(const std::size_t vecSize, std::vector<Vector3>* const posMap);
	void SetWayPointRot(const std::size_t vecSize, std::vector<Quaternion>* rotMap);

private:
	/// <summary>
	/// �E�F�C�|�C���g�̍X�V����
	/// </summary>
	void CheckWayPoint();
	/// <summary>
	/// �ړ�����
	/// </summary>
	void Move();
	/// <summary>
	/// �X�e�[�W�ɏ��
	/// </summary>
	void GetOnStage();
	/// <summary>
	/// ���f���̉�]����
	/// </summary>
	void Rotation();

public:	//�C�����C���֐�
	const bool GetStartDirecting()const
	{
		return m_startDirecting;
	}

	const Vector3 GetPosition()const
	{
		return m_position;
	}
	/// <summary>
	/// �⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]���擾
	/// </summary>
	/// <returns>�⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]</returns>
	const Quaternion GetFinalWPRot()const
	{
		return m_finalWPRot;
	}
	const int GetStartDirectingTime()const
	{
		return startDirectingTime;
	}
	/// <summary>
	/// ���o���̃J�����̒����_�̊J�n�n�_�̍��W��ݒ�
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}


private:
	bool m_startDirecting = true; //�X�e�[�W�J�n���̉��o�����Ă��邩�ǂ����B
	Vector3 m_position = g_vec3Zero; //���o�Ɏg�p���钍���_���W
	Vector3 m_moveSpeed = g_vec3Zero;
	Vector3 m_onWayPosition = g_vec3Zero;		//���̏�̍��W
	Quaternion m_rotation = g_quatIdentity;
	Vector3 m_upVec = g_vec3Up;

	float m_padLStickXF = 0.0f;		//�p�b�h�̍��X�e�B�b�N��X���̓��͏��

	std::vector<Vector3>* m_wayPointPos;		//�E�F�C�|�C���g�́u�ꏊ�v�̃R���e�i
	std::vector<Quaternion>* m_wayPointRot;	//�E�F�C�|�C���g�́u��]�v�̃R���e�i
	int m_rpIndex = 0;				//�E�̃E�F�C�|�C���g�̃C���f�b�N�X
	int m_lpIndex = m_rpIndex + 1;	//���̃E�F�C�|�C���g�̃C���f�b�N�X
	int m_wayPointState = 0;		//���g���ǂ̃E�F�C�|�C���g�ɂ��邩�\���X�e�[�g
	int m_maxWayPointState = 0;		//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l
	Quaternion m_finalWPRot = g_quatIdentity;	//�⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]

	Mobius* m_mobius = nullptr;		//�X�e�[�W�̃��r�E�X�̗ւ̃|�C���^


	enum EnLeftOrRight
	{
		enLeft,		//��
		enRight,	//�E
	};
	int m_leftOrRight = enRight;	//�L�����N�^�[�̍��E�̌���

private:
	bool check = false;
	int count = 0;
	Vector3 m_startPosition = g_vec3Zero; //���o�Ɏg�p���钍���_���W�̊J�n�ʒu

	const int startDirectingTime = 120; //�J��������鉉�o�̎���
};

