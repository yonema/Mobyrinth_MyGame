#include "stdafx.h"

#include "ROmizu_kori.h"

#include "TransparentSwitch.h"

//�R���X�g���N�^
OOTransparentSwitch::OOTransparentSwitch()
{
	//��ɃJ���[��ݒ肷��

	//�M���̐̐F
	Vector3 color = { 71.0f,225.0f,241.0f };
	//���邷���邩�琳�K������
	color.Normalize();
	//����ǂ͈Â����邩�炿����Ɩ��邭����
	color.Scale(1.5f);
	//�ʏ�̃t�H���g�̃J���[��ݒ�
	m_normalColor = { color.x,color.y,color.z,1.0f };

	//�_�Ŏ��̃t�H���g�̃J���[��ݒ�
	m_blinkColor = { 1.0f,0.0f,0.0f,1.0f };


	//�E�F�C�|�C���g����̉��s�̋�����ݒ�
	SetZPosLen(100.0f);
}
//�X�^�[�g�֐�
bool OOTransparentSwitch::StartSub()
{
	Init("Assets/modelData/transparentSwitch.tkm", enTransparentSwitch);

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//�����I�u�W�F�N�g�𔼓����ɂ���B
	ChangeTransparent();

	//�{�^���������ꂽ�Ƃ��p�̃��f�������_���[�̐����Ə�����
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init("Assets/modelData/transparentSwitch2.tkm", D3D12_CULL_MODE_NONE);
	m_modelRender->SetPosition(m_position);
	m_modelRender->SetRotation(m_rotation);
	//��\���ɂ���
	m_modelRender->Deactivate();

	//�J������T��	//����͊m���Ɍ�����
	m_gameCamera = FindGO<GameCamera>("GameCamera");

	//UFO��T��	//�������͌����邩������Ȃ��B
	m_ufo = FindGO<CUFO>("UFO");

	//�t�F�[�h�Ɏg���X�v���C�g�̐����Ə�����
	m_fadeSR = NewGO<CSpriteRender>(0);
	m_fadeSR->Init("Assets/Image/black.DDS", 1280.0f, 780.0f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_fadeSR->SetPosition({ 0.0f,0.0f,0.0f });
	m_fadeSR->SetPostRenderFlag(true);
	//�����ɂ��Ă���
	m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
	//��\���ɂ���
	m_fadeSR->Deactivate();

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 100.0f,200.0f,600.0f };
	//OBB�̕����x�N�g���̒�����ݒ�
	GetOBB().SetDirectionLength(obbSize);

	
	//m_buttonpushSE�̃T�E���h�L���[�𐶐�����
	m_buttonpushSE = NewGO<CSoundCue>(0);
	//m_buttonpushSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_buttonpushSE->Init(L"Assets/sound/buttonpush.wav");
	//���ʒ���
	m_buttonpushSE->SetVolume(0.5f);

	//m_activationSE�̃T�E���h�L���[�𐶐�����
	m_activationSE = NewGO<CSoundCue>(0);
	//m_activationSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_activationSE->Init(L"Assets/sound/activation.wav");
	//���ʒ���
	m_activationSE->SetVolume(0.5f);

	//m_invalidationSE�̃T�E���h�L���[�𐶐�����
	m_invalidationSE = NewGO<CSoundCue>(0);
	//m_activationSE�̃T�E���h�L���[���Awave�t�@�C�����w�肵�ď���������B
	m_invalidationSE->Init(L"Assets/sound/invalidation.wav");
	//���ʒ���
	m_invalidationSE->SetVolume(0.5f);

	//m_activation�G�t�F�N�g�̍쐬
	m_activation = NewGO<Effect>(0);
	m_activation->Init(u"Assets/effect2/activation.efk");
	float scale = 200.0f;								//�������̂ő傫�����Ă���
	m_activation->SetScale({ scale ,scale ,scale });
	m_activation->SetPosition(m_position);				//���W��n��
	m_activation->SetRotation(m_rotation);

	return true;
}

