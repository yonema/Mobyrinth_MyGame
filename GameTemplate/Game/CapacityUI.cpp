#include "stdafx.h"
#include "CapacityUI.h"

//�X�^�[�g�֐�
bool CCapacityUI::Start()
{
	//�X�v���C�g�̏�����
	InitSprite();

	//�t�H���g�̏�����
	InitFont();

	return true;

}

//�X�v���C�g�̏�����
void CCapacityUI::InitSprite()
{
	//�L���p�V�e�B��\������X�v���C�g�̐����Ə�����
	m_capacityDisplaySR = NewGO<CSpriteRender>(1);
	const float capacityX = -490.0f;
	const float capacityY = 270.0f;
	m_capacityDisplaySR->Init
	("Assets/image/Capacity_display.DDS", 512.0f * 1.1f, 256.0f * 1.1f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_capacityDisplaySR->SetPosition({ capacityX, capacityY,0.0f });
	m_capacityDisplaySR->SetPostRenderFlag(true);
	//�^�C�g����ʂ��H
	if (m_titleFlag)
	{
		//�^�C�g����ʂȂ�
		//��\���ɂ���
		m_capacityDisplaySR->Deactivate();
	}
}


//�t�H���g�̏�����
void CCapacityUI::InitFont()
{
	//�\���Ɨ����̔��]�I�u�W�F�N�g�̐���\������
	//�t�H���g�����_���[�̏�����
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		m_RONumFR[i] = NewGO<CFontRender>(1);
		m_RONumFR[i]->SetPostRenderFlag(true);
		//�ő吔��ݒ肢
		CLevelObjectManager::GetInstance()->SetReversibleObjectMaxNum(i, 5);
	}

	//�����\���̃t�H���g�̐ݒ聖��
	//�t�H���g�̊g��
	const float scale = 1.5f;
	//�t�H���g�̃J���[
	Vector3 fontColor;
	//�\���̃^�C���̐F
	fontColor = { 214.0f,85.0f,0.0f };
	//���邷���邽�ߐ��K��
	fontColor.Normalize();
	m_normalFontColor[enFrontSide] = fontColor;
	//�t�H���g��X���W
	const float capacityNumX = -615.0f;
	//�t�H���g��Y���W
	const float capacityNumY = 332.5f;
	//�\���̃t�H���g�̏�����
	m_RONumFR[enFrontSide]->Init(L"",
		{ capacityNumX,capacityNumY }, { fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });

	//���������̃t�H���g�̐ݒ聖��
	//�t�H���g�̃J���[
	//�����̃^�C���̐F
	fontColor = { 99.0f,97.0f,99.0f };
	//���邷���邽�ߐ��K��
	fontColor.Normalize();
	//�F���ꏏ�߂��Č����ɂ�����������A�Â�����
	fontColor.Scale(0.1);
	m_normalFontColor[enBackSide] = fontColor;
	//�\���̃t�H���g�Ƃ�Y���W�̍�
	const float capacityNumDiff = 70.0f;
	//�����̃t�H���g�̏�����
	m_RONumFR[enBackSide]->Init(L"",
		{ capacityNumX,capacityNumY - capacityNumDiff }, { fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });

	//�^�C�g����ʂ��H
	if (m_titleFlag)
	{
		//�^�C�g����ʂȂ�

		//��\���ɂ���
		m_RONumFR[enFrontSide]->Deactivate();
		m_RONumFR[enBackSide]->Deactivate();
	}
}

//�f�X�g���N�^
CCapacityUI::~CCapacityUI()
{
	//�L���p�V�e�B��\������X�v���C�g�̔j��
	DeleteGO(m_capacityDisplaySR);

	//�\���Ɨ����ɂ���A�C�e���̐��̃t�H���g�̔j��
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		DeleteGO(m_RONumFR[i]);
	}
}

void CCapacityUI::Update()
{
	//���o�̊���U��
	Directing();


	//�e�L�X�g�p��
	wchar_t text[256];
	//���݂̕\���Ɨ����̔��]�I�u�W�F�N�g�̐����擾
	const int* num = CLevelObjectManager::GetInstance()->GetReversibleObjectNum();
	const int* maxNum = CLevelObjectManager::GetInstance()->GetReversibleObjectMaxNum();
	//�e�L�X�g���Z�b�g
	std::wstring wString = L"��";
	swprintf(text, L"�\:%d /%d", num[enFrontSide], maxNum[enFrontSide]);
	m_RONumFR[enFrontSide]->SetText(text);
	swprintf(text, L"��:%d /%d", num[enBackSide], maxNum[enBackSide]);
	m_RONumFR[enBackSide]->SetText(text);

	//�ǂ̉��o���N�������`�F�b�N����
	CheckDirecting(num, maxNum);


	//���̕\�Ɨ��̐����A�O�̕\�Ɨ��̐��ɑ������
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		m_oldRONum[i] = num[i];
	}
}

