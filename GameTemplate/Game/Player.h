#pragma once
#include "LightData.h"
#include "DirectionLight.h"
#include "ModelRender.h"
#include "Mobius.h"
class ILevelObjectBase;
#include "OBB.h"


class Player : public IGameObject
{
public://public�ȃ����o�֐�
	bool Start() override final;
	~Player();
	void Update() override final;
	/// <summary>
	/// �v���C���[�̍��W��ݒ�
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �v���C���[�̉�]��ݒ�
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �v���C���[�̍��W���擾
	/// </summary>
	/// <returns>�ꏊ</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// �E�F�C�|�C���g�́u�ꏊ�v��������
	/// </summary>
	/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
	/// <param name="posMap">�ꏊ�̃}�b�v</param>
	//void InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap);
	/// <summary>
	/// �E�F�C�|�C���g�́u��]�v��������
	/// </summary>
	/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
	/// <param name="rotMap">��]�̃}�b�v</param>
	//void InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap);

	void SetWayPointPos(const std::size_t vecSize, std::vector<Vector3>*const posMap);
	void SetWayPointRot(const std::size_t vecSize, std::vector<Quaternion>* rotMap);

	/// <summary>
	/// �⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]���擾
	/// </summary>
	/// <returns>�⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]</returns>
	const Quaternion GetFinalWPRot()const
	{
		return m_finalWPRot;
	}

	void SetHoldObject(const bool holdFlag)
	{
		m_holdObject = holdFlag;
	}
	void SetOperationFlag(const bool b) {
		m_operationFlag = b;
	}
	const bool GetHoldObject()const
	{
		return m_holdObject;
	}
	const Vector3 GetUpVec()const
	{
		return m_upVec;
	}
	const int GetLeftPointIndex()const
	{
		return m_lpIndex;
	}
	const int GetRightPointIndex()const
	{
		return m_rpIndex;
	}
	const Vector3 GerWayPointPos(const int index)
	{
		return (*m_wayPointPos)[index];
	}

private://private�ȃ����o�֐�

	/// <summary>
	/// �v���C���[�̏����ݒ�
	/// </summary>
	void Init();
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

public://�f�o�b�N�p
	void PostRender(RenderContext& rc)override final;
	Font m_font;
	CModelRender* m_dbgModel = nullptr;
	CModelRender* m_dbgModel2 = nullptr;
	CModelRender* m_dbgModel3 = nullptr;


	bool m_dbgHit = false;
	float m_dbgDot1 = 0.0f;
	float m_dbgDot2 = 0.0f;
	COBB m_obb;
	static const int m_obbNum = 8;
	CModelRender* m_dbgObbModel[m_obbNum] = { nullptr };
	COBB m_obb2;
	CModelRender* m_dbgObbCenter = nullptr;
	CModelRender* m_dbgObbModel2[m_obbNum] = { nullptr };
	bool m_dbgOBBHit = false;

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
	Vector3 m_onWayPosition = g_vec3Zero;		//���̏�̍��W
	Quaternion m_rotation = g_quatIdentity;	//�L�����N�^�[�̉�]
	bool m_holdObject = false;
	Vector3 m_upVec = g_vec3Up;


	enum EnLeftOrRight
	{
		enLeft,		//��
		enRight,	//�E
	};
	int m_leftOrRight = enRight;	//�L�����N�^�[�̍��E�̌���

	float m_padLStickXF = 0.0f;		//�p�b�h�̍��X�e�B�b�N��X���̓��͏��

	CModelRender* m_modelRender = nullptr;	//���f�������_���[

	/// <summary>
	/// �E�F�C�|�C���g�֘A�̃f�[�^�����o
	/// </summary>
	//std::vector<Vector3> m_wayPointPos;		//�E�F�C�|�C���g�́u�ꏊ�v�̃R���e�i
	//std::vector<Quaternion> m_wayPointRot;	//�E�F�C�|�C���g�́u��]�v�̃R���e�i
	std::vector<Vector3>* m_wayPointPos;		//�E�F�C�|�C���g�́u�ꏊ�v�̃R���e�i
	std::vector<Quaternion>* m_wayPointRot;	//�E�F�C�|�C���g�́u��]�v�̃R���e�i
	int m_rpIndex = 0;				//�E�̃E�F�C�|�C���g�̃C���f�b�N�X
	int m_lpIndex = m_rpIndex + 1;	//���̃E�F�C�|�C���g�̃C���f�b�N�X
	int m_wayPointState = 0;		//���g���ǂ̃E�F�C�|�C���g�ɂ��邩�\���X�e�[�g
	int m_maxWayPointState = 0;		//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l
	Quaternion m_finalWPRot = g_quatIdentity;	//�⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]

	Mobius* m_mobius = nullptr;		//�X�e�[�W�̃��r�E�X�̗ւ̃|�C���^

	bool m_operationFlag = false; //����ł��邩�̃t���O
};