//�f�X�g���N�^
OOTransparentSwitch::~OOTransparentSwitch()
{
	//���f�������_���[�̔j��
	DeleteGO(m_modelRender);

	//�^�C�}�[�̃t�H���g�̔j��
	for (auto timerFR : m_timerFR)
	{
		DeleteGO(timerFR);
	}

	//m_activationSE�̍폜
	DeleteGO(m_activationSE);

	//m_buttonpushSE�̍폜
	DeleteGO(m_buttonpushSE);
}

//�A�b�v�f�[�g�֐�
void OOTransparentSwitch::UpdateSub()
{

	if (m_switchingFlag)
	{
		Switching();
		return;
	}
	//���Z�b�g�^�C�}�[���O�ɂȂ����Ƃ�
	//�����I�u�W�F�N�g�����ׂĎ����グ���Ȃ��悤�ɂ���B
	//�����I�u�W�F�N�g�������Ă���ꍇ�A���̃I�u�W�F�N�g�������Ă��Ȃ���Ԃɂ���B
	//�����I�u�W�F�N�g�������ʒu�ɖ߂��B
	if (m_flagSwitchOn == true) {
		m_resetTimer -= GameTime().GetFrameDeltaTime();

		//�^�C�}�[�̃t�H���g�̍X�V
		UpdateTimerFR();
		///--m_resetTimer;

		if (m_resetTimer <= 0.0f) {
			m_flagSwitchOn = false;

			//�����ɓ����I�u�W�F�N�g�̏�����ǉ�����B
			//�z�u���Ă��邷�ׂẴ��x���I�u�W�F�N�g�̎Q�Ƃ̃x�N�^�[
			//std::vector<ILevelObjectBase*> levelObjects =
			//	CLevelObjectManager::GetInstance()->GetLevelObjects();

			////���x���I�u�W�F�N�g������������o��
			//for (auto lo : levelObjects)
			//{
			//	//�������g�̎��̓X�L�b�v
			//	if (lo == this)
			//		continue;

			//	//�����I�u�W�F�N�g�̏ꍇ
			//	if (GetFlagTransparentObject() == true)
			//	{
			//		//�����ɃI�u�W�F�N�g�ɑ΂��鏈��
			//		lo->TransparentSwitchOff();

			//		//�I�u�W�F�N�g�������Ă���ꍇ
			//		lo->SetFlagHeldPlayer(false);
			//	}
			//}

			//�����I�u�W�F�N�g�𔼓����ɂ���B
			ChangeTransparent();

			//�^�C�}�[�̃t�H���g���\���ɂ���
			for (auto timerFR : m_timerFR)
			{
				timerFR->Deactivate();
			}

			//������Ă��Ȃ����̃��f�������_���[��\������
			GetModelRender()->Activate();
			//�����ꂽ�Ƃ��̃��f�������_���[���펞�ɂ���
			m_modelRender->Deactivate();

			//m_invalidationSE�����[�v�Đ����I�t�ōĐ�����B
			m_invalidationSE->Play(false);

		}
	}
	//���Z�b�g�^�C�}�[���O�̂Ƃ��ɉ��̕��̏������쓮������B
	else if (m_flagSwitchOn == false) {
		//�X�C�b�`�������ꂽ�Ƃ�
		//�����I�u�W�F�N�g�����ׂĎ����グ����悤�ɂ���B
		//�X�C�b�`�̃I�u�W�F�N�g�͈͓̔���A�{�^���������ꂽ�Ƃ�
		if (IsHitPlayer() && g_pad[0]->IsTrigger(enButtonA)) {

			//�؂�ւ����t���O�𗧂Ă�
			m_switchingFlag = true;
			//�؂�ւ����^�C�}�[������������
			m_switchingTimer = 0.0f;

			//�v���C���[�𑀍�s�\�ɂ���
			m_pPlayer->SetOperationFlag(false);

			//UFO�����邩�H
			if (m_ufo)
				//����Ƃ�
				//UFO�𓮂��Ȃ�����
				m_ufo->SetMoveSpeed(0.0f);

			//���Z�b�g�^�C�}�[�ɊJ�n����l����
			m_resetTimer = m_resetTimerStartValue;
			//������Ă��Ȃ����̃��f�������_���[���\���ɂ���
			GetModelRender()->Deactivate();
			//�����ꂽ�Ƃ��̃��f�������_���[��\������
			m_modelRender->Activate();

			m_fadeSR->Activate();

			//m_buttonpushSE�����[�v�Đ����I�t�ōĐ�����B
			m_buttonpushSE->Play(false);
		}
	}
}

