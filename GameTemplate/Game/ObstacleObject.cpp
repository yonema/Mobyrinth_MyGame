#include "stdafx.h"
#include "ObstacleObject.h"

//�X�^�[�g�֐�
bool CObstacleObject::PureVirtualStart()
{
	//OBBWorld�Ɏ��g��OBB��o�^����
	//����ɂ����Player���ʂ�Ȃ��Ȃ�
	COBBWorld::GetInstance()->AddOBB(GetOBB());

	//�\���ɂ��邩�����ɂ��邩���ׂ�
	//���]�I�u�W�F�N�g�ł͂Ȃ�����false��n��
	CheckFrontOrBackSide(false);

	//�I�[�o�[���C�h���Ăق����֐�StartSub()�͂����ŌĂ΂��B
	return StartSub();
}


//�f�X�g���N�^
CObstacleObject::~CObstacleObject()
{
	//LevelObjectManager�ɂ��̃I�u�W�F�N�g�͏�����I���ē`����B
	Delete();

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(GetOBB());

	//���f���̏���
	DeleteGO(m_modelRender);

	return;
}



/// <summary>
/// �������֐�
/// �ŏ��ɌĂ�łˁBtrue��߂��ĂˁB
/// ���f���̃t�@�C���p�X�ƃI�u�W�F�N�g�̃^�C�v��
/// �ݒ肷��
/// �^�C�v�ꗗ��ObjectType.h���Q��
/// </summary>
/// <param name="filePath">���f����tkm�t�@�C���p�X</param>
/// <param name="objectType">�^�C�v</param>
/// <returns>true��߂��Ă�</returns>
bool CObstacleObject::Init(const char* filePath, int objectType)
{
	//���f�������_���[�𐶐�
	m_modelRender = NewGO<CModelRender>(PRIORITY_FIRST);
	//���f�������_���[��������
	m_modelRender->Init(filePath, D3D12_CULL_MODE_NONE);
	m_modelRender->SetShadowReceiverFlag(false);
	//�I�u�W�F�N�g�̃^�C�v��ݒ肷��
	SetObjectType(objectType);
	m_modelRender->SetDrawOutLineFlag(true);

	return true;
}

//�A�b�v�f�[�g�֐�
void CObstacleObject::PureVirtualUpdate()
{
	//�v���C���[���������Ă��Ȃ�������
	if (!m_player)
	{
		//�v���C���[��T��
		m_player = CLevelObjectManager::GetInstance()->GetPlayer();

		//����ł�������Ȃ������牽������return
		if (!m_player)
			return;
	}

	//���f�������_���[�̏ꏊ�Ɖ�]�Ɗg��
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->SetScale(m_scale);

	//�I�[�o�[���C�h���Ăق����֐�UpdateSub()
	UpdateSub();

	return;
}