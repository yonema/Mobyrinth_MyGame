#include "stdafx.h"
#include "ReversibleObject.h"



//�X�^�[�g�֐�
bool CReversibleObject::PureVirtualStart()
{
	//���f���̉�]���A���݂̏ꏊ�ƃC�C�����ɍ��킹��
	CheckWayPoint();

	//�I�[�o�[���C�h���Ăق����֐�StartSub()�͂����ŌĂ΂��B
	return StartSub();
}

//�f�X�g���N�^
CReversibleObject::~CReversibleObject()
{
	//LevelObjectManager�ɂ��̃I�u�W�F�N�g�͏�����I���ē`����B
	Delete();

	//�\�Ɨ��̃��f��������
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		DeleteGO(m_modelRender[i]);
	}

}


/// <summary>
/// �������֐�
/// �ŏ��ɓǂ�łˁBtrue��߂��ĂˁB
/// �\�̃��f���Ƃ��̃^�C�v�A���̃��f���Ƃ��̃^�C�v
/// ��ݒ肷��B
/// �^�C�v�ꗗ��LevelObjectBase.h���Q��
/// </summary>
/// <param name="filePath_front">�\�̃��f����tkm�t�@�C���p�X</param>
/// <param name="type_front">�\�̃^�C�v</param>
/// <param name="filePath_back">���̃��f����tkm�t�@�C���p�X</param>
/// <param name="type_back">���̃^�C�v</param>
/// <returns>true�߂��Ă�</returns>
bool CReversibleObject::Init
(const char* filePath_front, const int type_front,
	const char* filePath_back, const int type_back)
{
	//�\�Ɨ��̕��A���f�������_���[�𐶐�
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_modelRender[i] = NewGO<CModelRender>(0);
	}
	//���f�������_���[��������
	m_modelRender[enFront]->Init(filePath_front);
	m_modelRender[enBack]->Init(filePath_back);
	//�^�C�v��ݒ�
	m_reversibleType[enFront] = type_front;
	m_reversibleType[enBack] = type_back;
	//�����\�̏�Ԃ����̏�Ԃ��ݒ肷��
	SetObjectType(m_reversibleType[m_frontOrBack]);
	SetFrontOrBack(m_frontOrBack);

	return true;
}


/// <summary>
/// ���]������
/// </summary>
void CReversibleObject::Reverse()
{
	//���݂̕\�����́A�t�̐ݒ�ɂ���
	SetFrontOrBack(!m_frontOrBack);
}

/// <summary>
/// �\��������ݒ肷�邷��
/// </summary>
/// <param name="frontOrBack">enFront��enBack������</param>
void CReversibleObject::SetFrontOrBack(bool frontOrBack)
{
	//�\������ݒ肷��
	m_frontOrBack = frontOrBack;
	if (m_bothModelactiveFlag)
	{
		//�^�C�v��ݒ肷��
		SetObjectType(m_reversibleType[m_frontOrBack]);
		//���݂̕\������L��������
		if (m_modelRender[m_frontOrBack])
			m_modelRender[m_frontOrBack]->Activate();
		//�����𖳌�������
		if (m_modelRender[!m_frontOrBack])
			m_modelRender[!m_frontOrBack]->Deactivate();
	}
	else
	{
		SetFrontOrBackSub();
	}
}

/// <summary>
/// �\�Ɨ��̗����̃��f���̗L�����t���O��ݒ肷��B
/// true����ꂽ�ꍇ�A���݂̕\�����ǂ��炩��L�������A�����𖳌�������B
/// false����ꂽ�ꍇ�A�����𖳌�������
/// </summary>
/// <param name="activeFlag">�L�����t���O</param>
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

