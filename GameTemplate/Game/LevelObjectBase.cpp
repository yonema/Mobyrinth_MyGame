#include "stdafx.h"
#include "LevelObjectBase.h"

bool ILevelObjectBase::Start()
{
	CheckWayPoint();
	m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();
	CLevelObjectManager::GetInstance()->AddObject(this);
	
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



