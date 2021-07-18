#include "stdafx.h"
#include "UFO.h"

//�X�^�[�g�֐�
bool CUFO::PureVirtualStart()
{
	//���f�������_���[�̐����Ə�����
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init("Assets/modelData/UFO2.tkm");
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);

	m_modelRender->SetDrawOutLineFlag(true);

	//�A�b�v�x�N�g�������݂̉�]�ɍ��킹��
	m_rotation.Apply(m_upVec);
	//�E�F�C�|�C���g��̍��W���X�V����
	m_onWayPosition = m_position;

	//OBB�𒲐߂���
	SetOBBPivot({ 0.5f,0.0f,0.5f });
	SetOBBDirectionLength({ 10.0f,200.0f,400.0f });

	//������̃X�|�b�g���C�g�̐����Ə�����
	//m_spotLightUp = NewGO<CSpotLight>(0);
	//�A�b�v�x�N�g��
	Vector3 up = m_upVec;
	//�C�C�����ɐL�΂�
	up.Scale(300.0f);
	//������̃X�|�b�g���C�g�̃p�����[�^��ݒ�
	//m_spotLightUp->SetPosition(m_position + up);
	//m_spotLightUp->SetColor({ 300.0f, 300.0f, 0.0f, 0.0f });
	//m_spotLightUp->SetRange(500.0f);
	//m_spotLightUp->SetAngle(/*90.0f*/3.14f * 0.5f);
	//m_spotLightUp->SetDirection(g_VEC3_DOWN);


	//�������̃X�|�b�g���C�g�̐����Ə�����
	up.Normalize();
	//�C�C�����ɐL�΂�
	up.Scale(500.0f);
	//�������̃X�|�b�g���C�g�̃p�����[�^��ݒ�
	/*m_spotLightDown = NewGO<CSpotLight>(0);
	m_spotLightDown->SetPosition(m_position + up);
	m_spotLightDown->SetColor({ 0.0f,0.0f,0.0f,1.0f });
	m_spotLightDown->SetRange(500.0f);
	m_spotLightDown->SetDirection(g_VEC3_DOWN);
	m_spotLightDown->SetAngle(3.14f * 0.25f);*/

	//UFO�̌����̃��f���̐����Ə�����
	m_ufoLight = NewGO<CModelRender>(1);
	ModelInitData ufoLigInitData;
	ufoLigInitData.m_tkmFilePath = "Assets/modelData/ufoLight.tkm";
	ufoLigInitData.m_fxFilePath = "Assets/shader/ufoLight.fx";
	//ZPrepass�ō쐬���ꂽ�[�x�e�N�X�`���̓o�^
	ufoLigInitData.m_expandShaderResoruceView[1] = &g_graphicsEngine->GetZPrepassDepthTexture();
	m_ufoLight->Init(ufoLigInitData);
	m_ufoLight->SetPosition(m_position + up);
	m_ufoLight->SetRotation(m_rotation);
	m_ufoLight->SetDrawOutLineFlag(false);
	//�ŏ��͌����Ȃ��悤�ɓ����ɂ���
	m_ufoLight->SetAlphaValue(modelRenderConstData::ALPHA_VALUE_TRANSPARENT);

	//UFO�̒��n�_�̐����Ə�����
	m_ufoLandingPoint = NewGO<CUFOLandingPoint>(0);
	m_ufoLandingPoint->SetPosition(m_position);

	//���̃��x���I�u�W�F�N�g�ƏՓ˂��Ȃ�����
	SetIsHitFlag(false);

	//UFOmoveSE�̃T�E���h�L���[�𐶐�����
	m_UFOmoveSE = NewGO<CSoundCue>(0);
	//UFOmoveSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_UFOmoveSE->Init(L"Assets/sound/UFOmove.wav");
	//���ʒ���
	m_UFOmoveSE->SetVolume(0.5f);

	//UFOcarrymoveSE�̃T�E���h�L���[�𐶐�����
	m_UFOcarrymoveSE = NewGO<CSoundCue>(0);
	//UFOcarrymoveSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_UFOcarrymoveSE->Init(L"Assets/sound/UFOcarrymove.wav");
	//���ʒ���
	m_UFOcarrymoveSE->SetVolume(0.5f);

	//UFOyellowlight�̃T�E���h�L���[�𐶐�����
	m_UFOyellowlightSE = NewGO<CSoundCue>(0);
	//UFOyellowlight�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_UFOyellowlightSE->Init(L"Assets/sound/UFOyellowlight.wav");
	//���ʒ���
	m_UFOyellowlightSE->SetVolume(0.5f);

	//UFOdiscoverySE�̃T�E���h�L���[�𐶐�����
	m_UFOdiscoverySE = NewGO<CSoundCue>(0);
	//UFOdiscoverySE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_UFOdiscoverySE->Init(L"Assets/sound/UFOdiscovery.wav");
	//���ʒ���
	m_UFOdiscoverySE->SetVolume(0.5f);

	//UFOdiscoverySE�̃T�E���h�L���[�𐶐�����
	m_UFOredlightSE = NewGO<CSoundCue>(0);
	//UFOredlight�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_UFOredlightSE->Init(L"Assets/sound/UFOredlight.wav");
	//���ʒ���
	m_UFOredlightSE->SetVolume(0.5f);