//�A�b�v�f�[�g�֐�
void CReversibleObject::Update()
{
	//�v���C���[���������Ă��Ȃ�������
	if (!m_pPlayer)
	{
		//�v���C���[��T��
		m_pPlayer = CLevelObjectManager::GetInstance()->GetPlayer();

		//����ł�������Ȃ������牽������return
		if (!m_pPlayer)
			return;
	}

	/// <summary>
	/// �X�e�[�g�i��ԁj�A�b�v�f�[�g������U��
	/// </summary>
	switch (m_objectState)
	{
	case enCheckPlayer:	//�v���C���[�Ɏ�����邩�ǂ������ׂ�
		CheckPlayer();
		break;
	case enHeldPlayer:	//�v���C���[�Ɏ�����Ă�����
		HeldPlayer();
		break;
	case enCancel:		//�����Ă���I�u�W�F�N�g�����̏�ɒu��
		Cancel();
		break;
	case enThrownSide:	//�����Ă���I�u�W�F�N�g�����ɓ�����֐�
		ThrownSide();
		break;
	case enThrownDown:	//�����Ă���I�u�W�F�N�g�����ɓ�����֐�
		ThrownDown();
		break;
	case enQuery:		//�N�G�����Ăق����^�C�~���O
		Query();
		break;
	}


	//���f�������_���[�̍X�V
	for (int i = 0; i < enFrontAndBackNum; i++)
	{
		m_modelRender[i]->SetPosition(m_position);
		m_modelRender[i]->SetRotation(m_rotation);
		m_modelRender[i]->SetScale(m_scale);
	}


	//�I�[�o�[���C�h���Ăق����֐�UpdateSub()
	UpdateSub();

	return;
}


/// <summary>
/// �v���C���[�Ɏ�����邩�ǂ������ׂ�֐�
/// �����ꂽ��HeldPlayer�փX�e�[�g�i��ԁj�ڍs
/// </summary>
void CReversibleObject::CheckPlayer()
{
	//�v���C���[�Ƃ̓����蔻��
	if (IsHitPlayer())
	{
		//A�{�^������������
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//�v���C���[���ʂ̃I�u�W�F�N�g�������Ă��Ȃ�������
			if (!m_pPlayer->GetHoldObject())
			{
				//�X�e�[�g���v���C���[�Ɏ�����Ă����Ԃ�
				m_objectState = enHeldPlayer;
				//�v���C���[���I�u�W�F�N�g�������Ă��Ԃɂ���
				m_pPlayer->SetHoldObject(true);
			}
		}
	}
}


/// <summary>
/// �v���C���[�Ɏ�����Ă����Ԃ̊֐��B
/// �v���C���[�Ɏ�����Ă���ꏊ�Ɉړ����鏈���B
/// enCancel���AenThrownDown���AenThrownSide��
/// �X�e�[�g�i��ԁj���ڍs
/// </summary>
void CReversibleObject::HeldPlayer()
{
	//�v���C���[�̉�]��ێ�
	Quaternion qRot = m_pPlayer->GetFinalWPRot();
	//������x�N�g����ێ�
	Vector3 up = g_vec3Up;
	//������̃x�N�g�����v���C���[�̉�]�ŉ�
	qRot.Apply(up);
	//�x�N�g����L�΂�
	up.Scale(100.0f);

	//���f���̏ꏊ���v���C���[�̏�ɂ���
	m_position = m_pPlayer->GetPosition() + up;
	//���f���̉�]���v���C���[�Ɠ����ɂ���
	m_rotation = qRot;


	//A�{�^����������
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//�v���C���[�̉�]��ێ�����
		m_throwRot = m_pPlayer->GetFinalWPRot();

		//�X�e�[�g�����ɓ������Ԃ�
		m_objectState = enThrownDown;
	}
	//B�{�^����������
	else if (g_pad[0]->IsTrigger(enButtonB))
	{
		//�X�e�[�g���L�����Z����Ԃ�
		m_objectState = enCancel;
	}
	//X�{�^��
	else if (g_pad[0]->IsTrigger(enButtonX))
	{
		//���������v�Z����
		CalcTargetPos();

		//�X�e�[�g�����ɓ������Ԃ�
		m_objectState = enThrownSide;
	}
}

