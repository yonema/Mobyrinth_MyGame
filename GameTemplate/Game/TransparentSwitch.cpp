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
}

//�A�b�v�f�[�g�֐�
void OOTransparentSwitch::UpdateSub()
{
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

		}
	}
	//���Z�b�g�^�C�}�[���O�̂Ƃ��ɉ��̕��̏������쓮������B
	else if (m_flagSwitchOn == false) {
		//�X�C�b�`�������ꂽ�Ƃ�
		//�����I�u�W�F�N�g�����ׂĎ����グ����悤�ɂ���B
		//�X�C�b�`�̃I�u�W�F�N�g�͈͓̔���A�{�^���������ꂽ�Ƃ�
		if (IsHitPlayer() && g_pad[0]->IsTrigger(enButtonA)) {
			m_flagSwitchOn = true;
			//���Z�b�g�^�C�}�[�ɊJ�n����l����
			m_resetTimer = m_resetTimerStartValue;


			//�^�C�}�[�̃t�H���g��\������
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

			//������Ă��Ȃ����̃��f�������_���[���\���ɂ���
			GetModelRender()->Deactivate();
			//�����ꂽ�Ƃ��̃��f�������_���[��\������
			m_modelRender->Activate();

		}
	}
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
