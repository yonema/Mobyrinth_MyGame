#include "stdafx.h"
#include "Player_kari.h"
#include "LightManager.h"



bool Player_kari::Start()
{
	m_animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimClip_Run].SetLoopFlag(true);
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init
	("Assets/modelData/unityChan.tkm",m_animationClips,enAnimClip_Num, enModelUpAxisY);
	m_modelRender->SetPosition(m_position);

	m_charaCon.Init(50.0f, 10.0f, m_position);


	return true;
}
void Player_kari::Update()
{


	Vector3 moveSpeed;
	moveSpeed.x = g_pad[0]->GetLStickXF() * -1.0f;
	moveSpeed.z = g_pad[0]->GetLStickYF() * -1.0f;
	m_position = m_charaCon.Execute(moveSpeed, 1.0f);

	if (g_pad[0]->IsTrigger(enButtonB))
	{
		DeleteGO(FindGO<CDirectionLight>("directionLight1"));
		
	}
}
