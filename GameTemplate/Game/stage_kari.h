#pragma once
#include "level.h"
class CLevel;
class Player_kari;
class background_kari;

class stage_kari : public IGameObject
{
public:
	bool Start() override final;
	~stage_kari();
	void Update() override final;
	void Render(RenderContext& rc)override final;

private:	//データメンバ
	Player_kari* m_Player_kari = nullptr;
	background_kari* m_background_kari = nullptr;
	CLevel m_level;
};

