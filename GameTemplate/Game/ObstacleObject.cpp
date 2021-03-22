#include "stdafx.h"
#include "ObstacleObject.h"

//�X�^�[�g�֐�
bool CObstacleObject::PureVirtualStart()
{
	//���f���̉�]���A���݂̏ꏊ�ƃC�C�����ɍ��킹��
	CheckWayPoint();

	//�I�[�o�[���C�h���Ăق����֐�StartSub()�͂����ŌĂ΂��B
	return StartSub();
}


//�f�X�g���N�^
CObstacleObject::~CObstacleObject()
{
	//LevelObjectManager�ɂ��̃I�u�W�F�N�g�͏�����I���ē`����B
	Delete();

	//���f���̏���
	DeleteGO(m_modelRender);
}



/// <summary>
/// �������֐�
/// �ŏ��ɌĂ�łˁBtrue��߂��ĂˁB
/// ���f���̃t�@�C���p�X�ƃI�u�W�F�N�g�̃^�C�v��
/// �ݒ肷��
/// �^�C�v�ꗗ��LevelObjectBase.h���Q��
/// </summary>
/// <param name="filePath">���f����tkm�t�@�C���p�X</param>
/// <param name="objectType">�^�C�v</param>
/// <returns>true��߂��Ă�</returns>
bool CObstacleObject::Init(const char* filePath, int objectType)
{
	//���f�������_���[�𐶐�
	m_modelRender = NewGO<CModelRender>(0);
	//���f�������_���[��������
	m_modelRender->Init(filePath);
	//�I�u�W�F�N�g�̃^�C�v��ݒ肷��
	SetObjectType(objectType);

	return true;
}

//�A�b�v�f�[�g�֐�
void CObstacleObject::PureVirtualUpdate()
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

	//���f�������_���[�̏ꏊ�Ɖ�]�Ɗg��
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	m_modelRender->SetScale(m_scale);

	//�I�[�o�[���C�h���Ăق����֐�UpdateSub()
	UpdateSub();

	return;
}