#ifdef MY_DEBUG
	//�f�o�b�N�p
	//��ŏ���
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBB�̒��_�����邽�߂̃��f��
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}
	for (int i = 0; i < 2; i++)
	{
		//���C�̎n�_�ƏI�_�����邽�߂̃��f��
		m_dbgRay[i] = NewGO<CModelRender>(0);
		m_dbgRay[i]->Init("Assets/modelData/dbgBox.tkm");
	}
	
	//�X�|�b�g���C�g�̈ʒu�����邽�߂̃��f��
	m_dbgSpotLigPos = NewGO<CModelRender>(0);
	m_dbgSpotLigPos->Init("Assets/modelData/dbgBox.tkm");

#endif
	//�f�o�b�N�p�����܂�

	return true;
}

//�f�X�g���N�^
CUFO::~CUFO()
{
	//LevelObjectManager�ɂ��̃I�u�W�F�N�g�͏�����I���ē`����B
	Delete();

	//���f�������_���[�̔j��
	DeleteGO(m_modelRender);

	//�X�|�b�g���C�g�̔j��
	//DeleteGO(m_spotLightUp);
	//DeleteGO(m_spotLightDown);
	//UFO�̌����̃��f���̔j��
	DeleteGO(m_ufoLight);

	//UFO�̒��n�_�̔j��
	DeleteGO(m_ufoLandingPoint);

	//UFOmoveSE�̔j��
	DeleteGO(m_UFOmoveSE);

	//UFOcarrymoveSE�̔j��
	DeleteGO(m_UFOcarrymoveSE);

	//UFOyellowlightSE�̔j��
	DeleteGO(m_UFOyellowlightSE);

	//UFOredightSE�̔j��
	DeleteGO(m_UFOredlightSE);

	//UFOdiscoverySE�̔j��
	DeleteGO(m_UFOdiscoverySE);

#ifdef MY_DEBUG
	//�f�o�b�N�p
	//��ŏ���
	for (int i = 0; i < m_vertNum; i++)
	{
		//���_�����邽�߂̃��f���̔j��
		DeleteGO(m_dbgVertPosMR[i]);
	}
	for (int i = 0; i < 2; i++)
	{
		//���C�����邽�߂̃��f����j��
		DeleteGO(m_dbgRay[i]);
	}
	//�X�|�b�g���C�g�����邽�߂̃��f����j��
	DeleteGO(m_dbgSpotLigPos);
#endif
	//�f�o�b�N�p�����܂�
}

//�A�b�v�f�[�g�֐�
void CUFO::PureVirtualUpdate()
{
	//�ړ�����
	Move();
	//��]�����݂̍��W�ɍ��킹�Ă���]�ɂ���
	CheckRotation();
	//�A�b�v�x�N�g������]�ɍ��킹��
	m_upVec = g_VEC3_UP;
	m_rotation.Apply(m_upVec);
	m_upVec.Normalize();
	//�X�e�[�W�̏�ɏ��
	GetOnStage();


	//�A�b�v�f�[�g�̃X�e�[�g�ōs��������U��
	switch (m_updateState)
	{
	case enSearch:
		//�v���C���[��T������
		Search();
		break;
	case enCapture:
		//�v���C���[�����������̏���
		Capture();
		break;
	case enTransport:
		//�v���C���[���^�ԏ���
		Transport();
		break;
	case enLanding:
		//���n�̏���
		Landing();
		break;
	case enLeave:
		//���n�_���痣��鏈��
		Leave();
		break;
	}

	//���C�g�̍X�V
	UpdateLight();

	//���f�������_���[�̍X�V
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(Rotating());

	//�f�o�b�N�p
#ifdef MY_DEBUG
	//��ŏ���
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//���_�����邽�߂̃��f���̍X�V
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}
#endif
	//�f�o�b�N�p�����܂�
}

//�v���C���[��UFO�ɋ߂Â��Ɖ���炷
void CUFO::UFOmove()
{
	Vector3 distance = m_position - m_player->GetPosition();
	const float MaxDist = 1500;
	const float DistLen = distance.Length();

	if (DistLen < MaxDist) {
		float Le = MaxDist - DistLen;
		float SubLe = Le / MaxDist;
		float Vo = 2.0f * SubLe;

		//UFOmoveSE�����[�v�Đ����I���ōĐ�����B
		m_UFOmoveSE->Play(true);

		//���ʒ���
		m_UFOmoveSE->SetVolume(Vo);

	}
	else {
		if (m_UFOmoveSE->IsPlaying()) {
			m_UFOmoveSE->Stop();
		}
	}
}

//�v���C���[��UFOyellowlight�ɋ߂Â��Ɖ���炷
void CUFO::UFOyellowlight()
{
	Vector3 distance = m_position - m_player->GetPosition();
	const float MaxDist = 1500;
	const float DistLen = distance.Length();

	if (DistLen < MaxDist) {
		float Le = MaxDist - DistLen;
		float SubLe = Le / MaxDist;
		float Vo = 2.0f * SubLe;

		//UFOyellowlightSE�����[�v�Đ����I���ōĐ�����B
		m_UFOyellowlightSE->Play(true);

		//���ʒ���
		m_UFOyellowlightSE->SetVolume(Vo);

	}
	else {
		if (m_UFOyellowlightSE->IsPlaying()) {
			m_UFOyellowlightSE->Stop();
		}
	}
}

