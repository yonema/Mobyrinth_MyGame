#pragma once
#include "LightData.h"
#include "DirectionLight.h"
#include "ModelRender.h"
#include "Mobius.h"
#include "OBB.h"
#include "MyCharacterController.h"
#include "SoundCue.h"
#include "GameTime.h"

class CReversibleObject;



/// <summary>
/// �v���C���[�����삷��L�����N�^�[�̃N���X
/// </summary>
class Player : public IGameObject
{
public://public�ȃ����o�֐�
	bool Start() override final;	//�X�^�[�g�֐�
	~Player();						//�f�X�g���N�^
	void Update() override final;	//�A�b�v�f�[�g�֐�

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
	const Vector3& GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// �E�F�C�|�C���g�́u�ꏊ�v���擾
	/// </summary>
	/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
	/// <param name="posMap">�ꏊ�̃x�N�^�[</param>
	void SetWayPointPos(const std::size_t vecSize, std::vector<Vector3>*const posMap);

	/// <summary>
	/// �E�F�C�|�C���g�́u��]�v���擾
	/// </summary>
	/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
	/// <param name="rotMap">��]�̃x�N�^�[</param>
	void SetWayPointRot(const std::size_t vecSize, std::vector<Quaternion>* rotMap);

	//�E�F�C�|�C���g�p��OBB��ݒ�
	void SetWayPointOBB();

	/// <summary>
	/// �⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]���擾
	/// </summary>
	/// <returns>�⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]</returns>
	const Quaternion& GetFinalWPRot()const
	{
		return m_finalWPRot;
	}
	
	/// <summary>
	/// �v���C���[���I�u�W�F�N�g�������Ă��邩�ǂ�����ݒ肷��
	/// �����Ă���ꍇ��true��n��
	/// </summary>
	/// <param name="holdFlag">�I�u�W�F�N�g�������Ă��邩�H</param>
	void SetHoldObject(const bool holdFlag, CReversibleObject* reversibeObject = nullptr)
	{
		m_holdObject = holdFlag;
		if (m_holdObject)
			m_reversibleObject = reversibeObject;
		else
			m_reversibleObject = nullptr;
	}

	/// <summary>
	/// �v���C���[���I�u�W�F�N�g�������Ă��邩�ǂ����𒲂ׂ�
	/// �����Ă���ꍇ��true���߂��Ă���
	/// </summary>
	/// <returns>�I�u�W�F�N�g�������Ă��邩</returns>
	const bool GetHoldObject()const
	{
		return m_holdObject;
	}

	/// <summary>
	/// �����Ă��锽�]�I�u�W�F�N�g�̎Q�Ƃ𓾂�
	/// </summary>
	/// <returns>�����Ă��锽�]�I�u�W�F�N�g�̎Q��</returns>
	CReversibleObject* GetReversibleObject()
	{
		return m_reversibleObject;
	}

	void SetOperationFlag(const bool b)
	{
		m_operationFlag = b;
	}
	void SetTitleMove(const bool b)
	{
		m_titleMove = b;
	}


	/// <summary>
	/// �v���C���[�̌��݂�Up�x�N�g���𓾂�
	/// </summary>
	/// <returns>Up�x�N�g��</returns>
	const Vector3& GetUpVec()const
	{
		return m_upVec;
	}

	/// <summary>
	/// ���݂̃v���C���[�̍����̃E�F�C�|�C���g�̔ԍ��𓾂�
	/// </summary>
	/// <returns>�����̃E�F�C�|�C���g�̔ԍ�</returns>
	const int GetLeftPointIndex()const
	{
		return m_lpIndex;
	}

	/// <summary>
	/// �v���C���[�̍����̃E�F�C�|�C���g��ݒ肷��
	/// </summary>
	/// <param name="lpIndex">�����̃E�F�C�|�C���g</param>
	void SetLeftPointIndex(const int lpIndex)
	{
		m_lpIndex = lpIndex;
	}

	/// <summary>
	/// ���݂̃v���C���[�̉E���̃E�F�C�|�C���g�̔ԍ��𓾂�
	/// </summary>
	/// <returns>�E���̃E�F�C�|�C���g�̔ԍ�</returns>
	const int GetRightPointIndex()const
	{
		return m_rpIndex;
	}

