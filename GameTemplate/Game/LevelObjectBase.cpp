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



