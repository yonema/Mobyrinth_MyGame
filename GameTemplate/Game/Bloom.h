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
	RenderTarget luminanceRenderTarget;	//�P�x���o�p�̃����_�����O�^�[�Q�b�g
	Texture m_renderTargetTextrue;
	Sprite m_luminanceSprite;
	CGaussianBlur gaussianBlur[4];
};