	/// <summary>
	/// �v���C���[�̉E���̃E�F�C�|�C���g��ݒ肷��
	/// </summary>
	/// <param name="lpIndex">�E���̃E�F�C�|�C���g</param>
	void SetRightPointIndex(const int rpIndex)
	{
		m_rpIndex = rpIndex;
	}

	/// <summary>
	/// �����œn���ꂽ�ԍ��̃E�F�C�|�C���g�̏ꏊ��߂�
	/// </summary>
	/// <param name="index">�E�F�C�|�C���g�̔ԍ�</param>
	/// <returns>�E�F�C�|�C���g�̏ꏊ</returns>
	const Vector3& GerWayPointPos(const int index)
	{
		return (*m_wayPointPos)[index];
	}

	/// <summary>
	/// �L�����N�^�[�����ƉE�ǂ���������Ă��邩���ׂ�
	/// </summary>
	/// <returns>enLeft�i0�j��enRight�i1�j</returns>
	const int GetEnLeftOrRight()const
	{
		return m_leftOrRight;
	}

	/// <summary>
	/// ���g���ǂ̃E�F�C�|�C���g�ɂ��邩�\���X�e�[�g��߂��֐�
	/// </summary>
	/// <returns>�E�F�C�|�C���g�̃X�e�[�g</returns>
	const int GetWayPointState()const
	{
		return m_wayPointState;
	}

	/// <summary>
	/// ���g���ǂ̃E�F�C�|�C���g�ɂ��邩�\���X�e�[�g��ݒ�
	/// </summary>
	/// <param name="wayPointState">�E�F�C�|�C���g�̃X�e�[�g</param>
	void SetWayPointState(const int wayPointState)
	{
		m_wayPointState = wayPointState;
	}

	/// <summary>
	/// UFO�ɕ߂܂��Ă��邩�H��ݒ�
	/// </summary>
	/// <param name="captured">UFO�ɕ߂܂��Ă��邩�H</param>
	void SetCapturedUFOFlag(const bool captured)
	{
		m_capturedUFOFlag = captured;
	}

	void SetFallFlag(const bool fallFlag)
	{
		m_fallFlag = fallFlag;
	}

	/// <summary>
	/// OBB�̎Q�Ƃ�߂�
	/// </summary>
	/// <returns>OBB�̎Q��</returns>
	COBB& GetOBB()
	{
		return m_myCharaCon.GetOBB();
	}


private://private�ȃ����o�֐�

	void TitleMove();
	void GameMove();

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
	/// �X�^�����̈ړ�����
	/// </summary>
	void StunMove();

	/// <summary>
	/// �X�e�[�W�ɏ��
	/// </summary>
	void GetOnStage();

	/// <summary>
	/// �X�^�����̃X�e�[�W�ɏ�鏈��
	/// </summary>
	void StunGetOnStage();

	/// <summary>
	/// ���f���̉�]����
	/// </summary>
	void Rotation();

	/// <summary>
	/// �Փ˂���OBB�̃^�O�𒲂ׂ�
	/// </summary>
	void CheckHitOBBTag();

	/// <summary>
	/// ���C�g�̃f�[�^���X�V����
	/// </summary>
	void UpdateLightData();

	/// <summary>
	/// �v���C���[���Ƃ炷�e�𐶐����郉�C�g���X�V����
	/// </summary>
	void SetShadowParam();

	/// <summary>
	/// �f�B���N�V�������C�g���X�V����
	/// </summary>
	void SetDirectionLight();

	/// <summary>
	/// UFO�ɕ߂܂��Ă��鎞�̏���
	/// </summary>
	void CapturedUFO();

	void Fall();

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

	Vector3 m_moveSpeed = g_vec3Zero;				//�L�����N�^�[�̈ړ��X�s�[�h
	Vector3 m_position = g_vec3Zero;				//�L�����N�^�[�̍��W
	Vector3 m_onWayPosition = g_vec3Zero;			//���̏�̍��W
	Quaternion m_rotation = g_quatIdentity;			//�L�����N�^�[�̉�]
	Vector3 m_upVec = g_vec3Up;						//�v���C���[��Up�x�N�g��
	bool m_holdObject = false;						//�I�u�W�F�N�g�������Ă��邩�H
	CReversibleObject* m_reversibleObject = nullptr;	//�����Ă��锽�]�I�u�W�F�N�g�̃|�C���^
	
