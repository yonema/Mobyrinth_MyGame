#pragma once
#include "Mobius.h"
#include "GameTime.h"
#include "UFO.h"
class ILevelObjectBase;
class Player;
class GameCamera;

class StartDirecting : public IGameObject
{
public:
	bool Start() override final;
	~StartDirecting();
	void Update() override final;

	//void SetWayPointPos(const std::size_t vecSize, std::vector<Vector3>* const posMap);
	//void SetWayPointRot(const std::size_t vecSize, std::vector<Quaternion>* rotMap);

private:
	///// <summary>
	///// �E�F�C�|�C���g�̍X�V����
	///// </summary>
	//void CheckWayPoint();
	///// <summary>
	///// �ړ�����
	///// </summary>
	//void Move();
	///// <summary>
	///// �X�e�[�W�ɏ��
	///// </summary>
	//void GetOnStage();
	///// <summary>
	///// ���f���̉�]����
	///// </summary>
	//void Rotation();
	/// <summary>
	/// �v���C���[�̉��o���̗�������
	/// </summary>
	void FallPlayer();

public:	//Get�ASet�֐�
	const bool GetStartDirecting()const
	{
		return m_checkStartDirecting;
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
	void SetCheckStartDirecting(bool b)
	{
		m_checkStartDirecting = b;
	}

	/// <summary>
	/// ���C�v���I���������ݒ�
	/// </summary>
	/// <param name="wipeEndFlag">���C�v���I������H</param>
	void SetWipeEndFlag(const bool wipeEndFlag)
	{
		m_wipeEndFlag = wipeEndFlag;
	}


private:
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

	Player* m_player = nullptr;		//�v���C���[�̏�����

	GameCamera* m_gameCamera = nullptr;		//�Q�[���J�����̏�����

	enum EnLeftOrRight
	{
		enLeft,		//��
		enRight,	//�E
	};
	int m_leftOrRight = enRight;	//�L�����N�^�[�̍��E�̌���

private:
	bool m_checkStartDirecting = true; //�X�e�[�W�J�n���̉��o�����Ă��邩�ǂ����B�iGameCamera�N���X�Ŏg�p�j
	bool checkZoomIn = false; //�J�����̃Y�[�������̃t���O
	int countZoomIn = 0; //�J�����̃Y�[�����̃J�E���g
	Vector3 m_startPosition = g_vec3Zero; //���o�Ɏg�p���钍���_���W�̊J�n�ʒu

	const int startDirectingTime = 120; //�J��������鉉�o�̎���

	bool m_wipeEndFlag = false;			//���C�v���I������H
	float m_startTimer = 0;				//���o�J�n�܂ł̃^�C�}�[

	float m_fallTimer = 0.0f;			//�����ォ��J�n�܂ł̎���
	
	bool CheckAButton = false; //A�{�^�������������̊m�F


public:
	bool GetCheckAButton()
	{
		return CheckAButton;
	}

	void SetCheckAButton(bool b)
	{
		CheckAButton = b;
	}


};