/// <summary>
/// �ǂ̉��o���N�������`�F�b�N����
/// </summary>
/// <param name="num">�\���Ɨ����̃A�C�e���̐�</param>
/// <param name="maxNum">�\���Ɨ����̃A�C�e���̍ő吔</param>
void CCapacityUI::CheckDirecting(const int* num, const int* maxNum)
{
	//�\���Ɨ����������ׂ�
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		//�O�̐��ƌ��݂̐����ς���Ă��邩�H
		if (num[i] != m_oldRONum[i])
		{
			//�ς���Ă���

			//�^�C�}�[������������
			m_timer[i] = 0.0f;

			//���݂̐��𒲂ׂ�
			if (num[i] > maxNum[i])
				//���݂̐����ő�l���傫��
				//�I�[�o�[����
				m_directingState[i] = enOver;
			else if (num[i] > m_oldRONum[i])
				//���݂̐����O�̐����傫��
				//������
				m_directingState[i] = enIncreased;
			else if (num[i] < m_oldRONum[i])
				//���݂̐����O�̐���菬����
				//������
				m_directingState[i] = enDecreased;

		}
	}
}


//���o�̊���U��
void CCapacityUI::Directing()
{
	//�\���Ɨ����������ׂ�
	for (int i = 0; i < enFrontAndBackSideNum; i++)
	{
		//���o�̃X�e�[�g�Ŋ���U��
		switch (m_directingState[i])
		{
		case enNormal:
			//�ʏ�
			//�������Ȃ�
			break;
		case enIncreased:
			//������
			Increased(i);
			break;
		case enDecreased:
			//������
			Decreased(i);
			break;
		case enOver:
			//�I�[�o�[����
			Over(i);
			break;
		}
	}
}

/// <summary>
/// ���������o
/// </summary>
/// <param name="frontOrBackSide">�\�����������H</param>
void CCapacityUI::Increased(const int frontOrBackSide)
{
	//�؂�ւ�����
	const float switchingTime = 2.0f;

	//�t�H���g�̃J���[�𑝂������̃J���[�ɂ���
	m_RONumFR[frontOrBackSide]->SetColor({ 10.0f,0.0f,0.0f,1.0f });

	//�^�C�}�[���؂�ւ����Ԉȏォ
	if (m_timer[frontOrBackSide] >= switchingTime)
	{
		//�ȏ�

		//�^�C�}�[������������
		m_timer[frontOrBackSide] = 0.0f;
		//�t�H���g�̃J���[������������
		m_RONumFR[frontOrBackSide]->SetColor(m_normalFontColor[frontOrBackSide]);
		//���o�̃X�e�[�g��ʏ�ɂ���
		m_directingState[frontOrBackSide] = enNormal;
	}
	else
	{
		//����

		//�^�C�}�[��i�߂�
		m_timer[frontOrBackSide] += GameTime().GetFrameDeltaTime();
	}
}

/// <summary>
/// ���������o
/// </summary>
/// <param name="frontOrBackSide">�\�����������H</param>
void CCapacityUI::Decreased(const int frontOrBackSide)
{
	//�؂�ւ�����
	const float switchingTime = 2.0f;

	//�t�H���g�̃J���[�����������̃J���[�ɂ���
	m_RONumFR[frontOrBackSide]->SetColor({ 0.0f,0.0f,20.0f,1.0f });

	//�^�C�}�[���؂�ւ����Ԉȏォ
	if (m_timer[frontOrBackSide] >= switchingTime)
	{
		//�ȏ�

		//�^�C�}�[������������
		m_timer[frontOrBackSide] = 0.0f;
		//�t�H���g�̃J���[������������
		m_RONumFR[frontOrBackSide]->SetColor(m_normalFontColor[frontOrBackSide]);
		//���o�̃X�e�[�g��ʏ�ɂ���
		m_directingState[frontOrBackSide] = enNormal;
	}
	else
	{
		//����

		//�^�C�}�[��i�߂�
		m_timer[frontOrBackSide] += GameTime().GetFrameDeltaTime();
	}
}

/// <summary>
/// �I�[�o�[�������o
/// </summary>
/// <param name="frontOrBackSide">�\�����������H</param>
void CCapacityUI::Over(const int frontOrBackSide)
{
	//�؂�ւ�����
	const float switchingTime = 2.0f;

	//�t�H���g�̃J���[�𑝂������̃J���[�ɂ���
	m_RONumFR[frontOrBackSide]->SetColor({ 10.0f,0.0f,0.0f,1.0f });

	//�^�C�}�[���؂�ւ����Ԉȏォ
	if (m_timer[frontOrBackSide] >= switchingTime)
	{
		//�ȏ�

		//�^�C�}�[������������
		m_timer[frontOrBackSide] = 0.0f;
		//�t�H���g�̃J���[������������
		m_RONumFR[frontOrBackSide]->SetColor(m_normalFontColor[frontOrBackSide]);
		//���o�̃X�e�[�g��ʏ�ɂ���
		m_directingState[frontOrBackSide] = enNormal;
	}
	else
	{
		//����

		//�^�C�}�[��i�߂�
		m_timer[frontOrBackSide] += GameTime().GetFrameDeltaTime();
	}
}