//�v���C���[��T������
void CUFO::Search()
{
	//if (g_pad[0]->IsPress(enButtonA))
	//{
	//	m_moveSpeed = m_defaultSpeed;
	//}
	//else if (g_pad[0]->IsPress(enButtonB))
	//{
	//	m_moveSpeed = -m_defaultSpeed;
	//}
	//else
	//	m_moveSpeed = 0.0f;

	//�X�s�[�h��0.0f�Ȃ�T���Ȃ�
	if (m_moveSpeed == 0.0f)
		return;


	//�^�C�}�[�Ƀf���^�^�C�������Z
	m_timer += GameTime().GetFrameDeltaTime();

	//�{���t���O�̐؂�ւ��Ǝ���
	const float switchingTime = 1.5f;

	//�؂�ւ����ԂɂȂ�����
	if (m_timer >= switchingTime)
	{
		//�t���O��؂荇����
		m_searchFlag = !m_searchFlag;
		//�^�C�}�[��0�ɖ߂�
		m_timer = 0.0f;
	}
	//m_searchFlag = true;
	//�T�������H
	if (m_searchFlag)
	{
		//�{�����Ȃ�

		//���F�Ɍ���
		//m_modelRender->SetModelEmissionColor({ 0.5f,0.5f,0.0f,1.0f });
		//�㉺�̃X�|�b�g���C�g�����点��
		//m_spotLightUp->SetColor({ 450.0f,450.0f,0.0f,1.0f });
		//m_spotLightDown->SetColor({ 450.0f,450.0f,0.0f,1.0f });
		//UFO�̌��������点��
		m_ufoLight->SetModelEmissionColor({ 1.5f,1.5f,0.0f,1.0f });
		//UFO�̌����𔼓����ɂ���
		m_ufoLight->SetAlphaValue(0.5f);
		//UFOyellowlightSE
		UFOyellowlight();
		//�֊s���������悤�ɂ���
		m_ufoLight->SetDrawOutLineFlag(true);


		//�v���C���[���Փ˂��Ă��邩�H
		if (IsHitPlayer())
		{
			//�v���C���[��������

			//�A�b�v�f�[�g�X�e�[�g���v���C���[����������Ԃɂ���
			m_updateState = enCapture;

			//�ԐF�Ɍ���
			//m_modelRender->SetModelEmissionColor({ 1.0f,0.0f,0.0f,1.0f });
			//�㉺�̃X�|�b�g���C�g�����点��
			//m_spotLightUp->SetColor({ 900.0f,0.0f,0.0f,1.0f });
			//m_spotLightDown->SetColor({ 900.0f,0.0f,0.0f,1.0f });
			//UFO�̌��������点��
			m_ufoLight->SetModelEmissionColor({ 3.0f,0.0f,0.0f,1.0f });
			//UFO�̌����𔼓����ɂ���
			m_ufoLight->SetAlphaValue(0.5f);
			//�֊s���������悤�ɂ���
			m_ufoLight->SetDrawOutLineFlag(true);
			
			//�v���C���[��UFO�ɕ߂܂�����Ԃɂ���
			m_player->SetCapturedUFOFlag(true);
			m_player->SetRotation(m_rotation);
			m_player->SetCapturedRotation(m_rotation);
			//�v���C���[�����������Ă�����A��������
			if (m_player->GetHoldObject())
				m_player->GetReversibleObject()->StateToCancel();
			//�X�s�[�h��0�ɂ���
			m_moveSpeed = 0.0f;
			//�^�C�}�[������������
			m_timer = 0.0f;

			//UFOyellowSE�����Ă�����~�߂�
			if (m_UFOyellowlightSE->IsPlaying()) {
				m_UFOyellowlightSE->Stop();
			}
			//UFOdiscoverySE�����[�v�Đ����I�t�ōĐ�����B
			m_UFOdiscoverySE->Play(false);

			//UFOredlightSE�����[�v�Đ����I�t�ōĐ�����B
			m_UFOredlightSE->Play(false);
		}
	}
	else
	{
		//�{�����ł͂Ȃ�

		//����Ȃ�
		//m_modelRender->SetModelEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		//�㉺�̃X�|�b�g���C�g�̌�������
		//m_spotLightUp->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//m_spotLightDown->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//UFO�̌����̌�������
		m_ufoLight->SetModelEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		//UFO�̌����̓����ɂ���
		m_ufoLight->SetAlphaValue(modelRenderConstData::ALPHA_VALUE_TRANSPARENT);
		//�֊s���������Ȃ��悤�ɂ���
		m_ufoLight->SetDrawOutLineFlag(false);
		//UFOyellowSE�����Ă�����~�߂�
		if(m_UFOyellowlightSE->IsPlaying()) {
			m_UFOyellowlightSE->Stop();
		}

	}
}

