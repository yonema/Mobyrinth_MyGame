#pragma once
#include "GaussianBlur.h"

class CBloom
{
public:
	CBloom();
	~CBloom();
private:
	void InitluminanceRenderTarget();
	void InitRenderTargetTextrue();
	void InitluminanceSprite();
private:
	RenderTarget luminanceRenderTarget;	//輝度抽出用のレンダリングターゲット
	Texture m_renderTargetTextrue;
	Sprite m_luminanceSprite;
	CGaussianBlur gaussianBlur[4];
};

