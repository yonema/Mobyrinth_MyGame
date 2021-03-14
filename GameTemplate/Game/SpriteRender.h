#pragma once

class CSpriteRender : public IGameObject
{
public:
	bool Start()override final;
	~CSpriteRender();
	void Init(const char* texFilePath, float w, float h);
	void Update()override final;
	void Render(RenderContext& rc)override final;

	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
private:
	Sprite m_sprite;
	SpriteInitData m_initData;



	Vector3 m_position = g_vec3Zero;
	Quaternion m_rotation = g_quatIdentity;
	Vector3 m_scale = g_vec3One;
	Vector2 m_pivot = { 0.5f,0.5f };
};