void OOTransparentSwitch::Switching()
{
	const float startWaitTime = 0.5f;							//�ŏ��̑҂���		 
	const float startFadeOutTime = startWaitTime + 0.5f;		//�ŏ��̃t�F�[�h�A�E�g����
	const float startFadeWaitTime = startFadeOutTime + 0.5f;	//�ŏ��̃t�F�[�h���Ԏ���
	const float startFadeInTime = startFadeWaitTime + 0.5f;		//�ŏ��̃t�F�[�h�C������
	const float switchingTime = startFadeInTime + 2.0f;			//�؂�ւ�������
	const float endFadeOutTime = switchingTime + 0.5f;			//�I���̃t�F�[�h�A�E�g����
	const float endFadeWaitTime = endFadeOutTime + 0.5f;		//�I���̃t�F�[�h���Ԏ���
	const float endFadeInTime = endFadeWaitTime + 0.5f;			//�I���̃t�F�[�h�C������

	//�؂�ւ����̃^�C�}�[�̌o�߂𒲂ׂ�
	if (m_switchingTimer < startWaitTime)
	{
		//�ŏ��̑҂���

		//���������ɑ҂�
	}
	else if (m_switchingTimer < startFadeOutTime)
	{
		//�t�F�[�h�A�E�g���Ă���

		//�A���t�@�l
		float alphaValue = 1.0f;
		//�^�C�}�[�Ɍo�ߋ�ɂ���ĕ⊮
		float timeScale = (m_switchingTimer - startWaitTime) / (startFadeOutTime - startWaitTime);
		alphaValue *= timeScale;
		//�t�F�[�h�����X�ɈÂ����Ă���
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}
	else if (m_switchingTimer < startFadeWaitTime)
	{
		//�t�F�[�h�A�E�g�ƃt�F�[�h�C���̊�
		//���������ɑ҂�

		//�t�F�[�h�͐^����
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });

		//�J�������v���C���[�����Ȃ�����
		m_gameCamera->SetLookPlayerFlag(false);
		//�J�����ւ̍��W
		Vector3 toCameraPos = { 0.0f,0.0f,3500.0f };
		//�J�����̎n�_��ݒ�
		m_gameCamera->SetPosition(toCameraPos);
		//�J�����̒����_��ݒ�
		m_gameCamera->SetTarget({ 0.0f,0.0f,0.0f });

	}
	else if (m_switchingTimer < startFadeInTime)
	{
		//�t�F�[�h�C�����Ă���

		//�A���t�@�l
		float alphaValue = 1.0f;
		//�^�C�}�[�Ɍo�ߋ�ɂ���ĕ⊮
		float timeScale = (m_switchingTimer - startFadeWaitTime) / (startFadeInTime - startFadeWaitTime);
		alphaValue -= 1.0f * timeScale;
		//�t�F�[�h�����X�ɖ��邭���Ă���
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}
	else if (m_switchingTimer < switchingTime)
	{
		//�����I�u�W�F�N�g�̐؂�ւ���

		//�؂�ւ������Ԃ̔����̎���
		const float halfSwitchingTime = 
			startFadeInTime + (switchingTime - startFadeInTime) / 2.0f;

		//�����̎��Ԃ�菬�������H
		if (m_switchingTimer < halfSwitchingTime)
		{
			//�������Ƃ�

			//�t�F�[�h�𓧖��ɂ���
			m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		}
		else
		{
			//�傫���Ƃ�
			
			//��ڂ̃^�C�}�[�̃t�H���g�͗L�����ł͂Ȃ����H
			if (!(*m_timerFR.begin())->IsActive())
			{
				//�L�����ł͂Ȃ�

				//�^�C�}�[�̃t�H���g��L��������
				for (auto timerFR : m_timerFR)
				{
					timerFR->Activate();
				}
				//�_�Ń^�C�}�[������������
				m_blinkTimer = FLT_MAX;
				//�t�H���g�̃J���[��ʏ펞�̃J���[�ɂ���
				m_fontColor = m_normalColor;

				//�����I�u�W�F�N�g�����̂ɂ���B
				ChangeEntity();
				//m_activationSE�����[�v�Đ����I�t�ōĐ�����B
				m_activationSE->Play(false);
			}
			else
			{
				//�L�����ł���

				//���������ɑ҂�
			}
		}
	}
	else if (m_switchingTimer < endFadeOutTime)
	{
		//�t�F�[�h�A�E�g���Ă���

		//�A���t�@�l
		float alphaValue = 1.0f;
		//�^�C�}�[�Ɍo�ߋ�ɂ���ĕ⊮
		float timeScale = (m_switchingTimer - switchingTime) / (endFadeOutTime - switchingTime);
		alphaValue *= timeScale;
		//�t�F�[�h�����X�ɈÂ����Ă���
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });
	}
	else if (m_switchingTimer < endFadeWaitTime)
	{
		//�t�F�[�h�A�E�g�ƃt�F�[�h�C���̊�
		//���������ɑ҂�

		//�t�F�[�h�͐^����
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
		//�J�������v���C���[������悤�ɂ���
		m_gameCamera->SetLookPlayerFlag(true);
	}
	else if (m_switchingTimer < endFadeInTime)
	{
		//�t�F�[�h�C�����Ă�

		//�A���t�@�l
		float alphaValue = 1.0f;
		//�^�C�}�[�Ɍo�ߋ�ɂ���ĕ⊮
		float timeScale = (m_switchingTimer - endFadeWaitTime) / (endFadeInTime - endFadeWaitTime);
		alphaValue -= 1.0f * timeScale;
		//�t�F�[�h�����X�ɖ��邭���Ă���
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,alphaValue });

		//�^�C�}�[�̃t�H���g�̍X�V
		UpdateTimerFR();
	}
	else
	{
		//�I��

		//�؂�ւ����t���O��܂�
		m_switchingFlag = false;
		//�؂�ւ����̃^�C�}�[������������
		m_switchingTimer = 0.0f;
		//�t�F�[�h�𓧖��ɂ���
		m_fadeSR->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });
		//�t�F�[�h���\���ɂ���
		m_fadeSR->Deactivate();

		//�X�C�b�`�I���t���O�𗧂Ă�
		m_flagSwitchOn = true;

		//�v���C���[�𑀍�\�ɂ���
		m_pPlayer->SetOperationFlag(true);

		//UFO�����邩�H
		if (m_ufo)
			//����Ƃ�
			//UFO�𓮂���
			m_ufo->SetMoveSpeed();

	}

	//�؂�ւ������H
	if (m_switchingFlag)
		//�؂�ւ���
		//�؂�ւ����^�C�}�[��i�߂�
		m_switchingTimer += GameTime().GetFrameDeltaTime();

}