//�v���C���[�����������̏���
void CUFO::Capture()
{
	//UFO�Ɍ������Ăт����肵�Ď~�܂��Ă��鎞��
	const float surprisedTimer = 1.0f;
	//�^�C�}�[�̐؂�ւ�����
	const float switchingTimer = surprisedTimer + 2.0f;

	//�^�C�}�[���т����肵�Ă��鎞�Ԃ�菬������
	if (m_timer < surprisedTimer)
	{
		//�������Ƃ�

		//�v���C���[�͂��̏�ɂƂǂ܂点��
		m_player->SetCapturedPosition(m_player->GetPosition());
		m_player->SetCapturedRotation(m_player->GetRotation());
		//�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();

		//���̂܂ܖ߂�
		return;
	}

	//�A�b�v�x�N�g���𓾂�
	Vector3 upVec = m_upVec;
	//�C�C�����ɐL�΂�
	const float upVecLen = 550.0f;
	upVec.Scale(upVecLen);

	//UFO�ɕ߂܂�ʒu
	Vector3 capturePos = m_position + upVec;

	//�v���C���[�ɉ�����x�N�g��
	Vector3 addVec = capturePos - m_player->GetPosition();
	//�f���^�^�C�����|���Ă���
	addVec.Scale(GameTime().GetFrameDeltaTime());
	//�v���C���[�̍��W��ݒ�
	Vector3 pos = m_player->GetPosition() + addVec;
	m_player->SetPosition(pos);

	//�v���C���[�ɓn����]
	Quaternion qRot;
	//���Ɍ�������
	Quaternion sideRot;
	sideRot.SetRotationDegY(90.0f);
	//���邭���]�������]
	Quaternion spiralRot;
	//�^�C�}�[�ɂ��i�ނɂ�ăX�P�[����0.0f�`1.0f�ɕω�����
	float timeScale = (m_timer - surprisedTimer) / (switchingTimer - surprisedTimer);
	//�S����3�T����悤�ɂ��邭���
	float spiralAngel = (360.0f * 3) * timeScale;
	spiralRot.SetRotationDegZ(spiralAngel);
	//������������
	qRot = sideRot;
	//���邭��񂵂�
	qRot.Multiply(spiralRot);
	//�{���̉�]�ɍ��킹��
	qRot.Multiply(m_rotation);
	//�v���C���[�ɖ{���̉�]��n��
	m_player->SetRotation(m_rotation);
	//�v���C���[��UFO�ɕ߂܂����Ƃ�����̌����ڂ����̉�]��n��
	m_player->SetCapturedRotation(qRot);

	//�^�C�}�[�̈��̂Ƃ���ŏI���
	float endTimeScale = 0.95f;

	//�^�C�}�[�ɂ��i�ނɂ�ăX�P�[����0.0f�`1.0f�ɕω�����
	timeScale = (m_timer - surprisedTimer) / ((switchingTimer - surprisedTimer) * endTimeScale);
	//�ω���1.0f�`0.0f�ɂ���
	float scale = 1.0f - 1.0f * timeScale;
	//0.0f��菬������
	if (scale < 0.0f)
		//�������Ƃ���0.0f�ɂ���
		scale = 0.0f;

	//�v���C���[�̊g���ݒ肷��
	m_player->SetScale({ scale ,scale ,scale });

	//���邭��񂷉�]������
	qRot = spiralRot;
	//�{���̉�]�ɍ��킹��
	qRot.Multiply(m_rotation);
	//�v���C���[�̊�_�͑��������ǁA������ւ�𒆐S�ɉ񂷂��߂̃I�t�Z�b�g
	Vector3 offsetVec = g_VEC3_UP;
	//��]�Ńx�N�g������
	qRot.Apply(offsetVec);
	//�v���C���[�̔����̍���
	const float playerHalfHeight = 150.0f;
	//�v���C���[�̊g��ɍ��킹�ăI�t�Z�b�g���g�傷��
	offsetVec.Scale(-playerHalfHeight * scale);
	//�v���C���[��UFO�ɕ߂܂����Ƃ�����̌����ڂ����̍��W��n��
	m_player->SetCapturedPosition(m_player->GetPosition() + offsetVec);

	//�^�C�}�[�����̂Ƃ���܂ł�
	if (m_timer - surprisedTimer < ((switchingTimer - surprisedTimer) * endTimeScale))
	{
		//UFO�����ɖc��܂��鏈��

		//�^�C�}�[�ɉ�����0.0f�`1.0f�ɕω�����
		float scaleXZ = (m_timer - surprisedTimer) /
			((switchingTimer - surprisedTimer) * endTimeScale);
		//�w���֐��I�ɕω�������
		//���X�ɑ傫���Ȃ�X�s�[�h�������Ȃ�
		scaleXZ = pow(scaleXZ, 3.0f);
		//�ő��1.5�{�ɂ���
		scaleXZ *= 0.5f;
		scaleXZ += 1.0f;
		//XZ�����Ɋg�傷��
		m_modelRender->SetScale({ scaleXZ ,1.0f ,scaleXZ });
	}
	else
	{
		//�^�C�}�[�����ȍ~

		//�g���
		//����
		float denominator = ((switchingTimer - surprisedTimer) * (1.0f - endTimeScale));
		//���q
		float numerator = m_timer - surprisedTimer -
			((switchingTimer - surprisedTimer) * endTimeScale);

		//���X�Ɍ��̔{���ɖ߂�
		float scaleXZ = numerator / denominator;
		//�ő��1.5�{����1.0�ɏ��X�ɖ߂�
		scaleXZ *= 0.5f;
		scaleXZ = 1.5f - scaleXZ;
		//XZ�����Ɋg�傷��
		m_modelRender->SetScale({ scaleXZ ,1.0f ,scaleXZ });
	}

	if (m_timer >= switchingTimer)
	{
		//�^�C�}�[���؂�ւ����ԂɂȂ�����
		//�^�C�}�[������������
		m_timer = 0.0f;
		//�A�b�v�f�[�g�X�e�[�g���v���C���[���^�ԏ�����Ԃɂ���
		m_updateState = enTransport;

		m_modelRender->SetScale({ 1.0f ,1.0f ,1.0f });

	}
	else
		//�܂��Ȃ�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();

}

