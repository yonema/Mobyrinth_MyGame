#include "stdafx.h"
#include "OOgoal.h"


bool OOgoal::StartSub()
{
	Init("Assets/modelData/goal.tkm", enGoal);

	m_fontRender = NewGO<CFontRender>(0);
	m_fontRender->Init(L"ゴール！",
		{-200.0f,100.0f},
		{1.0f,1.0f,0.0f,1.0f},
		0.0f,
		3.0f
		);
	m_fontRender->SetPostRenderFlag(true);
	m_fontRender->Deactivate();

	//OBBWorldに自身のOBBの登録を消去させる
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	return true;
}

OOgoal::~OOgoal()
{
	DeleteGO(m_fontRender);
}

void OOgoal::UpdateSub()
{
	if (IsHitPlayer())
	{
		m_isGoal = true;
		m_fontRender->Activate();
	}

}

