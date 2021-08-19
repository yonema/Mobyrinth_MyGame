#pragma once

//�t�H���g�����_���[�̒萔�f�[�^
namespace fontRendetConstData
{
	//�f�t�H���g�̍��W
	const Vector2 POSITION_DEFAULT = { 0.0f,0.0f };
	//�f�t�H���g�̃J���[
	const Vector4 COLOR_DEFAULT = g_VEC4_WHITE;
	//�f�t�H���g�̃s�{�b�g
	const Vector2 PIVOT_DEFAULT = { 0.0f,0.0f };
	//�e�L�X�g�̍ő�̃T�C�Y
	constexpr UINT SIZE_MAX_TEXT = 256;
}

/// <summary>
/// �t�H���g�`��N���X
/// </summary>
class CFontRender : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;							//�X�^�[�g�֐�
	~CFontRender();										//�f�X�g���N�^
	void Render(RenderContext& rc)override final;		//�`��p�֐�

public:		//�����̃����o�֐�����Ɏg���B

	/// <summary>
	/// �������֐��B�ŏ��ɌĂ�łˁB
	/// </summary>
	/// <param name="text">�\������e�L�X�g</param>
	/// <param name="position">���W�B{0.0f,0.0f}������</param>
	/// <param name="color">�J���[{1.0f,1.0f,1.0f,1.0f}�����F</param>
	/// <param name="rotation">��]</param>
	/// <param name="scale">�g��</param>
	/// <param name="pivot">�s�{�b�g�i��_�j</param>
	void Init
	(const wchar_t* text,
		Vector2 position = fontRendetConstData::POSITION_DEFAULT,
		Vector4 color = fontRendetConstData::COLOR_DEFAULT,
		float rotation = 0.0f,
		float scale = 1.0f,
		Vector2 pivot = fontRendetConstData::PIVOT_DEFAULT
	);

	/// <summary>
	/// �\������e�L�X�g��ݒ肷��
	/// </summary>
	/// <param name="text">�e�L�X�g</param>
	void SetText(const wchar_t* text);

	/// <summary>
	/// �\������e�L�X�g���t�H�[�}�b�g�w�肵�Đݒ肷��B
	/// �e���v���[�g��int�Ƃ��t�H�[�}�b�g���w�肵�āA
	/// text�̒��ł�%d�݂����Ɏw�肵�āA
	/// �����Ŏw�肷��B
	/// </summary>
	/// <typeparam name="T">�t�H�[�}�b�g�w��</typeparam>
	/// <param name="text">�e�L�X�g</param>
	/// <param name="var">�\������ϐ�</param>
	template <typename T>
	void SetText(const wchar_t* text, const T var)
	{
		swprintf_s(m_text, text, var);
		return;
	}

	/// <summary>
	/// �ꏊ��ݒ肷��B{0.0f,0.0f}������
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector2& pos)
	{
		m_position = pos;
		return;
	}

	/// <summary>
	/// ���W���擾����
	/// </summary>
	/// <returns>���W</returns>
	const Vector2& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// �J���[��ݒ肷��B{1.0f,1.0f,1.0f,1.0f}�����F
	/// </summary>
	/// <param name="color">�J���[</param>
	void SetColor(const Vector4& color)
	{
		m_color = color;
		return;
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const float rotation)
	{
		m_rotation = rotation;
		return;
	}

	/// <summary>
	/// �g���ݒ肷��
	/// </summary>
	/// <param name="scale">�g��</param>
	void SetScale(const float scale)
	{
		m_scale = scale;
		return;
	}

	/// <summary>
	/// �s�{�b�g�i��_�j��ݒ肷��
	/// </summary>
	/// <param name="pivot">�s�{�b�g�i��_�j</param>
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
		return;
	}


	void SetShadowParam(const bool isDrawShadow, const float shadowOffset, const Vector4& shadowColor)
	{
		m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		return;
	}

private:	//�f�[�^�����o
	Font m_font;								//�t�H���g�N���X

	wchar_t m_text[fontRendetConstData::SIZE_MAX_TEXT];			//�\������e�L�X�g
	Vector2 m_position = fontRendetConstData::POSITION_DEFAULT;	//�\������ꏊ�B{0.0f,0.0f}������
	Vector4 m_color = fontRendetConstData::COLOR_DEFAULT;		//�J���[{1.0f,1.0f,1.0f,1.0f}�����F
	float m_rotation = 0.0f;									//��]
	float m_scale = 1.0f;										//�g��
	Vector2 m_pivot = fontRendetConstData::PIVOT_DEFAULT;		//�s�{�b�g�i��_�j

};

