#pragma once

//COBB�̏������p�̍\����
struct SInitOBBData
{
	Vector3 position = g_vec3Zero;			//�ꏊ
	float width = 1.0f;						//X�̕ӂ̒���
	float height = 1.0f;					//Y�̕ӂ̒���
	float length = 1.0f;					//Z�̕ӂ̒���
	Quaternion rotation = g_quatIdentity;	//��]
	Vector3 pivot = { 0.5f, 0.5f, 0.5f };	//�s�{�b�g�i��_�j
};


/// <summary>
/// OBB�N���X�iOriented Bounding Box�j
/// �����蔻��p
/// </summary>
class COBB
{
public:		//�����ŌĂ΂�郁���o�֐�
	COBB();		//�R���X�g���N�^

public:		//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// �������֐��B
	/// �ŏ��ɌĂ�łˁB
	/// </summary>
	/// <param name="initData">COBB�̏������p�̍\����</param>
	void Init(SInitOBBData& initData);

	/// <summary>
	/// �ꏊ�̐ݒ�
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		//�Z���^�[�|�W�V�������v�Z����
		CalcCenterPosition();
	}

	/// <summary>
	/// �ꏊ�̎擾
	/// </summary>
	/// <returns>�ꏊ</returns>
	const Vector3 GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// �Z���^�[�|�W�V�����̎擾
	/// </summary>
	/// <returns>�Z���^�[�|�W�V����</returns>
	const Vector3 GetCenterPosition()const
	{
		return m_centerPosition;
	}

	/// <summary>
	/// �s�{�b�g�i��_�j��ݒ肷��
	/// </summary>
	/// <param name="pivot">�s�{�b�g�i��_�j</param>
	void SetPivot(const Vector3& pivot)
	{
		m_pivot = pivot;
		//�Z���^�[�|�W�V�������v�Z����
		CalcCenterPosition();
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	/// <param name="rot">��]</param>
	void SetRotation(const Quaternion& rot)
	{
		Rotating(rot);
		//�Z���^�[�|�W�V�������v�Z����
		CalcCenterPosition();
	}

	void SetDirectionLength(const Vector3& directionLength)
	{
		m_directionLength[enLocalX] = directionLength.x / 2;
		m_directionLength[enLocalY] = directionLength.y / 2;
		m_directionLength[enLocalZ] = directionLength.z / 2;
		//�Z���^�[�|�W�V�������v�Z����
		CalcCenterPosition();
	}

	/// <summary>
	/// �{�b�N�X��8�̒��_�̍��W���擾����B
	/// �i���Ӂj�z��̐擪�A�h���X��߂��Ă��邯�ǁA
	/// �z��̗v�f����8����B
	/// </summary>
	/// <returns>���_�̔z��̐擪�A�h���X</returns>
	Vector3* GetBoxVertex();


	/// <summary>
	/// �����Ŏw�肵�����[�J�����̒P�ʕ����x�N�g���̎擾
	/// </summary>
	/// <param name="localAxis">�߂������P�ʕ����x�N�g���̃��[�J����</param>
	/// <returns>�P�ʕ����x�N�g��</returns>
	const Vector3 GetNormalDirection(const int localAxis)const
	{
		return m_normalDirection[localAxis];
	}
	/// <summary>
	/// �����Ŏw�肵�����[�J�����̕����x�N�g���̒���
	/// </summary>
	/// <param name="localAxis">�߂����������x�N�g���̒����̃��[�J����</param>
	/// <returns>�����x�N�g���̒���</returns>
	const float GetDirectionLength(const int localAxis)const
	{
		return m_directionLength[localAxis];
	}

private:	//private�ȃ����o�֐�
	/// <summary>
	/// �P�ʕ����x�N�g�����񂷊֐�
	/// </summary>
	/// <param name="rot">��]</param>
	void Rotating(const Quaternion& rot);
	/// <summary>
	/// �s�{�b�g�ƃ|�W�V���������ƂɁA
	/// �Z���^�[�|�W�V�������v�Z����B
	/// </summary>
	void CalcCenterPosition();
public:		//public�ȃf�[�^�����o
	//�I�u�W�F�N�g�̃��[�J���Ȏ�
	enum ENLocalAxis
	{
		enLocalX,		//���[�J��X��
		enLocalY,		//���[�J��Y��
		enLocalZ,		//���[�J��Z��
		enLocalAxisNum,	//���[�J�����̐�
	};
private:	//�f�[�^�����o
	Vector3 m_position;							//�ꏊ
	Vector3 m_pivot;							//�s�{�b�g�i��_�j
	Vector3 m_centerPosition;					//�Z���^�[�|�W�V�����iOBB�̒����̏ꏊ�j
	
	Vector3 m_normalDirection[enLocalAxisNum];	//�e���̒P�ʕ����x�N�g��
	float m_directionLength[enLocalAxisNum];	//�e���̕����x�N�g���̒���

};

//�N���X�O
//�֐�


/// <summary>
/// OBB���m�̓����蔻��
/// ����������true��߂�
/// </summary>
/// <param name="obb1">�Е���OBB</param>
/// <param name="obb2">�����Е���OBB</param>
/// <returns>�����������ǂ���</returns>
const bool CollisionOBBs(COBB& obb1, COBB& obb2);

/// <summary>
/// �ˉe�������v�Z����֐�
/// </summary>
/// <param name="sepAxis">�ˉe����镪�����i���K���ς݂�n�����Ɓj</param>
/// <param name="dirVec_X">�ˉe��������x�N�g��X�i�傫���t����n�����Ɓj</param>
/// <param name="dirVec_Y">�ˉe��������x�N�g��Y�i�傫���t����n�����Ɓj</param>
/// <param name="dirVec_Z">�ˉe��������x�N�g��Z�i�傫���t����n�����Ɓj(Z�͂Ȃ��Ă��ǂ�)</param>
/// <returns>�ˉe����</returns>
const float CalcProjectionLen(
	const Vector3& sepAxis,
	const Vector3& dirVec_X,
	const Vector3& dirVec_Y,
	const Vector3& dirVec_Z = { 0.0f,0.0f,0.0f }
);