#pragma once
#include "LightManager.h"

class CShadowMap
{
public:
	void Init();
	void Draw(RenderContext& renderContext);
	void CreateShadowMap(const Vector3& direction, const float length);
	ShadowParam* GetShadowParam()
	{
		return &m_shadowParam[0];
	}
	Texture& GetShadowBlur()
	{
		return m_gaussianBlur.GetBokeTexture();
	}
	Texture& GetRenderTargetTex()
	{
		return m_shadowMapRenderTarget.GetRenderTargetTexture();
	}
	void AddShadow(Model& shadowModel);
	void RemoveShadow(Model& shadowModel);
private:
	void InitShadowMapRenderTarget();
	void InitGaussianBlur();
private:

	ShadowParam m_shadowParam[g_max_shadowMap];
	RenderTarget m_shadowMapRenderTarget;
	CGaussianBlur m_gaussianBlur;
	std::list<Model*> m_shadowModels;

};