//�v���C���[���^�ԏ���
void CUFO::Transport()
{
	//�^�C�}�[��0.0f�̎���
	//�܂�ŏ��̈�񂾂��Ă΂��
	if (m_timer == 0.0f)
	{
		//UFO�̉�]���x��2�{�ɂ���
		m_ufoAngleSpeed = 200.0f;
		//�㉺�̃X�|�b�g���C�g�̌�������
		//m_spotLightUp->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//m_spotLightDown->SetColor({ 0.0f,0.0f,0.0f,1.0f });
		//UFO�̌����̌�������
		m_ufoLight->SetModelEmissionColor({ 0.0f,0.0f,0.0f,1.0f });
		//UFO�̌����𓧖��ɂ���
		m_ufoLight->SetAlphaValue(modelRenderConstData::ALPHA_VALUE_TRANSPARENT);
		//�֊s���������Ȃ��悤�ɂ���
		m_ufoLight->SetDrawOutLineFlag(false);

		//�E�F�C�|�C���g�̍ő�l
		const int maxWayPoint = 31;

		//���݂̍����̃E�F�C�|�C���g�̔��Α��̃E�F�C�|�C���g���o���B
		//0���܂ނ��ߍő�l��1���Z���Ă���A���Α����o�����ߔ����ɂ��āA
		//���݂̍����̃E�F�C�|�C���g�ɂ��̒l�����Z���Ă���B
		int reverseLp = GetLeftWayPointIndex() + (maxWayPoint + 1) / 2;

		//�E���������čs���̂���������s���̂����߂�
		if (GetLeftWayPointIndex() == m_ufoLandingPoint->GetLeftWayPointIndex())
		{
			//���g�̍����̃E�F�C�|�C���g�ƁA���n�_�̍����̃E�F�C�|�C���g�������Ȃ�

			//�E�̒P�ʕ����x�N�g��
			Vector3 rightVec = g_VEC3_RIGHT;
			//���g�̉�]���|����
			m_rotation.Apply(rightVec);
			//���g�iUFO�j���璅�n�_�ւ̃x�N�g��
			Vector3 UFOToLandPointVec = m_ufoLandingPoint->GetPosition() - m_position;
			//���K������
			UFOToLandPointVec.Normalize();
			//���n�_�ւ̃x�N�g���ƉE�̃x�N�g���̓��ς����
			float landVecDotRightVec = Dot(UFOToLandPointVec, rightVec);
			//���ς̐��������ō��E��U�蕪����
			if (landVecDotRightVec >= 0.0f)
				//���ɐi��
				m_leftOrRight = enLeft;
			else
				//�E�ɐi��
				m_leftOrRight = enRight;
		}
		//���Α��̃E�F�C�|�C���g���ő�l�𒴂��Ă��邩�H
		else if (reverseLp > maxWayPoint)
		{
			//�����Ă��鎞

			//�ő�l+1�i0���܂ނ��߁j�����Z����
			reverseLp -= ( maxWayPoint + 1 );

			//���n�_�̈ʒu�𒲂ׂāA�E����s����������s������U�蕪����
			if (GetLeftWayPointIndex() < m_ufoLandingPoint->GetLeftWayPointIndex() ||
				m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
				//���n�_���A���݂̃E�F�C�|�C���g�����傫����
				//���Α��̃E�F�C�|�C���g�ȉ���������
				//���ɐi��
				m_leftOrRight = enLeft;
			else
				//����ȊO�Ȃ�
				//�E�ɐi��
				m_leftOrRight = enRight;
		}
		else
		{
			//�����Ă��Ȃ���

			//���n�_�̈ʒu�𒲂ׂāA�E����s����������s������U�蕪����
			//if (m_ufoLandingPoint->GetLeftWayPointIndex() <= reverseLp)
			if (m_ufoLandingPoint->GetLeftWayPointIndex() >= GetLeftWayPointIndex())
			{
				//���n�_�����Α��̃E�F�C�|�C���g�ȉ���������
				//���ɐi��

				//���n�_�����g�̃E�F�C�|�C���g�ȏゾ������
				//���ɐi��
				m_leftOrRight = enLeft;
			}
			else
			{
				//����ȊO�Ȃ�
				//�E�ɐi��
				m_leftOrRight = enRight;
			}
		}
	}

	//�ŏ��ɉ^�ԑO�ɏ�����~���鎞��
	const float stopTimer = 0.5f;
	if (m_timer < stopTimer)
	{
		//��~���Ԃ�菬����

		//�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();
		//���������ɖ߂�
		return;
	}
	else
	{
		//�ړ����x���f�t�H���g�̑��x�ɂ���
		SetMoveSpeed();
	}

	//�A�b�v�x�N�g���𓾂�
	Vector3 upVec = m_upVec;
	//�C�C�����ɐL�΂�
	const float upVecLen = 550.0f;
	upVec.Scale(upVecLen);

	//UFO�ɕ߂܂�ʒu
	Vector3 capturePos = m_position + upVec;

	//���g��OBB�ƒ��n�_��OBB���Փ˂��Ă��邩�H
	if (CollisionOBBs(GetOBB(), m_ufoLandingPoint->GetOBB()))
	{
		//�Փ˂�����

		//�A�b�v�f�[�g�X�e�[�g�𒅒n��Ԃɂ���
		m_updateState = enLanding;
		//�^�C�}�[������������
		m_timer = 0.0f;
		m_ufoAngleSpeed = 100.0f;

		//UFOcarrymoveSE�����Ă�����~�߂�
		if (m_UFOcarrymoveSE->IsPlaying()) {
			m_UFOcarrymoveSE->Stop();
		}
	}
	else
		//�Փ˂��Ă��Ȃ�������
		//�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();

	//�v���C���[�̏����X�V����
	m_player->SetPosition(capturePos);
	m_player->SetCapturedPosition(capturePos);
	m_player->SetRotation(m_rotation);
	m_player->SetCapturedRotation(m_rotation);
	m_player->SetLeftPointIndex(GetLeftWayPointIndex());
	m_player->SetRightPointIndex(GetRightWayPointIndex());

	//UFOcarrymoveSE�����[�v�Đ��I���ōĐ�����
	m_UFOcarrymoveSE->Play(true);
}

//���n�̏���
void CUFO::Landing()
{
	//�c��ގ���
	const float inflateTimer = 1.0f;
	//�v���C���[��f���o������
	const float spitOutTimer = inflateTimer + 0.25f;
	//���΂炭�҂���
	const float waitTimer = spitOutTimer + 1.0f;

	//�A�b�v�x�N�g���𓾂�
	Vector3 upVec = m_upVec;
	//�C�C�����ɐL�΂�
	const float upVecLen = 550.0f;
	upVec.Scale(upVecLen);
	//UFO�ɕ߂܂�ʒu
	Vector3 capturePos = m_position + upVec;
	//UFO�ɕ߂܂�ʒu�ƒ��n�_�̏�����̊Ԃ̃x�N�g��
	Vector3 dist = (m_ufoLandingPoint->GetPosition() + upVec);
	dist -= capturePos;
	//�������o��
	float distLen = dist.Length();
	//�����̍ő�l
	const float maxDistLen = 1000.0f;
	//�g��̍ő�l
	const float maxScale = 1.5f;

	//�v���C���[�ɓn����]
	Quaternion qRot;
	//�������������]
	Quaternion qRotY;
	qRotY.SetRotationDegY(90.0f);
	//�����ɂ����]
	Quaternion qRotZ;
	qRotZ.SetRotationDegZ(90.0f);
	//�������̉�]��n��
	qRot = qRotY;
	//�����̉�]��n��
	qRot.Multiply(qRotZ);
	//�{���̉�]�ɍ��킹��
	qRot.Multiply(m_rotation);
	
	//�v���C���[�̊�_�͑��������ǁA������ւ�𒆐S�ɉ񂷂��߂̃I�t�Z�b�g
	Vector3 offSet = /*g_VEC3_LEFT*/g_VEC3_RIGHT;
	//���g�̉�]�ŉ�
	m_rotation.Apply(offSet);
	//�v���C���[�̍����̔���
	const float playerHalfHeight = 150.0f;
	offSet.Scale(playerHalfHeight);

	//�^�C�}�[��0.0f���H
	//���g��OBB�ƒ��n�_�̒[��OBB���Փ˂��Ă��邩�H
	if (!CollisionOBBs(GetOBB(), m_ufoLandingPoint->GetSideOBB(m_leftOrRight)) && 
		m_timer == 0.0f)
	{
		//0.0f����
		//�Փ˂��Ă��Ȃ���

		//�ړ����x���f�t�H���g�ɂ��Ă���
		SetMoveSpeed();

		//�����̍Œ�l�A�X�s�[�h�����ȉ��ɂ��Ȃ�����
		float minDistLen = 200.0f;
		if (distLen < minDistLen)
			//�������Œ�l������������
			//�Œ�l�ɂ��Ă���
			distLen = minDistLen;

		//�������߂��قǒx������
		m_moveSpeed *= distLen / maxDistLen;
	}
	else 
	{
		//0.0f�łȂ��A�܂��́A
		//�Փ˂��Ă��鎞

		//�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();

		//�^�C�}�[�̐i��𒲂ׂ�
		if (m_timer <= inflateTimer)
		{
			//�c��ގ��Ԉȉ��Ȃ�
			
			//�X�s�[�h��0�ɂ���
			m_moveSpeed = 0.0f;

			//�c��ގ��Ԃ̈��̎��Ԃ܂�
			const float endInflateTimerScale = 0.8f;

			//XZ�����̊g��
			float scaleXZ = 1.0f;
			scaleXZ += (maxScale - 1.0f) * (m_timer / (inflateTimer * endInflateTimerScale));
			if (scaleXZ > maxScale)
				scaleXZ = maxScale;

			//���f����XZ�����Ɋg��
			m_modelRender->SetScale({ scaleXZ ,1.0f,scaleXZ });

		}
		else if (m_timer <= spitOutTimer)
		{
			//�c��ގ��Ԃ��傫���A����
			//�f���o�����Ԉȉ�

			//�f���o�����Ԃ̔����̎���
			const float halfSpitOutTimer = spitOutTimer / 2.0f;
			if (m_timer <= halfSpitOutTimer)
			{
				//�^�C�}�[���f���o�����Ԃ̔����ȉ��Ȃ�

				//�^�C�}�[�ɉ�����0.0f�`1.0f�ɕω�����
				float timeScale = (m_timer - inflateTimer) / 
					(spitOutTimer - inflateTimer - halfSpitOutTimer);
				//XZ�����̊g��	//�ő�̊g�������
				float scaleXZ = maxScale;
				//���X�ɏ���������
				scaleXZ -= (maxScale - 1.0f) * timeScale;

				//XZ�����Ɋg�傷��
				m_modelRender->SetScale({ scaleXZ ,1.0f,scaleXZ });
			}
			else
			{
				//�����̎��Ԃ��߂�����
				//���̑傫���ɂ���
				m_modelRender->SetScale({ 1.0f ,1.0f,1.0f });
			}

			//���ւ̃x�N�g��
			Vector3 downVec = m_upVec;
			downVec.Scale(-1.0f);
			//�ړ���܂Ńx�N�g����L�΂�
			const float nextPosDist = 500.0f;
			downVec.Scale(nextPosDist);

			//�^�C�}�[�ɉ�����0.0f�`1.0f�ɕω�����
			float timeScale = (m_timer - inflateTimer) / (spitOutTimer - inflateTimer);
			//�v���C���[�̍��W���^�C�}�[�ɉ����ĉ�����
			downVec *= timeScale;
			capturePos += downVec;

			//�v���C���[�̊g����^�C�}�[�ɉ����Č��̑傫���Ɍ������đ傫������
			m_player->SetScale({ timeScale ,timeScale ,timeScale });

			//�I�t�Z�b�g���^�C�}�[�ɉ����đ傫������
			offSet.Scale(timeScale);
		}
		else if (m_timer <= waitTimer)
		{
			//�^�C�}�[���f���o�����Ԃ��傫���A��
			//�҂����Ԉȉ�

			//���΂炭�҂�
		}
		else
		{
			//�҂����Ԃ܂ŏI�������

			//�A�b�v�f�[�g�X�e�[�g�𒅒n�_���痣��鏈����Ԃɂ���
			m_updateState = enLeave;
		}
	}

	//�^�C�}�[���f���o�����Ԉȉ����H
	if (m_timer <= spitOutTimer)
	{
		//�f���o�����Ԉȉ��̎�

		//�v���C���[�̃p�����[�^�[���X�V����
		m_player->SetPosition(capturePos);
		m_player->SetCapturedPosition(capturePos + offSet);
		m_player->SetRotation(m_rotation);
		m_player->SetCapturedRotation(qRot);
		m_player->SetWayPointState(GetLeftWayPointIndex());
		m_player->SetLeftPointIndex(GetLeftWayPointIndex());
		m_player->SetRightPointIndex(GetRightWayPointIndex());
	}
	else if (m_updateState == enLeave)
	{
		//�A�b�v�f�[�g�p�����[�^��enLeave��ԂȂ�

		//�^�C�}�[��0�ɂ���
		m_timer = 0.0f;

	}

}

//���n�_���痣��鏈��
void CUFO::Leave()
{
	//�i�ތ������E�����ɖ߂��B
	m_leftOrRight = enRight;

	//�^�C�}�[��0.0f�̎���
	//�܂�ŏ��̈�񂾂��Ă΂��
	if (m_timer == 0.0f)
	{
		//�E�F�C�|�C���g��̍��W�ɍ���
		m_onWayPosition = m_position;

		//�X�e�[�W�̏�ɏ�鏈�����s���悤�ɂ���
		m_getOnStageFlag = true;

		//�E�F�C�|�C���g�̍ő吔
		const int maxWayPointNum = 31;
		//���ɐi�ރE�F�C�|�C���g
		int nextIndex = m_ufoLandingPoint->GetLeftWayPointIndex();
		if (m_leftOrRight == enLeft)
		{
			//���ɓ�������A���ɉ���������Ȃ�
		}
		else
		{
			//�E�ɓ������̂ŁA���̃E�F�C�|�C���g�����Z����

			nextIndex--;
			//0��菬����������
			if (nextIndex < 0)
				//������čő�l�ɂ���
				nextIndex = maxWayPointNum;
		}
		//���g�̍����̃E�F�C�|�C���g���X�V����
		SetLeftWayPointIndex(nextIndex);
	}

	//�ړ����x���f�t�H���g�̑��x�ɂ���
	SetMoveSpeed();
	//�؂�ւ��^�C��
	const float switchingTime = 2.0f;

	//�^�C�}�[�̒��ׂ�
	if (m_timer >= switchingTime)
	{
		//�؂�ւ����Ԉȏ�ɂȂ�����

		//�A�b�v�f�[�g�X�e�[�g���v���C���[��T��������Ԃɂ���
		m_updateState = enSearch;
		//�^�C�}�[������������
		m_timer = 0.0f;
		//�v���C���[��UFO�ɕ߂܂��Ă��Ȃ���Ԃɂ���
		m_player->SetCapturedUFOFlag(false);

		if (m_UFOcarrymoveSE->IsPlaying()) {
			m_UFOcarrymoveSE->Stop();
		}
	}
	else
		//�؂�ւ����Ԃ�菬����������
		//�^�C�}�[��i�߂�
		m_timer += GameTime().GetFrameDeltaTime();
}

//�ړ�����
void CUFO::Move()
{
	//���̃E�F�C�|�C���g
	int nextIndex = GetRightWayPointIndex();
	//�ŏI�I�Ȉړ����x	//�ړ����x�Ƀf���^�^�C�����|����
	float finalMoveSpeed = m_moveSpeed * GameTime().GetFrameDeltaTime();
	//�E�F�C�|�C���g��̎��̍��W���v�Z����
	m_onWayPosition = CLevelObjectManager::GetInstance()->CalcWayPointNextPos
	(GetRightWayPointIndex(), m_onWayPosition, finalMoveSpeed, m_leftOrRight, &nextIndex);

	//�E�F�C�|�C���g�̍ő吔
	const int maxWayPointNum = 31;
	if (m_leftOrRight == enLeft)
	{
		//���ɓ�������A���ɉ���������Ȃ�
	}
	else
	{
		//�E�ɓ������̂ŁA���̃E�F�C�|�C���g�����Z����

		nextIndex++;
		//�E�F�C�|�C���g�̍ő吔���傫��������
		if (nextIndex > maxWayPointNum)
			//�������0�ɂ���
			nextIndex = 0;
	}
	//���g�̍����̃E�F�C�|�C���g���X�V����
	SetLeftWayPointIndex(nextIndex);
	//UFOmoveSE
	UFOmove();
}

//���r�E�X�̗ւ̃X�e�[�W�ɃC�C�����ɍ��킹�鏈��
void CUFO::GetOnStage()
{
	if (!m_getOnStageFlag)
		return;
	//Up�x�N�g�����C�C�����ɐL�΂��B
	//�L�΂����悪���C�̎n�_�ƂȂ�
	Vector3 upVec = g_VEC3_UP;
	m_rotation.Apply(upVec);
	upVec.Scale(500.0f);

	//�f�o�b�N�p
#ifdef MY_DEBUG
	//��ŏ���
	m_dbgRay[0]->SetPosition(m_onWayPosition + upVec);
	m_dbgRay[1]->SetPosition(m_onWayPosition - upVec);
#endif
	//�f�o�b�N�p�����܂�
	if (m_mobius)
	{
		//���r�E�X�̗ւ��������Ă�����
		//���r�E�X�̗ւ̃��f���̃|���S���ƁA���C�̓����蔻������
		m_mobius->GetIntersectPosWithMobius(
			m_onWayPosition + upVec, m_onWayPosition - upVec, &m_position
		);
	}
	else
	{
		//���r�E�X�̗ցi�X�e�[�W�j���������Ă��Ȃ�������
		//�T����return����
		m_mobius = FindGO<CMobius>("Mobius");
		return;
	}
}

/// <summary>
/// ��]����
/// </summary>
/// <returns>�v�Z���ꂽ��]</returns>
const Quaternion CUFO::Rotating()
{
	//UFO�̉�]�̊p�x�𑝂₷	//�f���^�^�C�����|���Ă���
	m_ufoAngle += m_ufoAngleSpeed * GameTime().GetFrameDeltaTime();

	//�p�x��360�x���傫�����H
	if (m_ufoAngle > 360.0f)
	{
		//�傫���Ƃ���

		//���������
		m_ufoAngle = m_ufoAngle - 360.0f;
	}

	//�n����]
	Quaternion qRot;
	//Y�����S�ɂ��邭���
	qRot.SetRotationDegY(-m_ufoAngle);
	//�{���̉�]�ɂ��킹��
	qRot.Multiply(m_rotation);

	return qRot;
}

//���C�g�̍X�V
void CUFO::UpdateLight()
{
	//���C�g�̍X�V
	Vector3 upVec = m_upVec;
	upVec.Scale(300.0f);
	//�Ƃ炷�ʒu
	//m_spotLightUp->SetAngle(10.0f);
	//m_spotLightUp->SetPosition(m_position + upVec);
	m_ufoLight->SetPosition(m_position);
	m_ufoLight->SetRotation(m_rotation);
	//�Ƃ炷����
	//m_spotLightUp->SetDirection(m_upVec);

	upVec = m_upVec;
	upVec.Scale(500.0f);
	//m_spotLightDown->SetPosition(m_position + upVec);
	Vector3 downVec = m_upVec;
	downVec.Scale(-1.0f);
	//m_spotLightDown->SetDirection(downVec);



#ifdef MY_DEBUG
	m_dbgSpotLigPos->SetPosition(m_position + upVec);
	m_dbgSpotLigPos->SetRotation(m_rotation);
#endif
}