/// <summary>
/// �^�C�}�[�̃t�H���g�̍X�V
/// </summary>
void OOTransparentSwitch::UpdateTimerFR()
{
	//���x���I�u�W�F�N�g������Ă���
	std::vector<ILevelObjectBase*> levelObjects
		= CLevelObjectManager::GetInstance()->GetLevelObjects();

	//�^�C�}�[�̃t�H���g�̃C�e���[�^�[
	std::list<CFontRender*>::iterator itr = m_timerFR.begin();

	//�_�ł��n�߂�^�C��
	const float blinkStartTime = 3.1f;

	//�_�ł��n�߂�^�C�����^�C�}�[�����������H
	if (m_resetTimer <= blinkStartTime)
	{
		//������
		//�_�ł�����

		//�_�ł���^�C��
		const float blinkTime = 0.1f;

		//�_�Ń^�C�}�[���_�ł���^�C�����傫�����H
		if (m_blinkTimer > blinkTime)
		{
			//�傫��������
			//�_�ł�����

			//�t�H���g�̃J���[�͒ʏ�̃J���[���H
			if (m_fontColor.x == m_normalColor.x)
				//�ʏ�̃J���[�Ȃ�
				//�_�Ŏ��̃J���[�ɂ���
				m_fontColor = m_blinkColor;
			else
				//�Ⴄ�Ȃ�
				//�ʏ�̃J���[�ɂ���
				m_fontColor = m_normalColor;

			//�_�Ń^�C�}�[��0.0f�ɂ���
			m_blinkTimer = 0.0f;
		}
		else
		{
			//�������Ƃ�

			//�_�Ń^�C�}�[��i�߂�
			m_blinkTimer += GameTime().GetFrameDeltaTime();
		}
	}

	//�S�Ẵ��x���I�u�W�F�N�g�Ɍ���
	for (int i = 0; i < levelObjects.size(); i++)
	{
		//�����I�u�W�F�N�g�ł͂Ȃ��ꍇ�A���̃I�u�W�F�N�g������
		if (levelObjects[i]->GetFlagTransparentObject() == false) {
			continue;
		}

		//�^�C�}�[�̃t�H���g�̃p�����[�^�[��ݒ肷��
		SetTimerFRParam(itr, levelObjects[i]);

		//�C�e���[�^�[��i�߂�
		itr++;
	}
}

