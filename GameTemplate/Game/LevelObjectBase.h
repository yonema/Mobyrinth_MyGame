#pragma once
#include "ModelRender.h"
#include "LevelObjectManager.h"
#include "Player.h"
#include "OBB.h"

//�f�o�b�N�p
//��ŏ���
#include "ModelRender.h"

/// <summary>
/// ���x���I�u�W�F�N�g�̒��ۃN���X
/// ReversibleObject��ObstacleObject�͂��̒��ۃN���X���p�����Ă���
/// </summary>
class ILevelObjectBase : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;				//�X�^�[�g�֐�
	virtual ~ILevelObjectBase();			//�f�X�g���N�^
	void Update()override final;			//�A�b�v�f�[�g�֐�

public:		//�I�[�o�[���C�h���Ăق��������o�֐�
	virtual bool PureVirtualStart() = 0 {};		//�X�^�[�g�֐�
	virtual void PureVirtualUpdate() = 0 {};	//�A�b�v�f�[�g�֐�

protected:	//�����̃����o�֐�����Ɏg��
	/// <summary>
	/// �߂��̃E�F�C�|�C���g��T���āA�C�C�����ɉ�]����֐�
	/// </summary>
	void CheckWayPoint();

public:		//�����̃����o�֐�����Ɏg��
	
	/// <summary>
	///	�ꏊ��ݒ肷��
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// �ꏊ���擾����
	/// </summary>
	/// <returns>�ꏊ</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// ��]���擾����
	/// </summary>
	/// <returns>��]</returns>
	const Quaternion GetRotation()const
	{
		return m_rotation;
	}

	/// <summary>
	/// �g���ݒ�
	/// </summary>
	/// <param name="scale">�g��</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// �g����擾
	/// </summary>
	/// <returns>�g��</returns>
	const Vector3 GetScale()const 
	{
		return m_scale;
	}

	/// <summary>
	/// �^�C�v��ݒ肷��
	/// �^�C�v��LevelObjectBase.h��EnObjectType���Q��
	/// </summary>
	/// <param name="objectType">�^�C�v</param>
	void SetObjectType(int objectType)
	{
		m_objectType = objectType;
	}

	/// <summary>
	/// �^�C�v���擾����
	/// �^�C�v��LevelObjectBase.h��EnObjectType���Q��
	/// </summary>
	/// <returns>�^�C�v</returns>
	const int GetObjectType()const
	{
		return m_objectType;
	}

	/// <summary>
	/// OBB�̎Q�Ƃ�߂�
	/// </summary>
	/// <returns>OBB�̎Q��</returns>
	COBB& GetOBB()
	{
		return m_obb;
	}

	/// <summary>
	/// ����ł���t���O���擾
	/// </summary>
	/// <returns></returns>
	const bool GetIsDead()const
	{
		return m_isDead;
	};

	/// <summary>
	/// LevelObjectManager�Ɏ���ł������ē`����
	/// </summary>
	void Delete()
	{
		m_isDead = true;
	}



	/// <summary>
	/// ���g�ƃv���C���[�̓����蔻��
	/// </summary>
	/// <returns>true���߂��Ă����瓖�����Ă���</returns>
	bool IsHitPlayer();

private:	//private�ȃ����o�֐�

	/// <summary>
	/// OBB�̏������֐�
	/// </summary>
	void InitOBB();

public:		//public�ȃf�[�^�����o
	/// <summary>
	/// �I�u�W�F�N�g�̔ԍ�
	/// �I�u�W�F�N�g�𑝂₵���炱���Ŕԍ��𑝂₷����
	/// </summary>
	enum EnObjectType
	{
		enEnpty,

		//���]�I�u�W�F�N�g
		enWater,
		enIce,
		enBird,
		enFish,
		enGrilledChicken,
		enGrilledFish,
		enRunning,
		enStop,
		enWire,
		enString,
		enNail,
		enBar,
		enAxe,
		enPickaxe,
		enKeymold,
		enKey,
		enPadlock,

		//��Q�I�u�W�F�N�g
		enObsatcle,

		enBigFire,
		enWall,
		enGoal,
		enNotHavePadlock,
		enBox,

		enEnd
	};

protected:	//protected�ȃf�[�^�����o	//����ܗǂ��Ȃ����Ǘ��֐��̂��߂�
	Vector3 m_position = g_vec3Zero;		//�ꏊ
	Quaternion m_rotation = g_quatIdentity;	//��]
	Vector3 m_scale = g_vec3One;			//�g��
	Player* m_pPlayer = nullptr;			//�v���C���[�̃|�C���^

private:	//�f�[�^�����o
	int m_objectType = enEnpty;				//�^�C�v
	bool m_isDead = false;					//����ł��邩�H
	COBB m_obb;								//OBB�̓����蔻��

	//�f�o�b�N�p
	//��ŏ���
private:
	int m_objectNumber = 0;		//���Ԗڂɍ��ꂽ�I�u�W�F�N�g���ێ�����
	static int objectNumber;	//���Ԗڂɍ��ꂽ�I�u�W�F�N�g���J�E���g����

	static const int m_dbgOBBNum = 8;			//OBB�̒��_�̐�
	CModelRender* m_dbgOBBVert[m_dbgOBBNum];	//OBB�̒��_�����邽�߂̃��f�������_���[
	//�f�o�b�N�p�����܂�
};



//��������N���X�O�֐�


/// <summary>
/// OBB���m�̓����蔻��
/// </summary>
/// <param name="lhs">���x���I�u�W�F�N�g1</param>
/// <param name="rhs">���x���I�u�W�F�N�g2</param>
/// <returns>true���߂��Ă����瓖�����Ă���</returns>
bool IsHitObject
(ILevelObjectBase& lhs, ILevelObjectBase& rhs);