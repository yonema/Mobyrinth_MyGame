#pragma once
#include "Bloom.h"


class CPostEffect
{
public:
	CPostEffect();
	~CPostEffect();
public:
	void Init();
	void Draw(RenderContext& renderContext);
private:
	CBloom m_bloom;

};