/// <summary>
/// �^�C�}�[�̃t�H���g�̃p�����[�^�[��ݒ肷��
/// </summary>
/// <param name="itr">�^�C�}�[�̃t�H���g�̃C�e���[�^�[</param>
/// <param name="levelObject">�Ή����铧���I�u�W�F�N�g</param>
void OOTransparentSwitch::SetTimerFRParam
(std::list<CFontRender*>::iterator itr, const ILevelObjectBase* levelObject)
{
	//�^�C�}�[�̃t�H���g�̍��W
	Vector2 timerPos;
	//�Ή����铧���I�u�W�F�N�g�̉�]�������Ă���
	Quaternion qRot = levelObject->GetRotation();
	//�A�b�v�x�N�g��
	Vector3 upVec = g_vec3Up;
	//��]�ŉ�
	qRot.Apply(upVec);
	//�A�b�v�x�N�g���̒���
	const float upLen = 300.0f;
	//�A�b�v�x�N�g�����g�傷��
	upVec.Scale(upLen);
	//�t�H���g�̓s�{�b�g�������ĂȂ����玩���ł��炷
	const Vector2 offset = { -40.0f,40.0f };
	//���[���h���W����X�N���[�����W�֕ϊ�����
	g_camera3D->CalcScreenPositionFromWorldPosition
	(timerPos, levelObject->GetPosition() + upVec);
	//�s�{�b�g�̕����炷
	timerPos.x += offset.x;
	timerPos.y += offset.y;
	//�^�C�}�[�̃t�H���g�̍��W��ݒ肷��
	(*itr)->SetPosition(timerPos);

	//�^�C�}�[�̃t�H���g�ɐݒ肷��e�L�X�g
	wchar_t text[32];
	//���̂܂܃^�C�}�[�������0.9�b�`0.0�b�̊Ԃ�����0���\������Ă��܂��̂�
	//1.0f���Z���āA0��������Ƃ͕\������������0.1f���Z����
	swprintf_s(text, L"%d", static_cast<int>(m_resetTimer + 0.9f));
	//�^�C�}�[�̃t�H���g�Ƀe�L�X�g��ݒ肷��
	(*itr)->SetText(text);
	//�^�C�}�[�̃t�H���g�̃J���[��ݒ肷��
	(*itr)->SetColor(m_fontColor);
}

