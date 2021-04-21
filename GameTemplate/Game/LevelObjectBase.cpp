#include "stdafx.h"
#include "LevelObjectBase.h"
//�f�o�b�N�p
//��ŏ���
//int ILevelObjectBase::objectNumber = 0;

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
	//m_objectNumber = objectNumber++;

	//Vector3* vertPos = m_obb.GetBoxVertex();
	//for (int i = 0; i < m_dbgOBBNum; i++)
	//{
	//	m_dbgOBBVert[i] = NewGO<CModelRender>(0);
	//	m_dbgOBBVert[i]->Init("Assets/modelData/dbgBox.tkm");
	//	m_dbgOBBVert[i]->SetPosition(vertPos[i]);
	//}

	//�f�o�b�N�p�����܂�

	//�I�[�o�[���C�h���Ăق����֐�PureVirtualStart()
	return PureVirtualStart();

}

//�f�X�g���N�^
ILevelObjectBase::~ILevelObjectBase()
{
	//�f�o�b�N�p
	//��ŏ���
	//for (int i = 0; i < m_dbgOBBNum; i++)
	//{
	//	DeleteGO(m_dbgOBBVert[i]);
	//}
	//�f�o�b�N�p�����܂�

	//LevelObjectManager�ɃI�u�W�F�N�g���j�����ꂽ���Ƃ�`����
	CLevelObjectManager::GetInstance()->RemoveObject(this);
}

/// <summary>
/// OBB�̏������֐�
/// </summary>
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

/// <summary>
/// �߂��̃E�F�C�|�C���g��T���āA�C�C�����ɉ�]����֐�
/// </summary>
void ILevelObjectBase::CheckWayPoint()
{
	//LevelObjectManager����E�F�C�|�C���g�̏��������Ă���
	//�E�F�C�|�C���g�́u�ꏊ�v�������Ă���
	std::vector<Vector3>* wayPointPosVec
		= CLevelObjectManager::GetInstance()->GetWayPointPos();
	//�E�F�C�|�C���g�́u��]�v�������Ă���
	std::vector<Quaternion>* wayPointRotVec
		= CLevelObjectManager::GetInstance()->GetWayPointRot();


	//��ԋ߂��E�F�C�|�C���g�𒲂ׂ�

	//��ԋ߂������𒲂ׂ�悤�̕ϐ�
	//FLT_MAX�ifloat�^�̑傫�����j���ŏ��ɓ���Ă���
	float dist = FLT_MAX;
	//��ԋ߂��E�F�C�|�C���g�̃x�N�^�[�̃C���f�b�N�X��ێ����Ă����p�̕ϐ�
	int index = 0;
	//�E�F�C�|�C���g�̃x�N�^�[�̃T�C�Y��ێ�
	int vecSize = (*wayPointPosVec).size();

	//��ԋ߂��E�F�C�|�C���g�𒲂ׂ�
	for (int i = 0; i < vecSize; i++)
	{
		//�E�F�C�|�C���g�Ǝ��g�̊Ԃ̃x�N�g�����v�Z����
		Vector3 lenVec = (*wayPointPosVec)[i] - m_position;
		//�E�F�C�|�C���g�Ǝ��M�̊Ԃ̒������v�Z����
		float distTmp = lenVec.Length();

		//dist���߂������Ȃ�
		if (distTmp < dist)
		{
			//��ԋ߂��E�F�C�|�C���g�̃x�N�^�[�̃C���f�b�N�X��
			//���݂̌J��Ԃ��񐔂�ێ������āA
			index = i;
			//dist�ɂ��̋�����ێ�������
			dist = distTmp;
		}
	}

	//���g�̍��̃E�F�C�|�C���g�̃x�N�^�[�̃C���f�b�N�X
	int lpIndex;
	//���g�̉E�̃E�F�C�|�C���g�̃x�N�^�[�̃C���f�b�N�X
	int rpIndex;

	//��ԋ߂��E�F�C�|�C���g�̍��E�̃C���f�b�N�X�𒲂ׂ�
	{
		//��ԋ߂��E�F�C�|�C���g�̉E�̃C���f�b�N�X
		int right = index - 1;
		//��ԋ߂��E�F�C�|�C���g�̍��̃C���f�b�N�X
		int left = index + 1;

		//0��菬����������
		if (right < 0)
		{
			//���������
			right = vecSize;
			//�C���f�b�N�X�̍ő�l�̓x�N�^�[�̃T�C�Y���1����������-1����
			right -= 1;
		}
		//�x�N�^�[�̃T�C�Y�ȏゾ������
		//�C���f�b�N�X�̍ő�l�̓x�N�^�[�̃T�C�Y���1����������
		//�@>=�@�Ŕ�r����
		if (left >= vecSize)
		{
			//���������
			left = 0;
		}

		//��ԋ߂��E�F�C�|�C���g�̍��̃E�F�C�|�C���g�ƁA���g�Ƃ̊Ԃ̃x�N�g��
		Vector3 leftLen = (*wayPointPosVec)[left] - m_position;
		//��ԋ߂��E�F�C�|�C���g�̉E�̃E�F�C�|�C���g�ƁA���g�Ƃ̊Ԃ̃x�N�g��
		Vector3 rightLen = (*wayPointPosVec)[right] - m_position;

		//�E�ƍ��̋������ǂ��炪�߂�����r����
		if (leftLen.Length() <= rightLen.Length())
		{
			//���̕����߂��Ȃ�
			//���̃C���f�b�N�X���A��ԋ߂��E�F�C�|�C���g�̍��̃E�F�C�|�C���g
			//�Ɍ��肷��
			lpIndex = left;
			//�E�̃C���f�b�N�X���A��ԋ߂��E�F�C�|�C���g
			//�Ɍ��肷��
			rpIndex = index;
		}
		else
		{
			//�E�̕����߂��Ȃ�
			//�݂��̃C���f�b�N�X���A��ԋ߂��E�F�C�|�C���g�̉E�̃E�F�C�|�C���g
			//�Ɍ��肷��
			rpIndex = right;
			//���̃E�F�C�|�C���g���A��ԋ߂��E�F�C�|�C���g
			//�Ɍ��肷��
			lpIndex = index;
		}
	}

	//���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 lpToRpLen = (*wayPointPosVec)[rpIndex] - (*wayPointPosVec)[lpIndex];

	//���̃E�F�C�|�C���g����v���C���[�ւ̃x�N�g��
	Vector3 lpToPosLen = m_position - (*wayPointPosVec)[lpIndex];

	//���g�����E�̃E�F�C�|�C���g�̊Ԃ̂ǂꂭ�炢�̈ʒu�ɂ��邩��
	//�⊮�����v�Z����
	float ComplementRate = lpToPosLen.Length() / lpToRpLen.Length();

	//���ʐ��`�⊮
	//���g���ĉ�]������
	m_rotation.Slerp(ComplementRate, (*wayPointRotVec)[lpIndex], (*wayPointRotVec)[rpIndex]);

}



