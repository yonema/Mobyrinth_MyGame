#include "stdafx.h"
#include "stage_kari.h"
#include "Player_kari.h"
#include "background_kari.h"

bool stage_kari::Start()
{
	m_Player_kari = NewGO<Player_kari>(0);
	m_background_kari = NewGO<background_kari>(0);

	return true;
}
stage_kari::~stage_kari()
{
	DeleteGO(m_Player_kari);
	DeleteGO(m_background_kari);
}
void stage_kari::Update()
{

}
void stage_kari::Render(RenderContext& rc)
{

}
