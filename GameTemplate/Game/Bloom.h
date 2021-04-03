#pragma once
#include "GaussianBlur.h"

class CBloom
{
public:
	CBloom();
	~CBloom();
public:
	void Init();
	void Draw(RenderContext& renderContext);
	void DrawToMainRenderTarget(RenderContext& renderContext);
private:
	void InitluminanceRenderTarget();
	void InitluminanceSprite();
	void InitGaussianBlur();
private:
	RenderTarget m_luminanceRenderTarget;	//輝度抽出用のレンダリングターゲット
	Sprite m_luminanceSprite;
	CGaussianBlur m_gaussianBlur[4];
	Sprite m_finalSprite;

};

