#include "stdafx.h"
#include "OOReverseALL.h"

//�X�^�[�g�֐�
bool OOReverseALL::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init("Assets/modelData/grilled fish.tkm", enEmpty);

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//���Ȕ����F��ݒ�
	SetEmissionColor({ 0.5f,0.0f,0.5f,1.0f });

	//OBB�̃p�����[�^�[��ݒ肷��
	GetOBB().SetDirectionLength({ 10.0f,400.0f,400.0f });
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });


	//�f�o�b�N�p
	//��ŏ���
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBB�̒��_�����邽�߂̃��f���̐����Ə�����
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);
	}
	//�f�o�b�N�p�����܂�

	return true;
}

//�f�X�g���N�^
OOReverseALL::~OOReverseALL()
{

	//�f�o�b�N�p
	//��ŏ���
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBB�̒��_�����邽�߂̃��f���̔j��
		DeleteGO(m_dbgVertPosMR[i]);
	}
	//�f�o�b�N�p�����܂�
}

//�A�b�v�f�[�g�֐�
void OOReverseALL::UpdateSub()
{
	//�A�b�v�f�[�g�X�e�[�g�ŏ�����U�蕪����
	switch (m_updateState)
	{
	case enBeforHitPlayer:
		//�v���C���[�ƏՓˑO�̏���
		BeforeHitPlayer();
		break;
	case enHitPlayer:
		//�v���C���[�ƏՓˎ��̏���
		HitPlayer();
		break;
	case enAfterHitPlayer:
		//�v���C���[�ƏՓˌ�̏���
		AfterHitPlayer();
		break;
	}

}

//�v���C���[�ƏՓˑO�̏���
void OOReverseALL::BeforeHitPlayer()
{
	//�v���C���[�Ƃ̏Փ˂𒲂ׂ�
	if (IsHitPlayer())
	{
		//�Փ˂��Ă���

		//�Փˎ��̃v���C���[�̍��W��ێ�
		m_playerHitPosition = m_pPlayer->GetPosition();
		//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˎ��̏�Ԃ�
		m_updateState = enHitPlayer;
	}
	else
	{
		//�Փ˂��Ă��Ȃ�

		//�ՓˑO�̃v���C���[�̍��W��ێ�
		m_playerBeforePosition = m_pPlayer->GetPosition();
	}
}

//�v���C���[�ƏՓˎ��̏���
void OOReverseALL::HitPlayer()
{
	//�v���C���[�Ƃ̏Փ˂𒲂ׂ�
	if (IsHitPlayer())
	{
		//�Փ˂��Ă���

		//�Փˎ��̃v���C���[�̍��W��ێ�
		m_playerHitPosition = m_pPlayer->GetPosition();
	}
	else
	{
		//�Փ˂��Ă��Ȃ�

		//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˌ�̏�Ԃ�
		m_updateState = enAfterHitPlayer;
	}
}

//�v���C���[�ƏՓˌ�̏���
void OOReverseALL::AfterHitPlayer()
{
	//�Փˌ�̃v���C���[��
	m_playerAfterPosition = m_pPlayer->GetPosition();

	Vector3 beforeToHit = m_playerHitPosition - m_playerBeforePosition;
	Vector3 hitToAfter = m_playerAfterPosition - m_playerHitPosition;
	beforeToHit.Normalize();
	hitToAfter.Normalize();
	float inner = Dot(beforeToHit, hitToAfter);

	if (inner >= 0.0f)
	{
		std::vector<ILevelObjectBase*> levelObjects
			= CLevelObjectManager::GetInstance()->GetLevelObjects();
		for (int i = 0; i < levelObjects.size(); i++)
		{
			CReversibleObject* revers = dynamic_cast<CReversibleObject*>(levelObjects[i]);
			if (revers)
				revers->Reverse();
		}

	}
	m_updateState = enBeforHitPlayer;
}