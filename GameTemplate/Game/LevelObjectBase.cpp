#include "stdafx.h"
#include "LevelObjectBase.h"
int ILevelObjectBase::objectNumber = 0;
bool ILevelObjectBase::Start()
{
	CheckWayPoint();
	m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();
	CLevelObjectManager::GetInstance()->AddObject(this);
	m_objectNumber = objectNumber++;
	return PureVirtualStart();
}

ILevelObjectBase::~ILevelObjectBase()
{
	CLevelObjectManager::GetInstance()->RemoveObject(this);
}

void ILevelObjectBase::CheckWayPoint()
{
	std::vector<Vector3>* wayPointPosVec
		= CLevelObjectManager::GetInstance()->GetWayPointPos();
	std::vector<Quaternion>* wayPointRotVec
		= CLevelObjectManager::GetInstance()->GetWayPointRot();
	float dist = FLT_MAX;
	int index = 0;
	int vecSize = (*wayPointPosVec).size();

	//��ԋ߂��E�F�C�|�C���g�𒲂ׂ�
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
	//��ԋ߂��E�F�C�|�C���g�̍��E�̃C���f�b�N�X�𒲂ׂ�
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

	//���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 lpToRpLen = (*wayPointPosVec)[rpIndex] - (*wayPointPosVec)[lpIndex];

	//���̃E�F�C�|�C���g����v���C���[�ւ̃x�N�g��
	Vector3 lpToPosLen = m_position - (*wayPointPosVec)[lpIndex];

	//�⊮��
	float ComplementRate = lpToPosLen.Length() / lpToRpLen.Length();

	//���ʐ��`�⊮
	m_rotation.Slerp(ComplementRate, (*wayPointRotVec)[lpIndex], (*wayPointRotVec)[rpIndex]);


}


bool ILevelObjectBase::IsHitPlayer(const float hitDot)
{
	if (!m_pPlayer)
		return false;


	Vector3 myUp = g_vec3Up;
	m_rotation.Apply(myUp);
	myUp.Normalize();
	
	Vector3 playerUp = m_pPlayer->GetUpVec();
	playerUp.Normalize();

	if (std::abs(Dot(myUp, playerUp) - 1.0f) <= hitDot)
	{
		//��������
		return true;
	}
	else
	{
		//�������ĂȂ�
		return false;
	}

}
bool ILevelObjectBase::IsHitObject
(const ILevelObjectBase& lhs, const ILevelObjectBase& rhs, const float hitDot)
{
	Quaternion qRot;

	Vector3 lhsUp = g_vec3Up;
	qRot = lhs.GetRotation();
	qRot.Apply(lhsUp);
	lhsUp.Normalize();

	Vector3 rhsUp = g_vec3Up;
	qRot = rhs.GetRotation();
	qRot.Apply(rhsUp);
	rhsUp.Normalize();

	if (std::abs(Dot(lhsUp, rhsUp) - 1.0f) <= hitDot)
	{
		//��������
		return true;
	}
	else
	{
		//�������ĂȂ�
		return false;
	}

}