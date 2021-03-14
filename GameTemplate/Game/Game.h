#pragma once
#include "Title.h"

#include "SpriteRender.h"

class Game : public IGameObject
{
public:
	bool Start()override final;
	~Game();
	void PostRender(RenderContext& rc)override final;

	void Update()override final;
private:
	int m_numText = 0;
	CSpriteRender* m_spriteRender = nullptr;

	std::vector<ILevelObjectBase*> m_lb;

};

