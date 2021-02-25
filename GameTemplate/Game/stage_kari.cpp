#include "stdafx.h"
#include "stage_kari.h"
#include "Player_kari.h"
#include "background_kari.h"
#include "DirectionLight.h"

bool stage_kari::Start()
{
	CDirectionLight* d1 = NewGO<CDirectionLight>(0, "directionLight1");
	d1->SetDirection({ 1.0f,1.0f,-1.0f });
	d1->SetColor({ 0.1f,0.1f,0.1f,1.0f });
	//CDirectionLight* d2 = NewGO<CDirectionLight>(0, "directionLight2");
	//d2->SetDirection({ -1.0f,1.0f,-1.0f });
	//d2->SetColor({ 0.0f,0.0f,0.4f,1.0f });
	//CDirectionLight* d3 = NewGO<CDirectionLight>(0, "directionLight3");
	//d3->SetDirection({ -1.0f,1.0f,-1.0f });
	//d3->SetColor({ 0.4f,0.0f,0.0f,1.0f });
	//CDirectionLight* d4 = NewGO<CDirectionLight>(0, "directionLight4");
	//d4->SetDirection({ -1.0f,1.0f,-1.0f });
	//d4->SetColor({ 0.4f,0.0f,0.0f,1.0f });
	//CDirectionLight* d5 = NewGO<CDirectionLight>(0, "directionLight4");
	//d5->SetDirection({ -1.0f,1.0f,-1.0f });
	//d5->SetColor({ 0.4f,0.0f,0.0f,1.0f });
	//m_Player_kari = NewGO<Player_kari>(0);
	//m_background_kari = NewGO<background_kari>(0);


	m_level.Init("Assets/level/level_kari01.tkl", [&](LevelObjectData& objData)
		{
			if (objData.EqualObjectName(L"unityChan") == true)
			{
				Player_kari* pJimen_kari;
				pJimen_kari = NewGO<Player_kari>(0);
				//pJimen_kari->SetPosition(objData.position);
				return true;
			}
			
			
			return false;
		
		});

	g_camera3D->SetPosition({ 0.0f,0.0f,600.0f });
	g_camera3D->SetTarget({ 0.0f,0.0f,0.0f });

	return true;
}
stage_kari::~stage_kari()
{
	//DeleteGO(m_Player_kari);
	//DeleteGO(m_background_kari);
}
void stage_kari::Update()
{

}
void stage_kari::Render(RenderContext& rc)
{
	m_level.Draw(rc);
}
