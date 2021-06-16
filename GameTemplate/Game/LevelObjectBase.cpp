#include "stdafx.h"
#include "LevelObjectBase.h"
//�f�o�b�N�p
#ifdef MY_DEBUG
//��ŏ���
int ILevelObjectBase::objectNumber = 0;
#endif


//�X�^�[�g�֐�
bool ILevelObjectBase::Start()
{
	//�߂��̃E�F�C�|�C���g��T���āA�C�C�����ɉ�]����
	CheckWayPoint();
	//�v���C���[�̎Q�Ƃ�ێ�����
	m_player = CLevelObjectManager::GetInstance()->GetPlayer();

	//LevelObjectManager�ɃI�u�W�F�N�g���ǉ����ꂽ���Ƃ��
	CLevelObjectManager::GetInstance()->AddObject(this);

	//OBB������������
	InitOBB();


#ifdef MY_DEBUG
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
#endif
	//�I�[�o�[���C�h���Ăق����֐�PureVirtualStart()
	return PureVirtualStart();

}

//�f�X�g���N�^
ILevelObjectBase::~ILevelObjectBase()
{
	//m_swichon�폜
	DeleteGO(m_swichonEffect);

	//�t�H���g�̃^�C�}�[�̔j��
	if (m_timerFR)
		DeleteGO(m_timerFR);

#ifdef MY_DEBUG
	//�f�o�b�N�p
	//��ŏ���
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		DeleteGO(m_dbgOBBVert[i]);
	}
	//�f�o�b�N�p�����܂�
#endif

	//LevelObjectManager�ɃI�u�W�F�N�g���j�����ꂽ���Ƃ�`����
	CLevelObjectManager::GetInstance()->RemoveObject(this);
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
#ifdef MY_DEBUG
	//�f�o�b�N�p
	//��ŏ���
	Vector3* vertPos = m_obb.GetBoxVertex();
	for (int i = 0; i < m_dbgOBBNum; i++)
	{
		m_dbgOBBVert[i]->SetPosition(vertPos[i]);
		m_dbgOBBVert[i]->SetRotation(m_rotation);
	}
	//�f�o�b�N�p�����܂�
#endif
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
void ILevelObjectBase::CheckWayPoint(const bool checkRotaton, const bool checkPosition)
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


	m_lpIndex = lpIndex;
	m_rpIndex = rpIndex;
	
	if (checkRotaton)
		CheckRotation();
	if (checkPosition)
		CheckPosition();
}

void ILevelObjectBase::CheckRotation()
{
	//LevelObjectManager����E�F�C�|�C���g�̏��������Ă���
	//�E�F�C�|�C���g�́u�ꏊ�v�������Ă���
	std::vector<Vector3>* wayPointPosVec
		= CLevelObjectManager::GetInstance()->GetWayPointPos();
	//�E�F�C�|�C���g�́u��]�v�������Ă���
	std::vector<Quaternion>* wayPointRotVec

		= CLevelObjectManager::GetInstance()->GetWayPointRot();
	//���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 lpToRpLen = (*wayPointPosVec)[m_rpIndex] - (*wayPointPosVec)[m_lpIndex];

	//���̃E�F�C�|�C���g���玩�g�ւ̃x�N�g��
	Vector3 lpToPosLen = m_position - (*wayPointPosVec)[m_lpIndex];

	//���g�����E�̃E�F�C�|�C���g�̊Ԃ̂ǂꂭ�炢�̈ʒu�ɂ��邩��
	//�⊮�����v�Z����
	float ComplementRate = lpToPosLen.Length() / lpToRpLen.Length();

	//���ʐ��`�⊮
	//���g���ĉ�]������
	m_rotation.Slerp(ComplementRate, (*wayPointRotVec)[m_lpIndex], (*wayPointRotVec)[m_rpIndex]);

}

void ILevelObjectBase::CheckPosition()
{
	Vector3 onWayPosition;
	//LevelObjectManager����E�F�C�|�C���g�̏��������Ă���
	//�E�F�C�|�C���g�́u�ꏊ�v�������Ă���
	const std::vector<Vector3>* wayPointPosVec
		= CLevelObjectManager::GetInstance()->GetWayPointPos();
	//���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
	Vector3 lpToRpVec = (*wayPointPosVec)[m_rpIndex] - (*wayPointPosVec)[m_lpIndex];
	lpToRpVec.Normalize();
	//���̃E�F�C�|�C���g���玩�g�ւ̃x�N�g��
	const Vector3 lpToPosVec = m_position - (*wayPointPosVec)[m_lpIndex];

	const float projectionLen = Dot(lpToRpVec, lpToPosVec);

	Vector3 correctionPos = lpToRpVec;
	correctionPos.Scale(projectionLen);
	correctionPos += (*wayPointPosVec)[m_lpIndex];

	Vector3 zVec = g_VEC3_BACK;
	m_rotation.Apply(zVec);
	zVec.Scale(m_zPosLen);
	correctionPos += zVec;

	m_position = correctionPos;

}

