#pragma once
#include "stage_kari.h"
#include "stage_proto01.h"

class Game : public IGameObject
{
public:
	bool Start()override final;
	~Game();
	void PostRender(RenderContext& rc)override final;
private:
	int m_numText = 0;

};

