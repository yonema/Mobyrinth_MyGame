#include "stdafx.h"
#include "LevelObjectBase.h"
//�f�o�b�N�p
//��ŏ���
int ILevelObjectBase::objectNumber = 0;

//�X�^�[�g�֐�
bool ILevelObjectBase::Start()
{
	//�߂��̃E�F�C�|�C���g��T���āA�C�C�����ɉ�]����
	CheckWayPoint();
	//�v���C���[�̎Q�Ƃ�ێ�����
	m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();

	//LevelObjectManager�ɃI�u�W�F�N�g���ǉ����ꂽ���Ƃ��
	CLevelObjectManager::GetInstance()->AddObject(this);

	//OBB������������
	InitOBB();

	//�f�o�b�N�p
	//��ŏ����B
	m_objectNumber = objectNumber++;

	Vector3* vertPos = m_obb.GetBoxVertex();
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		m_dbgOBBVert[i] = NewGO<CModelRender>(0);
		m_dbgOBBVert[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgOBBVert[i]->SetPosition(vertPos[i]);
	}

	//�f�o�b�N�p�����܂�

	//�I�[�o�[���C�h���Ăق����֐�PureVirtualStart()
	return PureVirtualStart();

}

ILevelObjectBase::~ILevelObjectBase()
{
	//�f�o�b�N�p
	//��ŏ���
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		DeleteGO(m_dbgOBBVert[i]);
	}
	CLevelObjectManager::GetInstance()->RemoveObject(this);
}

void ILevelObjectBase::InitOBB()
{
	//OBB�̏������f�[�^
	SInitOBBData initData;
	initData.position = m_position;
	initData.width = 100.0f;
	initData.height = 100.0f;
	initData.length = 100.0f;
	initData.pivot = { 0.5f,0.0f,0.5f };
	initData.rotation = m_rotation;

	//OBB�̏�����
	m_obb.Init(initData);
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


bool ILevelObjectBase::IsHitPlayer()
{
	//�v���C���[���������Ă��Ȃ������牽��������return
	if (!m_pPlayer)
		return false;

	//OBB���m�̓����蔻������āA
	//���̌��ʂ�߂�
	return CollisionOBBs(m_pPlayer->GetOBB(), m_obb);


}
bool ILevelObjectBase::IsHitObject
(ILevelObjectBase& lhs, ILevelObjectBase& rhs)
{
	//OBB���m�̓����蔻������āA
	//���̌��ʂ�߂�
	return CollisionOBBs(lhs.GetOBB(), rhs.GetOBB());
}

void ILevelObjectBase::Update()
{
	PureVirtualUpdate();

	m_obb.SetPosition(m_position);
	m_obb.SetRotation(m_rotation);

	//�f�o�b�N�p
	//��ŏ���
	Vector3* vertPos = m_obb.GetBoxVertex();
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		m_dbgOBBVert[i]->SetPosition(vertPos[i]);
		m_dbgOBBVert[i]->SetRotation(m_rotation);
	}
	//�f�o�b�N�p�����܂�
}