#pragma once
class CFontRender : public IGameObject
{
public:
	bool Start()override final;
	~CFontRender();
	void Render(RenderContext& rc)override final;
	void PostRender(RenderContext& rc)override final;

	void Init
	(const wchar_t* text,
		Vector2 position = { 0.0f,0.0f },
		Vector4 color = { 1.0f,1.0f,1.0f,1.0f },
		float rotation = 0.0f,
		float scale = 1.0f,
		Vector2 pivot = { 0.0f,0.0f }
	);

	void SetText(const wchar_t* text);

	template <typename T>
	void SetText(const wchar_t* text, const T var)
	{
		wsprintf(m_text, text, var);
	}
	void SetPosition(const Vector2& pos)
	{
		m_position = pos;
	}
	void SetColor(const Vector4& color)
	{
		m_color = color;
	}
	void SetRotation(const float rotation)
	{
		m_rotation = rotation;
	}
	void SetScale(const float scale)
	{
		m_scale = scale;
	}
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
	}


	void SetPostRenderFlag(const bool flag)
	{
		m_postRenderFlag = flag;
	}

private:
	Font m_font;
	wchar_t m_text[256];
	Vector2 m_position = { 0.0f,0.0f };
	Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f };
	float m_rotation = 0.0f;
	float m_scale = 1.0f;
	Vector2 m_pivot = { 0.0f,0.0f };

	bool m_postRenderFlag = false;
};