//�A�b�v�f�[�g�֐�
void ILevelObjectBase::Update()
{
	//�I�[�o�[���C�h�����֐�
	//�h���N���X�̃A�b�v�f�[�g�֐������ɌĂ΂��
	PureVirtualUpdate();

	//OBB�̏ꏊ�Ɖ�]��ݒ肷��
	m_obb.SetPosition(m_position);
	m_obb.SetRotation(m_rotation);

	//�f�o�b�N�p
	//��ŏ���
	//Vector3* vertPos = m_obb.GetBoxVertex();
	//for (int i = 0; i < m_dbgOBBNum; i++)
	//{
	//	m_dbgOBBVert[i]->SetPosition(vertPos[i]);
	//	m_dbgOBBVert[i]->SetRotation(m_rotation);
	//}
	//�f�o�b�N�p�����܂�
}


/// <summary>
/// ���g�ƃv���C���[�̓����蔻��
/// </summary>
/// <returns>true���߂��Ă����瓖�����Ă���</returns>
bool ILevelObjectBase::IsHitPlayer()
{
	//�v���C���[���������Ă��Ȃ������牽��������return
	if (!m_pPlayer)
		return false;

	//OBB���m�̓����蔻������āA
	//���̌��ʂ�߂�
	return CollisionOBBs(m_pPlayer->GetOBB(), m_obb);


}



/// <summary>
/// OBB���m�̓����蔻��
/// </summary>
/// <param name="lhs">���x���I�u�W�F�N�g1</param>
/// <param name="rhs">���x���I�u�W�F�N�g2</param>
/// <returns>true���߂��Ă����瓖�����Ă���</returns>
bool IsHitObject
(ILevelObjectBase& lhs, ILevelObjectBase& rhs)
{
	//OBB���m�̓����蔻������āA
	//���̌��ʂ�߂�
	return CollisionOBBs(lhs.GetOBB(), rhs.GetOBB());
}