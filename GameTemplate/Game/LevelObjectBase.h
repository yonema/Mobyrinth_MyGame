#pragma once
#include "ModelRender.h"
#include "LevelObjectManager.h"
#include "Player.h"
#include "OBB.h"
#include "ObjectType.h"

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

	/// <summary>5
	/// �\��������ꍇ�́A�\�������̃��f���̎Q�Ƃ𓾂�B
	/// �Ȃ��ꍇ�́A���ʂɃ��f���̎Q�Ƃ𓾂�
	/// </summary>
	/// <param name="frontOrBack">�\�������H�Ȃ��ꍇ�͊֌W�Ȃ�</param>
	/// <returns>���f���̎Q��</returns>
	virtual CModelRender* GetModelRender(const int frontOrBack = 0)
	{
		return nullptr;
	}

	virtual void SwitchReverse(const bool frontOrBack = 0) {};

protected:	//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// �߂��̃E�F�C�|�C���g��T���āA�C�C�����ɉ�]����֐�
	/// </summary>
	/// <param name="checkRotaton">��]�`�F�b�N���s�����H</param>
	/// <param name="checkPosition">���W�`�F�b�N���s�����H</param>
	void CheckWayPoint(const bool checkRotaton = true, const bool checkPosition = true);

	/// <summary>
	/// ���݂̍��W�ɍ��킹����]�ɂ���
	/// </summary>
	void CheckRotation();

	/// <summary>
	/// �E�F�C�|�C���g�ɂ��낦�����W�ɂ���
	/// </summary>
	void CheckPosition();

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
	const Vector3& GetPosition()const
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
	const Quaternion& GetRotation()const
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
	const Vector3& GetScale()const
	{
		return m_scale;
	}

	/// <summary>
	/// �^�C�v��ݒ肷��
	/// �^�C�v��ObjectType.h��EnObjectType���Q��
	/// </summary>
	/// <param name="objectType">�^�C�v</param>
	void SetObjectType(int objectType)
	{
		m_objectType = objectType;
	}

	/// <summary>
	/// �^�C�v���擾����
	/// �^�C�v��ObjectType.h��EnObjectType���Q��
	/// </summary>
	/// <returns>�^�C�v</returns>
	const int GetObjectType()const
	{
		return m_objectType;
	}


	/// <summary>
	/// ���b�N�����H�ݒ肷��B
	/// Tips�\����S���]�����b�N����
	/// </summary>
	/// <param name="lock">���b�N���邩�H</param>
	void SetLock(const bool lock)
	{
		m_lock = lock;
	}

	/// <summary>
	/// ���b�N�����H�𓾂�
	/// Tips�\����S���]�����b�N����Ă��邩�B
	/// </summary>
	/// <returns>���b�N�����H</returns>
	const bool GetLock() const
	{
		return m_lock;
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

	/// <summary>
	/// ���g�̍����̃E�F�C�|�C���g�̃C���f�b�N�X��߂�
	/// </summary>
	/// <returns>�����̃E�F�C�|�C���g�̃C���f�b�N�X</returns>
	const int GetLeftWayPointIndex() const
	{
		return m_lpIndex;
	}

	/// <summary>
	/// ���g�̍����̃E�F�C�|�C���g�̃C���f�b�N�X��ݒ肷��
	/// </summary>
	/// <param name="lpIndex">�����̃E�F�C�|�C���g�̃C���f�b�N�X</param>
	void SetLeftWayPointIndex(const int lpIndex)
	{
		m_lpIndex = lpIndex;
		m_rpIndex = m_lpIndex - 1;
		const int maxWayPoint = 31;
		if (m_rpIndex < 0)
		{
			m_rpIndex = maxWayPoint;
		}
	}

	/// <summary>
	/// ���g�̉E���̃E�F�C�|�C���g�̃C���f�b�N�X��߂�
	/// </summary>
	/// <returns></returns>
	const int GetRightWayPointIndex() const
	{

		return m_rpIndex;
	}

	/// <summary>
	/// �\���ɂ��邩�����ɂ��邩��߂�
	/// </summary>
	/// <returns>�\����������</returns>
	const int GetFrontOrBackSide() const
	{
		return m_frontOrBackSide;
	}

	/// <summary>
	/// ���g���\���ɂ��邩�����ɂ��邩�𒲂ׂ�֐�
	/// </summary>
	/// <param name="reversibleObject">���]�I�u�W�F�N�g���H</param>
	void CheckFrontOrBackSide(const bool reversibleObject = true);
	
	/// <summary>
	/// �E�F�C�|�C���g����̉��s�̋�����ݒ�
	/// </summary>
	/// <param name="zPosLen">���s�̋���</param>
	void SetZPosLen(const float zPosLen)
	{
		m_zPosLen = zPosLen;
	}

private:	//private�ȃ����o�֐�

	/// <summary>
	/// OBB�̏������֐�
	/// </summary>
	void InitOBB();

protected:	//protected�ȃf�[�^�����o	//����ܗǂ��Ȃ����Ǘ��֐��̂��߂�
	Vector3 m_position = g_vec3Zero;		//�ꏊ
	Quaternion m_rotation = g_quatIdentity;	//��]
	Vector3 m_scale = g_vec3One;			//�g��
	Player* m_pPlayer = nullptr;			//�v���C���[�̃|�C���^


private:	//�f�[�^�����o
	int m_objectType = enEmpty;				//�^�C�v
	bool m_isDead = false;					//����ł��邩�H
	COBB m_obb;								//OBB�̓����蔻��
	int m_lpIndex = 0;						//���g�̍����̃E�F�C�|�C���g�̃C���f�b�N�X
	int m_rpIndex = 0;
	int m_frontOrBackSide = CLevelObjectManager::enNone;	//���g���\���ɂ��邩�����ɂ��邩
	bool m_lock = false;					//���b�N�����H�ATips�\����S���]�����b�N����
	float m_zPosLen = 0.0f;					//�E�F�C�|�C���g����̉��s�̋���
	////////////////////////////////////////////////////////////
	// �����I�u�W�F�N�g�p�̕ϐ��Ɗ֐�
	////////////////////////////////////////////////////////////
public: //Set�֐�
	/// <summary>
	/// �d�Ȃ��Ă��邩�̔���̏������s�����m�F����t���O�̒l��ύX����B
	/// </summary>
	/// <param name="b">�t���O�̒l</param>
	void SetFlagIsHit(const bool b)
	{
		m_flagIsHit = b;
	}

	/// <summary>
	/// �d�Ȃ��Ă��邩�̔���̏������s�����m�F����t���O�̒l���擾
	/// </summary>
	/// <returns>m_flagIsHit�̒l</returns>
	const bool GetFlagIsHit()const
	{
		return m_flagIsHit;
	}

	/// <summary>
	/// �����I�u�W�F�N�g�Ɏg�p����f�[�^������������B
	/// </summary>
	void SetTransparentObject()
	{
		//�����I�u�W�F�N�g����Ɏg�p����t���O��true�ɂ���B
		m_flagTransparentObject = true;
		//�I�u�W�F�N�g�̏d�Ȃ��Ă��锻����s��Ȃ��悤�ɂ���B
		m_flagIsHit = false;
		//���Z�b�g���Ɏg�p����ʒu�A��]����������
		//m_startRotation = m_rotation;
		m_startPosition = m_position;

		//���Z�b�g���Ɏg�p����\������������
		m_startfrontOrBack = m_frontOrBack;
		

		//�I�u�W�F�N�g�𔼓����ɂ���B
		//GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
		//GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
		//��L�̔������ɂ��鏈�������܂������Ȃ������ꍇ�A
		//ChangeTransparent()�֐����g���Ă��������B
	}


public: //�����X�C�b�`�Ɏg�p����֐�
	/// <summary>
	/// �����X�C�b�`�������ꂽ�Ƃ��Ɏg�p�����֐�
	/// </summary>
	void TransparentSwitchOn()
	{
		//�I�u�W�F�N�g�������グ����悤�ɂ���B
		m_flagHeld = true;
		//�I�u�W�F�N�g�̏Փ˔�����s���悤�ɂ���B
		m_flagIsHit = true;

		//�I�u�W�F�N�g�̓����蔻���L���ɂ���B
		m_obb.SetExceptionFlag(false);
	}

	/// <summary>
	/// �����X�C�b�`�̌��ʂ��������Ƃ��Ɏg�p�����֐�
	/// </summary>
	void TransparentSwitchOff()
	{
		//�I�u�W�F�N�g�������グ���Ȃ��悤�ɂ���B
		m_flagHeld = false;
		//�I�u�W�F�N�g�̏Փ˔�����s��Ȃ��悤�ɂ���B
		m_flagIsHit = false;
		//�ʒu�A��]����������Ԃɖ߂��B
		m_rotation = m_startRotation;
		m_position = m_startPosition;
		//�\������������Ԃɖ߂��B
		m_frontOrBack = m_startfrontOrBack;
		CheckWayPoint();
		CheckFrontOrBackSide();
		SwitchReverse(m_frontOrBack);

		//�I�u�W�F�N�g�̓����蔻��𖳌��ɂ���B
		m_obb.SetExceptionFlag(true);

	}

	/// <summary>
	/// �I�u�W�F�N�g�����ݎ����グ���邩�̃t���O�̒l��������B
	/// </summary>
	/// <param name="b">�I�u�W�F�N�g�����ݎ��Ă邩</param>
	void SetFlagHeld(bool b)
	{
		m_flagHeld = b;
	}

	bool GetFlagHeld()
	{
		return m_flagHeld;
	}




	//void SetFlagHeldPlayer(bool b)
	//{
	//	m_flagHeldPlayer = b;
	//}

	//bool GetFlagHeldPlayer()
	//{
	//	return m_flagHeldPlayer;
	//}

	bool GetFlagTransparentObject()
	{
		return m_flagTransparentObject;
	}


private: //�����o�ϐ�
	bool m_flagTransparentObject = false; //�����I�u�W�F�N�g�ǂ����̃t���O
	bool m_flagIsHit = true; //�d�Ȃ��Ă��邩�̔���̏������s�����m�F����t���O
	bool m_flagHeld = true; //�I�u�W�F�N�g�����ݎ����グ���邩�̃t���O
	//bool m_flagHeldPlayer = false; //���݂��̃I�u�W�F�N�g��������Ă��邩�̃t���O

	Vector3 m_startPosition = { 0.0f,0.0f,0.0f }; //�I�u�W�F�N�g�̏����ʒu��ۑ�����ʒu���ϐ�
	Quaternion m_startRotation = g_quatIdentity; //�I�u�W�F�N�g�̏�����]��ۑ������]���ϐ�


	////////////////////////////////////////////////////////////
	// ���]�I�u�W�F�N�g�p�̕ϐ��Ɗ֐��i�����I�u�W�F�N�g�̂��߂Ɉړ��j
	////////////////////////////////////////////////////////////

public:		//public�ȃf�[�^�����o

	/// <summary>
	/// �\��������\���񋓑�
	/// </summary>
	enum EnFrontAndBack
	{
		enFront,			//�\���
		enBack,				//�����
		enFrontAndBackNum,	//�\���̐�
	};

protected:
	bool m_frontOrBack = enFront;				//�\�������H
	bool m_startfrontOrBack = enFront;



	//�f�o�b�N�p
	//��ŏ���
private:
#ifdef MY_DEBUG
	int m_objectNumber = 0;		//���Ԗڂɍ��ꂽ�I�u�W�F�N�g���ێ�����
	static int objectNumber;	//���Ԗڂɍ��ꂽ�I�u�W�F�N�g���J�E���g����

	static const int m_dbgOBBNum = 8;			//OBB�̒��_�̐�
	CModelRender* m_dbgOBBVert[m_dbgOBBNum];	//OBB�̒��_�����邽�߂̃��f�������_���[
#endif
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