/// <summary>
/// ���g���\���ɂ��邩�����ɂ��邩�𒲂ׂ�֐�
/// </summary>
/// <param name="reversibleObject">���]�I�u�W�F�N�g���H</param>
void ILevelObjectBase::CheckFrontOrBackSide(const bool reversibleObject)
{
	//�\����������
	int nextFrontOrBackSide = CLevelObjectManager::GetInstance()->CheckFrontOrBackSide(m_lpIndex);


	if (reversibleObject)
	{
		//�����l�ł͂Ȃ�������
		if (m_frontOrBackSide != EB_NONE_SIDE)
		{
			//�O�̏ꏊ�̔��]�I�u�W�F�N�g�̐������Z����
			CLevelObjectManager::GetInstance()->RemoveReversibleObjectNum(m_frontOrBackSide);
		}

		//���̏ꏊ�̔��]�I�u�W�F�N�g�̐������Z����
		CLevelObjectManager::GetInstance()->AddReversibleObjectNum(nextFrontOrBackSide);
	}
		//���݂̕\�������������X�V����
	m_frontOrBackSide = nextFrontOrBackSide;
}

/// <summary>
/// ���g�ƃv���C���[�̓����蔻��
/// </summary>
/// <returns>true���߂��Ă����瓖�����Ă���</returns>
bool ILevelObjectBase::IsHitPlayer()
{
	//�v���C���[���������Ă��Ȃ������牽��������return
	if (!m_player)
		return false;

	//OBB���m�̓����蔻������āA
	//���̌��ʂ�߂�
	return CollisionOBBs(m_player->GetOBB(), m_obb);
}


/// <summary>
/// �����I�u�W�F�N�g�Ɏg�p����f�[�^������������B
/// </summary>
void ILevelObjectBase::SetTransparentObject()
{
	//�����I�u�W�F�N�g����Ɏg�p����t���O��true�ɂ���B
	m_transparentObjectFlag = true;
	//�I�u�W�F�N�g�̏d�Ȃ��Ă��锻����s��Ȃ��悤�ɂ���B
	m_isHitFlag = false;



	//�^�C�}�[�̃t�H���g�����_���[�̐����Ə�����
	m_timerFR = NewGO<CFontRender>(0);
	m_timerFR->Init(L"10", { 0.0f,0.0f });
	m_timerFR->SetPostRenderFlag(true);
	//��\���ɂ���
	m_timerFR->Deactivate();

	m_swichonEffect = NewGO<Effect>(0);
	m_swichonEffect->Init(u"Assets/effect2/activation.efk");
	float scale = 70.0f;								//�������̂ő傫�����Ă���
	m_swichonEffect->SetScale({ scale ,scale ,scale });


	m_swichoffEffect = NewGO<Effect>(0);
	m_swichoffEffect->Init(u"Assets/effect2/activation.efk");
	float scale2 = 70.0f;								//�������̂ő傫�����Ă���
	m_swichoffEffect->SetScale({ scale2 ,scale2 ,scale2 });
}


/// <summary>
/// �����X�C�b�`�������ꂽ�Ƃ��Ɏg�p�����֐�
/// </summary>
void ILevelObjectBase::TransparentSwitchOn()
{
	m_swichonEffect->SetPosition(m_position);
	m_swichonEffect->SetRotation(m_rotation);
	m_swichonEffect->Play();

	//�I�u�W�F�N�g�̏Փ˔�����s���悤�ɂ���B
	m_isHitFlag = true;

	//�I�u�W�F�N�g�̓����蔻���L���ɂ���B
	m_obb.SetExceptionFlag(false);


	//�^�C�}�[�̃t�H���g��\������
	m_timerFR->Activate();

	m_swichonEffect->SetPosition(m_position);
	m_swichonEffect->SetRotation(m_rotation);
	m_swichonEffect->Play();

	ReversibleSwitchOn();
}


/// <summary>
/// �����X�C�b�`�̌��ʂ��������Ƃ��Ɏg�p�����֐�
/// </summary>
void ILevelObjectBase::TransparentSwitchOff()
{
	//�X�C�b�`���L�����H
	if (m_switchValid)
	{
		//�L��

		//�G�t�F�N�g���Đ�����
		m_swichoffEffect->SetPosition(m_position);
		m_swichoffEffect->SetRotation(m_rotation);
		m_swichoffEffect->Play();
	}
	else
	{
		//�L������Ȃ�

		//�L���ɂ���
		m_switchValid = true;
	}

	//�I�u�W�F�N�g�̏Փ˔�����s��Ȃ��悤�ɂ���B
	m_isHitFlag = false;

	CheckWayPoint();
	CheckFrontOrBackSide();
	//�I�u�W�F�N�g�̓����蔻��𖳌��ɂ���B
	m_obb.SetExceptionFlag(true);

	//�^�C�}�[�̃t�H���g���\���ɂ���
	m_timerFR->Deactivate();


	ReversibleSwitchOff();
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
	//�I�u�W�F�N�g�̏Փ˔�����s��Ȃ���Ԃ̏ꍇ
	if (lhs.GetIsHitFlag() == false || rhs.GetIsHitFlag() == false) {
		//�Փ˂��Ă��Ȃ����肷��B
		return false;
	}
	if (lhs.GetFrontOrBackSide() != rhs.GetFrontOrBackSide())
		return false;

	//OBB���m�̓����蔻������āA
	//���̌��ʂ�߂�
	return CollisionOBBs(lhs.GetOBB(), rhs.GetOBB());
}