#pragma once
#include "GameTime.h"

/// <summary>
/// �΂˃J����
/// </summary>
class CSpringCamera
{
public:		//�����ŌĂ΂�郁���o�֐�
	CSpringCamera();		//�R���X�g���N�^
	~CSpringCamera();		//�f�X�g���N�^

public:		//�����o�֐�

	/// <summary>
	///	������
	/// �R���W���������͌��݂͂����̏���B�������Ă��Ȃ��B
	/// </summary>
	/// <param name="camera">�J����</param>
	/// <param name="maxMoveSpeed">�J�����̍ő�ړ����x</param>
	/// <param name="isEnableCollisionSolver">�R���W����������L���ɂ���H</param>
	/// <param name="sphereCollisionRadius">���̃R���W�����̔��a</param>
	void Init(
		Camera& camera,
		float maxMoveSpeed,
		bool isEnableCollisionSolver,
		float sphereCollisionRadius
	);

	/// <summary>
	/// �ڕW�ƂȂ钍���_��ݒ�B
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const Vector3& target)
	{
		m_target = target;
	}

	/// <summary>
	/// �ڕW�ƂȂ鎋�_��ݒ�B
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector3 position)
	{
		m_position = position;
	}

	/// <summary>
	/// �����ʂ�ݒ�B
	/// </summary>
	/// <param name="_far"></param>
	void SetFar(float _far)
	{
		if (m_camera == nullptr) {
			return;
		}
		m_camera->SetFar(_far);
	}

	/// <summary>
	/// �ߕ��ʂ�ݒ�B
	/// </summary>
	/// <param name="_near"></param>
	void SetNear(float _near)
	{
		if (m_camera == nullptr) {
			return;
		}
		m_camera->SetNear(_near);
	}

	/// <summary>
	/// �J�����̏������ݒ�
	/// </summary>
	/// <param name="up"></param>
	void SetUp(const Vector3 up)
	{
		if (m_camera == nullptr)
		{
			return;
		}
		m_camera->SetUp(up);

	}

	/// <summary>
	/// �J�����̉�]�s����擾�B
	/// </summary>
	/// <returns></returns>
	const Matrix& GetCameraRotation() const
	{
		if (m_camera == nullptr) {
			return Matrix::Identity;
		}
		return m_camera->GetCameraRotation();
	}

	/// <summary>
	/// �ڕW�ƂȂ钍���_���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetTarget() const
	{
		return m_target;
	}

	/// <summary>
	/// �ڕW�ƂȂ鎋�_���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// ���ۂ̒����_���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetRealTarget() const
	{
		if (m_camera == nullptr) {
			return Vector3::Zero;
		}
		return m_camera->GetTarget();

	}

	/// <summary>
	/// ���ۂ̎��_���擾�B
	/// </summary>
	/// <returns></returns>
	const Vector3& GetRealPosition() const
	{
		if (m_camera == nullptr) {
			return Vector3::Zero;
		}
		return m_camera->GetPosition();
	}

	/// <summary>
	/// �X�V�B
	/// </summary>
	void Update();

	/// <summary>
	/// �J�������擾�B
	/// </summary>
	/// <returns></returns>
	const Camera* GetCamera() const
	{
		return m_camera;
	}

	/// <summary>
	/// �o�l�̌�������ݒ�B
	/// �l���傫���قǃJ�������x��ĕt���Ă��܂�
	/// </summary>
	/// <param name="dampingRate"></param>
	void SetDampingRate(float dampingRate)
	{
		m_targetDampingRate = dampingRate;
	}

	/// <summary>
	/// �J�������擾�B
	/// </summary>
	/// <returns></returns>
	Camera* GetCamera()
	{
		return m_camera;
	}
	void SetViewAngle(float angle)
	{
		if (m_camera == nullptr) {
			return;
		}
		m_camera->SetViewAngle(angle);
	}
	float GetViewAngle() const
	{
		if (m_camera == nullptr) {
			return 0.0f;
		}
		return m_camera->GetViewAngle();
	}

	/// <summary>
	/// ���t���b�V��
	/// ���̊֐����Ăяo���ƁA�o�l�̈ړ������t���b�V������āAUpdate�֐����Ăяo���ƃJ�����̈ʒu��
	/// �����ɖڕW���_�ƖڕW�����_�Ɉړ����܂��B�V�[�����؂�ւ���ăJ��������C�ɐ؂�ւ������Ƃ��Ȃǂ�
	/// �g�p���Ă��������B
	/// </summary>
	void Refresh()
	{
		m_targetMoveSpeed = Vector3::Zero;
		m_positionMoveSpeed = Vector3::Zero;
		m_isRefresh = true;
	}

	/// <summary>
	/// �o�l�J�����̍X�V�����B
	/// </summary>
	void UpdateSpringCamera();

	/// <summary>
	/// �J�����̍X�V�B
	/// </summary>
	void UpdateCamera()
	{
		if (m_camera) {
			m_camera->Update();
		}
	}

private:	//�f�[�^�����o
	Camera* m_camera = nullptr;							//!<�J�����B
	Vector3	m_target = Vector3::Zero;					//!<�ڕW�ƂȂ钍���_�B
	Vector3	m_position = Vector3::Zero;					//!<�ڕW�ƂȂ鎋�_�B
	Vector3	m_targetMoveSpeed = Vector3::Zero;			//!<�����_�̈ړ����x�B
	Vector3	m_positionMoveSpeed = Vector3::Zero;		//!<���_�̈ړ����x�B
	float		m_maxMoveSpeed = 0.0f;					//!<�ō��ړ����x�B
	float		m_targetDampingRate = 1.0f;				//!<�������B�l���傫���قǃJ�������x��t���Ă���B
	float		m_dampingRate = 1.0f;					//!<�������B
	float		m_dampingRateVel = 0.0f;
	bool		m_isEnableCollisionSolver = false;		//!<�R���W�����������L���H
	bool		m_isRefresh = true;						//!<���t���b�V�����K�v�H
	//CCameraCollisionSolver m_cameraCollisionSolver;
};