	/// <summary>
	/// �v���C���[���E�������Ă��邩���������Ă��邩
	/// </summary>
	enum EnLeftOrRight
	{
		enLeft,		//��
		enRight,	//�E
	};
	int m_leftOrRight = enRight;			//�L�����N�^�[�̍��E�̌���

	float m_padLStickXF = 0.0f;				//�p�b�h�̍��X�e�B�b�N��X���̓��͏��

	CModelRender* m_modelRender = nullptr;	//���f�������_���[
	CMyCharacterController m_myCharaCon;	//my�L�����N�^�[�R���g���[���[
	Mobius* m_mobius = nullptr;				//�X�e�[�W�̃��r�E�X�̗ւ̃|�C���^
	CDirectionLight* m_gameDirectionLight = nullptr;	//�f�B���N�V�������C�g�̃|�C���^
	Vector3 m_lightDirection = g_vec3Zero;	//���C�g�̏Ƃ炷����

	/// <summary>
	/// �E�F�C�|�C���g�֘A�̃f�[�^�����o
	/// </summary>
	std::vector<Vector3>* m_wayPointPos;		//�E�F�C�|�C���g�́u�ꏊ�v�̃R���e�i
	std::vector<Quaternion>* m_wayPointRot;		//�E�F�C�|�C���g�́u��]�v�̃R���e�i
	int m_rpIndex = 0;							//�E�̃E�F�C�|�C���g�̃C���f�b�N�X
	int m_lpIndex = m_rpIndex + 1;				//���̃E�F�C�|�C���g�̃C���f�b�N�X
	int m_wayPointState = 0;					//���g���ǂ̃E�F�C�|�C���g�ɂ��邩�\���X�e�[�g
	int m_maxWayPointState = 0;					//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l
	Quaternion m_finalWPRot = g_quatIdentity;	//�⊮�ς݂̍ŏI�I�ȃE�F�C�|�C���g�̉�]
	std::vector<COBB> m_wayPointOBB;			//�E�F�C�|�C���g�p��OBB


	bool m_operationFlag = false; //����ł��邩�̃t���O
	bool m_titleMove = false;
	bool m_stunFlag = false;					//�X�^����ԁi������я�ԁj���H
	float m_stunTimer = 0.0f;					//�X�^���̃^�C�}�[
	Vector3 m_stunMoveSpeed = g_vec3Zero;		//�X�^�����̐�����΂��x�N�g��
	Vector3 m_stunDownVec = g_vec3Zero;			//�X�^�����̏d��
	bool m_stunMoveFlag = true;					//�X�^�����ɐ�����ђ����H
	float m_blinkTimer = 0.0f;					//�X�^����ԂɃ��f����_�ł�����^�C�}�[
	COBB* m_hitOBB = nullptr;					//�Փ˂���OBB�̃|�C���^

	bool m_capturedUFOFlag = false;				//UFO�ɃL���v�`������Ă��邩�H
	bool m_fallFlag = false;					//���������H

public://�f�o�b�N�p
	//void PostRender(RenderContext& rc)override final;	//�f�o�b�N�p�̃t�H���g��\�����邽��
	//Font m_font;							//�t�H���g��\�����邽�߂̃N���X
	//CModelRender* m_dbgModel = nullptr;		//�v���C���[�ƃX�e�[�W�Ƃ̓����蔻���
	//CModelRender* m_dbgModel2 = nullptr;	//��邽�߂̃��C�̎��_�ƏI�_�ƃX�e�[�W�Ƃ�
	//CModelRender* m_dbgModel3 = nullptr;	//�����_�����邽�߂̃��f��
	//bool m_dbgHit = false;					//��̃��C���������Ă��邩�\�����邽�߂̕ϐ�

	//float m_dbgDot1 = 0.0f;			//�����̃E�F�C�|�C���g�ƃv���C���[�̓��ς�����
	//float m_dbgDot2 = 0.0f;			//�E���̃E�F�C�|�C���g�ƃv���C���[�̓��ς�����

	//CModelRender* m_dbgObbModel[8] = { nullptr };	//OBB�̒��_�����邽�߂̃��f��
	//CModelRender* m_dbgObbModel2[32][8];
	//CModelRender* m_dbgStunMoveModel = nullptr;
};

