#include "stdafx.h"
#include "OOReverseALL.h"

//�X�^�[�g�֐�
bool OOReverseALL::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init("Assets/modelData/grilled fish.tkm", EN_OO_TYPE_REVERSE_ALL);

	m_scale = { 0.0f,0.0f,0.0f };

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(GetOBB());

	//���Ȕ����F��ݒ�
	SetModelEmissionColor({ 0.5f,0.0f,0.5f,1.0f });

	//OBB�̃p�����[�^�[��ݒ肷��
	SetOBBDirectionLength({ 10.0f,400.0f,400.0f });
	SetOBBPivot({ 0.5f,0.0f,0.5f });

	//�I�u�W�F�N�g�Ɠ�����Ȃ��悤�ɂ���
	SetIsHitFlag(false);

	//changeSE�̃T�E���h�L���[�𐶐�����
	m_changeSE = NewGO<CSoundCue>(0);
	//changeSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_changeSE->Init(L"Assets/sound/allchange.wav");
	//���ʒ���
	m_changeSE->SetVolume(1.0f);

	//m_obujectefk�G�t�F�N�g�̍쐬
	m_obujectefk = NewGO<Effect>(0);
	m_obujectefk->Init(u"Assets/effect/reverseall.efk");
	float scale = 200.0f;								//�������̂ő傫�����Ă���
	m_obujectefk->SetScale({ scale ,scale ,scale });
	m_obujectefk->SetPosition(m_position);				//���W��n��
	m_obujectefk->SetRotation(m_rotation);
	m_obujectefk->Play();								//�Đ�

	//m_reverseall2�G�t�F�N�g�̍쐬
	m_reverseall2 = NewGO<Effect>(0);
	m_reverseall2->Init(u"Assets/effect/reverseall2.efk");
	float scale2 = 50.0f;								//�������̂ő傫�����Ă���
	m_reverseall2->SetScale({ scale2 ,scale2 ,scale2 });
	m_reverseall2->SetPosition(m_position);				//���W��n��
	m_reverseall2->SetRotation(m_rotation);

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
	//�G�t�F�N�g���Đ����Ȃ�
	if (m_obujectefk->IsPlay())
		//��~����
		m_obujectefk->Stop();

	//m_objectefk�̍폜
	DeleteGO(m_obujectefk);

	//m_reverseall2���폜
	DeleteGO(m_reverseall2);

	//m_changeSE���폜
	DeleteGO(m_changeSE);

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

//�|�[�Y���ł����ł��A�b�v�f�[�g����֐�
void OOReverseALL::AlwaysUpdate()
{
		//�G�t�F�N�g�Đ��܂ł̃^�C�}�[
	i += GameTime().GetFrameDeltaTime();

	if (i >= 2.0f) {				//120�t���[���ŃG�t�F�N�g���I���̂ł���n�߂�
		m_obujectefk->Play();
		i = 0;
	}
}


//�A�b�v�f�[�g�֐�
void OOReverseALL::UpdateSub()
{

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
		m_playerHitPosition = m_player->GetPosition();
		//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˎ��̏�Ԃ�
		m_updateState = enHitPlayer;

		
	}
	else
	{
		//�Փ˂��Ă��Ȃ�

		//�ՓˑO�̃v���C���[�̍��W��ێ�
		m_playerBeforePosition = m_player->GetPosition();
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
		m_playerHitPosition = m_player->GetPosition();
	}
	else
	{
		//�Փ˂��Ă��Ȃ�

		//�Փˌ�̃v���C���[�̍��W�̊m��
		m_playerAfterPosition = m_player->GetPosition();
		//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˌ�̏�Ԃ�
		m_updateState = enAfterHitPlayer;
	}
}

//�v���C���[�ƏՓˌ�̏���
void OOReverseALL::AfterHitPlayer()
{
	//�Փˌ�̃v���C���[�̍��W�̊m��
	m_playerAfterPosition = m_player->GetPosition();

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
			levelObjects[i]->GetModelRender(CReversibleObject::EN_BACK)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });*/
			//CReversibleObject�Ȃ甽�]������
			CReversibleObject* revers = dynamic_cast<CReversibleObject*>(levelObjects[i]);
			if (revers)
			{
				//�����I�u�W�F�N�g�ł͖���������
				if (revers->GetIsHitFlag())
					//���]������
					revers->AllReverse();
			}
		}
		//changeSE�����[�v�Đ����I�t�ōĐ�����B
		m_changeSE->Play(false);

		Vector3 upVec = g_VEC3_UP;
		m_rotation.Apply(upVec);
		//���̒l��ύX���č����𒲐߂���
		const float upVecLne = 100.0f;
		upVec.Scale(upVecLne);
		m_reverseall2->SetPosition(m_position + upVec);		//���W��n��
		m_reverseall2->SetRotation(m_rotation);
		m_reverseall2->Play();								//�Đ�
	}


	//�ՓˑO�̃v���C���[�̍��W��ێ�
	m_playerBeforePosition = m_player->GetPosition();

	//�A�b�v�f�[�g�X�e�[�g���v���C���[�ƏՓˑO�̏�Ԃ�
	m_updateState = enBeforHitPlayer;
}