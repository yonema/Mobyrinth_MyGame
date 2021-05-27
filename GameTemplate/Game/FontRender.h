#pragma once

/// <summary>
/// �t�H���g�`��N���X
/// </summary>
class CFontRender : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;							//�X�^�[�g�֐�
	~CFontRender();										//�f�X�g���N�^
	void Render(RenderContext& rc)override final;		//�`��p�֐�
	void PostRender(RenderContext& rc)override final;	//��ԏ�ɕ`�悷��֐�

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
		Vector2 position = { 0.0f,0.0f },
		Vector4 color = { 1.0f,1.0f,1.0f,1.0f },
		float rotation = 0.0f,
		float scale = 1.0f,
		Vector2 pivot = { 0.0f,0.0f }
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
	}

	/// <summary>
	/// �ꏊ��ݒ肷��B{0.0f,0.0f}������
	/// </summary>
	/// <param name="pos">�ꏊ</param>
	void SetPosition(const Vector2& pos)
	{
		m_position = pos;
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
	}

	/// <summary>
	/// ��]��ݒ肷��
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const float rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// �g���ݒ肷��
	/// </summary>
	/// <param name="scale">�g��</param>
	void SetScale(const float scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// �s�{�b�g�i��_�j��ݒ肷��
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


	void SetShadowParam(const bool isDrawShadow, const float shadowOffset, const Vector4& shadowColor)
	{
		m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
	}


private:
	static const int m_maxTextSize = 256;
private:	//�f�[�^�����o
	Font m_font;								//�t�H���g�N���X
	wchar_t m_text[m_maxTextSize];				//�\������e�L�X�g
	Vector2 m_position = { 0.0f,0.0f };			//�\������ꏊ�B{0.0f,0.0f}������
	Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f };	//�J���[{1.0f,1.0f,1.0f,1.0f}�����F
	float m_rotation = 0.0f;					//��]
	float m_scale = 1.0f;						//�g��
	Vector2 m_pivot = { 0.0f,0.0f };			//�s�{�b�g�i��_�j

	bool m_postRenderFlag = false;				//PostRender�ŕ`�悷�邩�ǂ����H
};

