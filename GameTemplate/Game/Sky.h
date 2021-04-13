#pragma once
#include "ModelRender.h"
/// <summary>
/// ‹óƒNƒ‰ƒX
/// </summary>
class CSky : public IGameObject
{
public:
	CSky();
	~CSky();
	bool Start()override final;
	void Update()override final;
	void Render(RenderContext& rc)override final;
public:
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		m_isDirty = true;
	}

	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		m_isDirty = true;
	}

	void SetScale(const float scale)
	{
		m_scale = g_vec3One;
		m_scale.Scale(scale);
		m_isDirty = true;
	}

	void SetEmissionColor(const Vector4 color)
	{
		m_model.SetEmissionColor(color);
		//m_selfLuminous = color;
		m_isDirty = true;
	}

private:
	Model m_model;
	//CModelRender* m_modelRender = nullptr;
	Texture m_texture;
	Vector3 m_position = g_vec3Zero;
	Vector3 m_scale = g_vec3One;
	Vector4 m_selfLuminous = { 0.0f,0.0f,0.0f,1.0f };
	bool m_isDirty = false;
};

