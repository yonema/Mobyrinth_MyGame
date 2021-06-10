#pragma once

/**
 * @brief ��N���X�̒萔�f�[�^
*/
namespace
{
	//��̃��f���̃t�@�C���p�X
	const char* const MODEL_FILEPATH_SKY = "Assets/modelData/preset/sky.tkm";
	//��̃V�F�[�_�[�t�@�C���p�X
	const char* const FX_FILEPATH_SKY = "Assets/shader/SkyCubeMap.fx";
	//��̃e�N�X�`���̃t�@�C���p�X
	const wchar_t* const TEXTURE_FILEPATH_SKY = L"Assets/modelData/preset/sky.dds";
}

/**
 * @brief ��N���X
*/
class CSky : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	CSky();		//�R���X�g���N�^
	~CSky();	//�f�X�g���N�^
	bool Start()override final;		//�X�^�[�g�֐�
	void Update()override final;	//�A�b�v�f�[�g�֐�
	void Render(RenderContext& rc)override final;	//�`��֐�

public:		//�����o�֐�

	/**
	 * @brief ���W��ݒ�
	 * @param [in] pos ���W
	*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		//�l���ς��������A�X�V���K�v
		m_isDirty = true;
	}

	/**
	 * @brief �g�嗦��ݒ�iVector3�j
	 * @param [in] scale �g�嗦
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		//�l���ς��������A�X�V���K�v
		m_isDirty = true;
	}

	/**
	 * @brief �g�嗦��ݒ�ifloat�j
	 * @param [in] scale �g�嗦
	*/
	void SetScale(const float scale)
	{
		//�g�嗦�����������Ă���
		m_scale = g_VEC3_ONE;
		//�g��
		m_scale.Scale(scale);
		//�l���ς��������A�X�V���K�v
		m_isDirty = true;
	}

	/**
	 * @brief ���Ȕ����J���[��ݒ�
	 * @param [in] color ���Ȕ�������\
	*/
	void SetEmissionColor(const Vector4 color)
	{
		m_model.SetEmissionColor(color);
		//�l���ς��������A�X�V��
		m_isDirty = true;
	}

private:	//�f�[�^�����o
	Model m_model;							//���f���N���X
	Texture m_skyCubeTexture;				//�X�J�C�L���[�u�e�N�X�`��
	Vector3 m_position = g_VEC3_ZERO;		//���W
	Vector3 m_scale = g_VEC3_ONE;			//�g�嗦
	Vector4 m_selfLuminous = g_VEC4_BLACK;	//���Ȕ����F
	bool m_isDirty = false;					//�_�[�e�B�t���O
};

