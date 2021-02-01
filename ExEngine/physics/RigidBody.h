#pragma once

class ICollider;
//���̏��������B
struct RigidBodyInitData {
	Vector3 pos;					//���W�B
	Quaternion rot;					//��]�B
	Vector3 localInteria;			//�����e���\���B��]�̂��₷���I�Ȃ��́B
	ICollider* collider = nullptr;	//�R���C�_�[�B
	float mass = 0;					//���ʁB

};

/// <summary>
/// ���̃N���X�B
/// </summary>
class RigidBody {
public:
	~RigidBody();
	/// <summary>
	/// ���̂��������B
	/// </summary>
	/// <param name="rbInfo">�������f�[�^�̏��</param>
	void Init(RigidBodyInitData& initData);
	/// <summary>
	/// bulletPhysics�̍��̂��擾�B
	/// </summary>
	/// <returns></returns>
	btRigidBody* GetBody()
	{
		return m_rigidBody.get();
	}
private:
	std::unique_ptr<btRigidBody>			m_rigidBody;		//���́B
	std::unique_ptr<btDefaultMotionState>	m_myMotionState;	//���[�V�����X�e�[�g�B
};
