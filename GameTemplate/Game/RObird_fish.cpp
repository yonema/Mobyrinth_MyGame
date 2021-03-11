#include "stdafx.h"
#include "RObird_fish.h"

bool RObird_fish::StartSub()
{
	Init("Assets/modelData/bird.tkm", enBird,
		"Assets/modelData/fish.tkm", enFish);

	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_otherModelRender[i] = NewGO<CModelRender>(0);
	}
	m_otherModelRender[enFront]->Init("Assets/modelData/grilled chicken.tkm");
	m_otherModelRender[enBack]->Init("Assets/modelData/grilled fish.tkm");
	m_reversibleType[enFront] = enGrilledChicken;
	m_reversibleType[enBack] = enGrilledFish;
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_otherModelRender[i]->Deactivate();
	}
	return true;
}

void RObird_fish::QuerySub()
{
	const float hitLen = 100.0f;

	if (GetObjectType() == enBird)
	{
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{
				Vector3 diff = m_position - bigFire->GetPosition();
				if (diff.Length() <= hitLen)
				{
					SetBothModelActiveFlag(false);
					m_otherModelRender[GetFrontOrBack()]->Activate();
				}
				return true;
			}
		);
	}
	else if (GetObjectType() == enFish)
	{
		QueryLOs<OObigFire>(enBigFire, [&](OObigFire* bigFire) -> bool
			{

				Vector3 diff = m_position - bigFire->GetPosition();
				if (diff.Length() <= hitLen)
				{
					SetBothModelActiveFlag(false);
					m_otherModelRender[GetFrontOrBack()]->Activate();
				}
				return true;
			}
		);
	}
}

void RObird_fish::UpdateSub()
{
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_otherModelRender[i]->SetPosition(m_position);
		m_otherModelRender[i]->SetRotation(m_rotation);
		m_otherModelRender[i]->SetScale(m_scale);
	}
}

void RObird_fish::SetFrontOrBackSub()
{
	SetObjectType(m_reversibleType[GetFrontOrBack()]);
	if (m_otherModelRender[GetFrontOrBack()])
		m_otherModelRender[GetFrontOrBack()]->Activate();
	if (m_otherModelRender[!GetFrontOrBack()])
		m_otherModelRender[!GetFrontOrBack()]->Deactivate();
}