#include "stdafx.h"
#include "ReversibleObject.h"



bool CReversibleObject::Init
(const char* filePath_front, const int type_front,
	const char* filePath_back, const int type_back)
{
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_modelRender[i] = NewGO<CModelRender>(0);
	}
	m_modelRender[enFront]->Init(filePath_front);
	m_modelRender[enBack]->Init(filePath_back);
	m_reversibleType[enFront] = type_front;
	m_reversibleType[enBack] = type_back;
	SetObjectType(m_reversibleType[m_frontOrBack]);
	SetFrontOrBack(m_frontOrBack);
	return true;
}
bool CReversibleObject::PureVirtualStart()
{
	CheckWayPoint();

	return StartSub();
}

void CReversibleObject::Reverse()
{
	SetFrontOrBack(!m_frontOrBack);
}
void CReversibleObject::SetFrontOrBack(bool frontOrBack)
{
	m_frontOrBack = frontOrBack;
	if (m_bothModelactiveFlag)
	{
		SetObjectType(m_reversibleType[m_frontOrBack]);
		if (m_modelRender[m_frontOrBack])
			m_modelRender[m_frontOrBack]->Activate();
		if (m_modelRender[!m_frontOrBack])
			m_modelRender[!m_frontOrBack]->Deactivate();
	}
	else
	{
		SetFrontOrBackSub();
	}
}
void CReversibleObject::SetBothModelActiveFlag(const bool activeFlag)
{
	m_bothModelactiveFlag = activeFlag;


	if (m_bothModelactiveFlag)
	{
		if (m_modelRender[m_frontOrBack])
			m_modelRender[m_frontOrBack]->Activate();
		if (m_modelRender[!m_frontOrBack])
			m_modelRender[!m_frontOrBack]->Deactivate();
	}
	else
	{
		m_modelRender[enFront]->Deactivate();
		m_modelRender[enBack]->Deactivate();
	}
	
}

void CReversibleObject::Update()
{
	if (!m_pPlayer)
	{
		m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();
		if (!m_pPlayer)
			return;
	}


	switch (m_objectState)
	{
	case enCheckPlayer:
		CheckPlayer();
		break;
	case enHeldPlayer:
		HeldPlayer();
		break;
	case enThrown:
		Thrown();
		break;
	case enQuery:
		Query();
		break;
	}

	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_modelRender[i]->SetPosition(m_position);
		m_modelRender[i]->SetRotation(m_rotation);
		m_modelRender[i]->SetScale(m_scale);
	}

	UpdateSub();

	return;
}

CReversibleObject::~CReversibleObject()
{
	Delete();
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		DeleteGO(m_modelRender[i]);
	}

}



void CReversibleObject::CheckPlayer()
{

	if (IsHitPlayer())
	{
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			if (!m_pPlayer->GetHoldObject())
			{
				m_objectState = enHeldPlayer;
				m_pPlayer->SetHoldObject(true);
			}
		}
	}
}

void CReversibleObject::HeldPlayer()
{
	Vector3 pos = m_pPlayer->GetPosition();
	Quaternion qRot = m_pPlayer->GetFinalWPRot();
	Vector3 up = g_vec3Up;
	up.Scale(100.0f);
	qRot.Apply(up);
	pos += up;
	m_position = pos;
	m_rotation = qRot;

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_objectState = enThrown;
		m_throwRot = m_pPlayer->GetFinalWPRot();
	}
}

void CReversibleObject::Thrown()
{
	Vector3 dir = g_vec3Down;
	m_throwRot.Apply(dir);
	dir.Scale(7.0f);
	m_position += dir;
	m_rotation.SetRotationDegX(180.0f * m_throwCounter / 24);
	m_rotation.Multiply(m_throwRot);
	m_throwCounter++;
	if (m_throwCounter >= 24)
	{
		m_rotation.SetRotationDegX(180.0f);
		m_rotation.Multiply(m_throwRot);
		m_objectState = enQuery;
		m_pPlayer->SetHoldObject(false);
		m_throwCounter = 0;
	}
	else if (m_throwCounter == 12)
	{
		Reverse();
	}
}


void CReversibleObject::Query()
{
	QuerySub();
	m_objectState = enCheckPlayer;
}
