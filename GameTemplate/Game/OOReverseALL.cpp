#include "stdafx.h"
#include "OOReverseALL.h"

//�X�^�[�g�֐�
bool OOReverseALL::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init("Assets/modelData/grilled fish.tkm", enReverseAll);

	m_scale = { 0.0f,0.0f,0.0f };

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//���Ȕ����F��ݒ�
	SetEmissionColor({ 0.5f,0.0f,0.5f,1.0f });

	//OBB�̃p�����[�^�[��ݒ肷��
	GetOBB().SetDirectionLength({ 10.0f,400.0f,400.0f });
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });

	//changeSE�̃T�E���h�L���[�𐶐�����
	m_changeSE = NewGO<CSoundCue>(0);
	//changeSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_changeSE->Init(L"Assets/sound/change.wav");
	//���ʒ���
	m_changeSE->SetVolume(0.5f);

	//�G�t�F�N�g�̍쐬
	m_obujectefk = NewGO<Effect>(0);
	m_obujectefk->Init(u"Assets/effect/reverseall.efk");
	float scale = 200.0f;								//�������̂ő傫�����Ă���
	m_obujectefk->SetScale({ scale ,scale ,scale });
	m_obujectefk->SetPosition(m_position);				//���W��n��
	m_obujectefk->SetRotation(m_rotation);
	m_obujectefk->Play();								//�Đ�

	//�f�o�b�N�p
#ifdef MY_DEBUG
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
#endif
	return true;
}

//�f�X�g���N�^
OOReverseALL::~OOReverseALL()
{
	DeleteGO(m_obujectefk);
#ifdef MY_DEBUG
	//�f�o�b�N�p
	//��ŏ���
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBB�̒��_�����邽�߂̃��f���̔j��
		DeleteGO(m_dbgVertPosMR[i]);
	}
	//�f�o�b�N�p�����܂�
#endif
}

//�A�b�v�f�[�g�֐�
void OOReverseALL::UpdateSub()
{
	//�G�t�F�N�g�Đ��܂ł̃^�C�}�[
	i++;
	if (i == 121.0f) {				//120�t���[���ŃG�t�F�N�g���I���̂�121����n�߂�
		m_obujectefk->Play();
		i = 0;
	}
	//�A�b�v�f�[�g�X�e�[�g�ŏ�����U�蕪����
	//switch (m_updateState)
	//{
	//case enBeforHitPlayer:
	//	//�v���C���[�ƏՓˑO�̏���
	//	BeforeHitPlayer();
	//	break;
	//case enHitPlayer:
	//	//�v���C���[�ƏՓˎ��̏���
	//	HitPlayer();
	//	break;
	//case enAfterHitPlayer:
	//	//�v���C���[�ƏՓˌ�̏���
	//	AfterHitPlayer();
	//	break;
	//}


	///
	///
	/// switch��else if�ɂ��Ă��܂��ƁA
	/// �ړI�̏�����1�t���[���x��Ă��܂�����
	/// �S��if�ŐU�蕪�����B

	//�A�b�v�f�[�g�X�e�[�g�ŏ�����U�蕪����
	if (m_updateState == enBeforHitPlayer)
		//�v���C���[�ƏՓˑO�̏���
		BeforeHitPlayer();

	if (m_updateState == enHitPlayer)
		//�v���C���[�ƏՓˎ��̏���
		HitPlayer();

	if (m_updateState == enAfterHitPlayer)
		//�v���C���[�ƏՓˌ�̏���
		AfterHitPlayer();

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

		//�Փˌ�̃v���C���[�̍��W�̊m��
		m_playerAfterPosition = m_pPlayer->GetPosition();
		//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˌ�̏�Ԃ�
		m_updateState = enAfterHitPlayer;
	}
}

//�v���C���[�ƏՓˌ�̏���
void OOReverseALL::AfterHitPlayer()
{
	//�Փˌ�̃v���C���[�̍��W�̊m��
	m_playerAfterPosition = m_pPlayer->GetPosition();

	//�ՓˑO����Փˎ��̍��W�ւ̃x�N�g��
	Vector3 beforeToHit = m_playerHitPosition - m_playerBeforePosition;
	//�Փˎ�����Փˌ�̍��W�ւ̃x�N�g��
	Vector3 hitToAfter = m_playerAfterPosition - m_playerHitPosition;
	//���K�����Ă���
	beforeToHit.Normalize();
	hitToAfter.Normalize();
	//��̓��ς����
	float inner = Dot(beforeToHit, hitToAfter);

	//���ςœ�̃x�N�g���̌����𒲂ׂ�
	if (inner >= 0.0f)
	{
		//���A���������Ȃ�

		//���x���I�u�W�F�N�g������Ă���
		std::vector<ILevelObjectBase*> levelObjects
			= CLevelObjectManager::GetInstance()->GetLevelObjects();
		//�S�Ẵ��x���I�u�W�F�N�g�Ɍ���
		for (int i = 0; i < levelObjects.size(); i++)
		{
			//���f���̎Q�Ƃ𓾂Ă���ASetMulColor()���Ă�
			//Obstacle�̏ꍇ�͖��ʂɓ��Ă΂�邯�ǁA�䖝���悤�B
			/*levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
			levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });*/
			//CReversibleObject�Ȃ甽�]������
			CReversibleObject* revers = dynamic_cast<CReversibleObject*>(levelObjects[i]);
			if (revers)
				revers->AllReverse();
		}

	}

	//changeSE�����[�v�Đ����I�t�ōĐ�����B
	m_changeSE->Play(false);

	//�ՓˑO�̃v���C���[�̍��W��ێ�
	m_playerBeforePosition = m_pPlayer->GetPosition();

	//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˑO�̏�Ԃ�
	m_updateState = enBeforHitPlayer;
}