/// <summary>
/// �����Ă���I�u�W�F�N�g�����̏�ɒu���֐�
/// enQuery�փX�e�[�g�i��ԁj���ڍs
/// </summary>
void CReversibleObject::Cancel()
{
	//�v���C���[�̉�]��ێ�����
	Quaternion qRot = m_pPlayer->GetFinalWPRot();
	//������̃x�N�g����ێ�����
	Vector3 up = g_vec3Up;
	//������̃x�N�g�����v���C���[�̉�]�ŉ�
	qRot.Apply(up);
	//�x�N�g����L�΂�
	up.Scale(20.0f);

	//���f���̏ꏊ���v���C���[�̑����̂�����Ə�ɂ���
	m_position = m_pPlayer->GetPosition() + up;
	//�v���C���[���I�u�W�F�N�g�������ĂȂ���Ԃɂ���
	m_pPlayer->SetHoldObject(false);
	//�X�e�[�g���N�G���Ɉڍs����
	m_objectState = enQuery;
}


/// <summary>
/// �����Ă���I�u�W�F�N�g�����ɓ�����֐�
/// enQuery�փX�e�[�g�i��ԁj���ڍs
/// </summary>
void CReversibleObject::ThrownSide()
{
	//�܂��ł��ĂȂ�


	//�v���C���[���I�u�W�F�N�g�������ĂȂ���Ԃɂ���
	m_pPlayer->SetHoldObject(false);
	//�X�e�[�g���v���C���[�Ɏ�����邩�ǂ������ׂ��ԂɈڍs����
	m_objectState = enCheckPlayer;
}

/// <summary>
/// �I�u�W�F�N�g�����ɓ�����ۂ́A���������v�Z����֐�
/// </summary>
void CReversibleObject::CalcTargetPos()
{
	//�܂��ł��Ă��Ȃ�

	m_pPlayer->GetLeftPointIndex();
}

/// <summary>
/// �����Ă���I�u�W�F�N�g�����ɓ�����֐�
/// enQuery�փX�e�[�g�i��ԁj���ڍs
/// </summary>
void CReversibleObject::ThrownDown()
{
	//�������̃x�N�g����ێ�����
	Vector3 dir = g_vec3Down;
	//�v���C���[�̉�]�ŉ������̂׃N�g������
	m_throwRot.Apply(dir);
	//�x�N�g����L�΂�
	dir.Scale(7.0f);

	//���f���̏ꏊ�����ɉ�����
	m_position += dir;
	//���f���̉�]���A�t�������Ɍ������Ă�����Ƃ���
	m_rotation.SetRotationDegX(180.0f * m_throwCounter / 24);
	m_rotation.Multiply(m_throwRot);

	//�����Ă��鎞�̃J�E���^�[�̍ő�l
	const int maxThrowCounter = 24;

	//�����Ă��鎞�̃J�E���^�[��i�߂�
	m_throwCounter++;

	//�����Ă��鎞�̃J�E���^�[���ő�l�܂ŗ�����
	if (m_throwCounter >= 24)
	{
		//���f���̉�]�����S�ɋt�������ɉ񂷁B
		m_rotation.SetRotationDegX(180.0f);
		m_rotation.Multiply(m_throwRot);

		//�v���C���[���I�u�W�F�N�g�������Ă��Ȃ���Ԃɂ���
		m_pPlayer->SetHoldObject(false);

		//�����Ă��鎞�̃J�E���^�[��0�ɖ߂�
		m_throwCounter = 0;

		//�X�e�[�g���N�G���ֈڍs����
		m_objectState = enQuery;
	}
	//�����Ă��鎞�̃J�E���^�[���ő�l�̔����܂ŗ�����
	else if (m_throwCounter == m_throwCounter / 2)
	{
		//���]������
		Reverse();
	}
}

/// <summary>
/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
/// �I�[�o�[���C�h���Ăق����֐��AQuerySub()�͂����ŌĂ΂��B
/// enCheckPlayer�փX�e�[�g�i��ԁj���ڍs
/// </summary>
void CReversibleObject::Query()
{
	//�I�[�o�[���C�h���Ăق����֐�QuerySub()
	QuerySub();

	//�X�e�[�g���v���C���[�Ɏ�����邩�ǂ������ׂ��ԂɈڍs����
	m_objectState = enCheckPlayer;
}
