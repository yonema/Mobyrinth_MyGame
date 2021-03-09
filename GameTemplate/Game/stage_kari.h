#pragma once
#include "level.h"
#include "font/Font.h"
#include "ModelRender.h"
#include "GameCamera.h"
#include "Mobius.h"
#include "LOmizu_kori.h"

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
	void PostRender(RenderContext& rc)override final;

private:	//データメンバ
	Player_kari* m_Player_kari = nullptr;
	background_kari* m_background_kari = nullptr;
	CLevel m_level;

	CModelRender* m_modelRender = nullptr;
	int m_numText = 0;

};

