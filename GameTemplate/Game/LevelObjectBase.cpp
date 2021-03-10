#include "stdafx.h"
#include "LevelObjectBase.h"

bool CLevelObjectBase::Init
(const char* filePath_front, const int type_front,
	const char* filePath_back, const int type_back)
{
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_modelRender[i] = NewGO<CModelRender>(0);
	}
	m_modelRender[enFront]->Init(filePath_front);
	m_modelRender[enBack]->Init(filePath_back);
	m_objectType[enFront] = type_front;
	m_objectType[enBack] = type_back;

	return true;
}
bool CLevelObjectBase::Start()
{
	StartSub();
	SetFrontOrBack(m_frontOrBack);
	CheckWayPoint();
	m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();

	CLevelObjectManager::GetInstance()->AddObject(this);

	return true;
}

void CLevelObjectBase::SetFrontOrBack(bool frontOrBack)
{
	m_frontOrBack = frontOrBack;
	if (m_modelRender[m_frontOrBack])
	m_modelRender[m_frontOrBack]->Activate();
	if (m_modelRender[!m_frontOrBack])
	m_modelRender[!m_frontOrBack]->Deactivate();

}

void CLevelObjectBase::Update()
{
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
	}

	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_modelRender[i]->SetPosition(m_position);
		m_modelRender[i]->SetRotation(m_rotation);
	}



	return;
}

CLevelObjectBase::~CLevelObjectBase()
{
	m_isDead = true;
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		DeleteGO(m_modelRender[i]);
	}

	CLevelObjectManager::GetInstance()->RemoveObject(this);
}

void CLevelObjectBase::CheckWayPoint()
{
	std::vector<Vector3>* wayPointPosVec
		= CLevelObjectManager::GetInstance()->GetWayPointPos();
	std::vector<Quaternion>* wayPointRotVec
		= CLevelObjectManager::GetInstance()->GetWayPointRot();
	float dist = FLT_MAX;
	int index = 0;
	int vecSize = (*wayPointPosVec).size();

	//一番近いウェイポイントを調べる
	for (int i = 0; i < vecSize; i++)
	{
		Vector3 lenVec = (*wayPointPosVec)[i] - m_position;
		float distTmp = lenVec.Length();

		if (distTmp < dist)
		{
			index = i;
			dist = distTmp;
		}
	}

	int lpIndex;
	int rpIndex;
	//一番近いウェイポイントの左右のインデックスを調べる
	{
		int right = index - 1;
		int left = index + 1;

		if (right < 0)
		{
			right = vecSize;
			right -= 1;
		}
		if (left >= vecSize)
		{
			left = 0;
		}

		Vector3 leftLen = (*wayPointPosVec)[left] - m_position;
		Vector3 rightLen = (*wayPointPosVec)[right] - m_position;


		if (leftLen.Length() <= rightLen.Length())
		{
			lpIndex = left;
			rpIndex = index;
		}
		else
		{
			rpIndex = right;
			lpIndex = index;
		}
	}

	//左のウェイポイントから右のウェイポイントへのベクトル
	Vector3 lpToRpLen = (*wayPointPosVec)[rpIndex] - (*wayPointPosVec)[lpIndex];

	//左のウェイポイントからプレイヤーへのベクトル
	Vector3 lpToPosLen = m_position - (*wayPointPosVec)[lpIndex];

	//補完率
	float ComplementRate = lpToPosLen.Length() / lpToRpLen.Length();

	//球面線形補完
	m_rotation.Slerp(ComplementRate, (*wayPointRotVec)[lpIndex], (*wayPointRotVec)[rpIndex]);


}

void CLevelObjectBase::CheckPlayer()
{
	
	if (!m_pPlayer)
	{
		m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();
		if (!m_pPlayer)
		{
			return;
		}
	}
	Vector3 toPlayer = m_pPlayer->GetPosition()- m_position;


	const float hitLen = 150.0f;
	if (toPlayer.Length() <= hitLen)
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

void CLevelObjectBase::HeldPlayer()
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
	}
}

void CLevelObjectBase::Thrown()
{
	Vector3 dir = g_vec3Up;
	Quaternion qRot = m_pPlayer->GetFinalWPRot();
	qRot.Apply(dir);
	dir.Scale(-9.0f);
	m_position += dir;
	m_rotation.SetRotationDegX(180.0f * m_throwCounter / 24);
	m_rotation.Multiply(qRot);
	m_throwCounter++;
	if (m_throwCounter >= 24)
	{
		m_rotation.SetRotationDegX(180.0f);
		m_rotation.Multiply(qRot);
		m_objectState = enCheckPlayer;
		m_pPlayer->SetHoldObject(false);
		m_throwCounter = 0;
	}
	else if (m_throwCounter == 12)
	{
		SetFrontOrBack(!m_frontOrBack);
	}
}