/// <summary>
/// �^�C�}�[�̃t�H���g��������̂��ݒ肷��
/// </summary>
/// <param name="num">����</param>
void OOTransparentSwitch::SetTimerFRNum(const int num)
{
	//�T�C�Y���w�肷��
	m_timerFR.resize(num);
	//�^�C�}�[�̃t�H���g�̃C�e���[�^�[��p�ӂ���
	std::list<CFontRender*>::iterator itr = m_timerFR.begin();

	//�t�H���g�̃J���[��ʏ�̃J���[�ɐݒ肷��
	m_fontColor = m_normalColor;

	//�w�肵�����A�t�H���g�𐶐����A����������
	for (; itr != m_timerFR.end(); itr++)
	{
		(*itr) = NewGO<CFontRender>(0);
		(*itr)->Init(L"10", { 0.0f,0.0f }, m_fontColor);
		(*itr)->SetPostRenderFlag(true);
		//��\���ɂ���
		(*itr)->Deactivate();
	}
}

/// <summary>
	/// �I�u�W�F�N�g�𔼓����ɂ���
	/// </summary>
void OOTransparentSwitch::ChangeTransparent()
{
	//���x���I�u�W�F�N�g������Ă���
	std::vector<ILevelObjectBase*> levelObjects
		= CLevelObjectManager::GetInstance()->GetLevelObjects();
	//�S�Ẵ��x���I�u�W�F�N�g�Ɍ���
	for (int i = 0; i < levelObjects.size(); i++)
	{
		//�����I�u�W�F�N�g�ł͂Ȃ��ꍇ�A���̃I�u�W�F�N�g������
		if (levelObjects[i]->GetFlagTransparentObject() == false) {
			continue;
		}

		//���f���̎Q�Ƃ𓾂Ă���ASetMulColor()���Ă�
		//Obstacle�̏ꍇ�͖��ʂɓ��Ă΂�邯�ǁA�䖝���悤�B
		levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });
		levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,0.5f });

		levelObjects[i]->TransparentSwitchOff();
	}
}

/// <summary>
	/// �I�u�W�F�N�g�����̂ɂ���
	/// </summary>
void OOTransparentSwitch::ChangeEntity()
{
	//���x���I�u�W�F�N�g������Ă���
	std::vector<ILevelObjectBase*> levelObjects
		= CLevelObjectManager::GetInstance()->GetLevelObjects();

	//�^�C�}�[�̃t�H���g�̃C�e���[�^�[
	std::list<CFontRender*>::iterator itr = m_timerFR.begin();

	//�S�Ẵ��x���I�u�W�F�N�g�Ɍ���
	for (int i = 0; i < levelObjects.size(); i++)
	{
		//�����I�u�W�F�N�g�ł͂Ȃ��ꍇ�A���̃I�u�W�F�N�g������
		if (levelObjects[i]->GetFlagTransparentObject() == false) {
			continue;
		}

		//���f���̎Q�Ƃ𓾂Ă���ASetMulColor()���Ă�
		//Obstacle�̏ꍇ�͖��ʂɓ��Ă΂�邯�ǁA�䖝���悤�B
		levelObjects[i]->GetModelRender(CReversibleObject::enFront)->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });
		levelObjects[i]->GetModelRender(CReversibleObject::enBack)->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });

		levelObjects[i]->TransparentSwitchOn();

		//�^�C�}�[�̃t�H���g�̃p�����[�^��ݒ肷��
		SetTimerFRParam(itr, levelObjects[i]);



		//�C�e���[�^�[��i�߂�
		itr++;

	}
}