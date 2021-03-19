#pragma once

/// <summary>
/// �X�v���C�g�`��N���X
/// </summary>
class CSpriteRender : public IGameObject
{
public:		//�����ł�΂�郁���o�֐�
	bool Start()override final;								//�X�^�[�g�֐�
	~CSpriteRender();										//�f�X�g���N�^
	void Update()override final;							//�A�b�v�f�[�g�֐�
	void Render(RenderContext& rc)override final;			//�`��p�֐�
	void PostRender(RenderContext& rc)override final;		//��ԏ�ɕ`�悷��֐�

public:		//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// �������p�֐�
	/// �ŏ��ɌĂ�łˁB
	/// </summary>
	/// <param name="texFilePath">dds�t�@�C���p�X</param>
	/// <param name="w">�X�v���C�g�̉���</param>
	/// <param name="h">�X�v���C�g�̏c��</param>
	/// <param name="pivot">�s�{�b�g�i��_�j</param>
	void Init(const char* texFilePath, float w, float h, const Vector2& pivot = { 0.5f,0.5f });

	/// <summary>
	/// �ꏊ��ݒ肷��B{0.0f,0.0f}������
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// �g���ݒ肷��
	/// </summary>
	/// <param name="scale">�g��</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// �s�{�b�g�i��_�j��ݒ肷��
	/// {0.0f,0.0f}������
	/// {0.5f,0.5f}������
	/// {1.0f,1.0f}���E��
	/// </summary>
	/// <param name="pivot">�s�{�b�g�i��_�j</param>
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
	}

	/// <summary>
	/// PostRender�ŕ`�悷�邩�ǂ����ݒ肷��֐�
	/// ������true�������PostRender�ŕ`�悳���悤�ɂȂ�A
	/// ��ԏ�ɕ`�悳���悤�ɂȂ�B
	/// </summary>
	/// <param name="flag">PostRender�ŕ`�悷�邩�H</param>
	void SetPostRenderFlag(const bool flag)
	{
		m_postRenderFlag = flag;
	}

private:	//�f�[�^�����o
	Sprite m_sprite;							//�X�v���C�g�N���X
	SpriteInitData m_initData;					//�X�v���C�g�̏������f�[�^�B�\���́B
	
	Vector3 m_position = { 100.0f,0.0f,0.0f };	//�ꏊ
	Quaternion m_rotation = g_quatIdentity;		//��]
	Vector3 m_scale = g_vec3One;				//�g��
	Vector2 m_pivot = { 0.5f,0.5f };			//�s�{�b�g�i��_�j
	
	bool m_postRenderFlag = false;				//PostRender�ŕ`�悷�邩�ǂ����H
};

