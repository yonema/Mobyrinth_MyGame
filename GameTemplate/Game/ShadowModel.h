#pragma once

/// <summary>
/// �V���h�E�p���f���N���X
/// </summary>
class CShadowModel : private Noncopyable
{
public:		//�����ŌĂ΂�郁���o�֐�
	CShadowModel();		//�R���X�g���N�^
	~CShadowModel();	//�f�X�g���N�^

public:		//�����o�֐�

	/// <summary>
	/// �������֐�
	/// </summary>
	void Init();

	/// <summary>
	/// ���f���̏������֐�
	/// </summary>
	/// <param name="initData">���f���̏������f�[�^</param>
	void InitModel(ModelInitData& initData);

	/// <summary>
	/// ���f���̃p�����[�^�X�V
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g��</param>
	void UpdateModel(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

	/// <summary>
	/// �V���h�E�p�̃��f���̓o�^����������
	/// </summary>
	void RemoveShadowModel();

	/// <summary>
	/// �L�����ǂ����H
	/// </summary>
	/// <returns>�L�����H</returns>
	const bool IsValid() const
	{
		return m_isValid;
	}

private:	//�f�[�^�����o
	std::unique_ptr<Model> m_model = nullptr;	//���f��
	bool m_isValid = false;						//�L�����H
};

