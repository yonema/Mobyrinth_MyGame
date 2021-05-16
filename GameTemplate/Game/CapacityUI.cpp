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
	//�X�v���C�g��X���W
	const float capacityX = -490.0f;
	//�X�v���C�g��Y���WS
	const float capacityY = 270.0f;
	//�X�v���C�g�̏�����
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
		//�\���Ɨ����ɂ���A�C�e���̐��̃t�H���g
		m_RONumFR[i] = NewGO<CFontRender>(1);
		m_RONumFR[i]->SetPostRenderFlag(true);
		//�L���p�V�e�B�̃t�H���g
		m_capacityFR[i] = NewGO<CFontRender>(1);
		m_capacityFR[i]->SetPostRenderFlag(true);
		//�ő吔��ݒ�
		CLevelObjectManager::GetInstance()->SetReversibleObjectMaxNum(i, m_maxCapacityNum[i]);
	}

	//�L���p�V�e�B����A�C�e���̐��̃t�H���g�ւ̍�
	const float diffCapacityToNum = 100.0f;
	//�e�L�X�g�p��
	wchar_t text[256];
	//���݂̕\���Ɨ����̔��]�I�u�W�F�N�g�̐����擾
	const int* maxNum = m_maxCapacityNum;


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
	m_capacityPos[enFrontSide] = { capacityNumX ,capacityNumY };
	//�e�L�X�g���Z�b�g
	swprintf(text, L"�\:  /%d", maxNum[enFrontSide]);
	//�\���̐��̃L���p�V�e�B�̃t�H���g�̏�����
	m_capacityFR[enFrontSide]->Init(text,
		m_capacityPos[enFrontSide],
		{ fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });
	//�\���̐��t�H���g�̏�����
	m_RONumFR[enFrontSide]->Init(L"",
		{ capacityNumX + diffCapacityToNum,capacityNumY }, { fontColor.x,fontColor.y,fontColor.z,1.0f },
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
	const float diffFrontToBackY = 70.0f;
	m_capacityPos[enBackSide] = { capacityNumX ,capacityNumY - diffFrontToBackY };
	//�e�L�X�g���Z�b�g
	swprintf(text, L"��:  /%d", maxNum[enBackSide]);
	//�����̃L���p�V�e�B�̃t�H���g�̏�����
	m_capacityFR[enBackSide]->Init(text,
		m_capacityPos[enBackSide],
		{ fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });
	//�����̐��̃t�H���g�̏�����
	m_RONumFR[enBackSide]->Init(L"",
		{ capacityNumX + diffCapacityToNum,capacityNumY - diffFrontToBackY },
		{ fontColor.x,fontColor.y,fontColor.z,1.0f },
		0.0f, scale, { 0.5f,0.5f });

	//�^�C�g����ʂ��H
	if (m_titleFlag)
	{
		//�^�C�g����ʂȂ�

		//��\���ɂ���
		m_capacityFR[enFrontSide]->Deactivate();
		m_capacityFR[enBackSide]->Deactivate();
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
		DeleteGO(m_capacityFR[i]);
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
	//�e�L�X�g���Z�b�g
	swprintf(text, L"%d", num[enFrontSide]);
	m_RONumFR[enFrontSide]->SetText(text);
	swprintf(text, L"%d", num[enBackSide]);
	m_RONumFR[enBackSide]->SetText(text);

	if (m_firstFrame)
	{
		for (int i = 0; i < enFrontAndBackSideNum; i++)
		{
			m_oldRONum[i] = num[i];
		}
		m_firstFrame = false;
	}
	//�ǂ̉��o���N�������`�F�b�N����
	CheckDirecting(num);


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
void CCapacityUI::CheckDirecting(const int* num)
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
			if (num[i] > m_maxCapacityNum[i])
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
	//�t�H���g�����L�̓��������鎞��
	const float actionTime = 1.0f;

	//�^�C�}�[��0.0f���H�A�܂�ŏ��̈��ڂ̃��[�v���H
	if (m_timer[frontOrBackSide] == 0.0f)
	{
		//�ŏ��̈���

		//�t�H���g�̃J���[�𑝂������̃J���[�ɂ���
		m_RONumFR[frontOrBackSide]->SetColor({ 10.0f,0.0f,0.0f,1.0f });
	}

	//�^�C�}�[���A�N�V�����^�C���ȓ����H
	if (m_timer[frontOrBackSide] <= actionTime)
	{
		//�ȓ��̎�

		//�񎟎��̓����Ńt�H���g���͂˂�����
		//y = ax^2+b
		//a = (y-b)/x^2

		//�A�N�V�����^�C���̔����̃^�C��
		const float halfActionTime = actionTime / 2.0f;
		//�񎟎���x�̒l�A�����̃^�C���Œ��_�ɍs���悤�ɂ���
		float x = m_timer[frontOrBackSide] - halfActionTime;
		//�񎟎���b�̒l�A�������͂˂�ő�̍����ɂȂ�
		const float b = 25.0f;
		//�񎟎���a�̒l�A
		//�^�C�}�[���ŏ��̎��A������0�ɂȂ�a�̒l�����߂�A�܂�
		//x = 0.0f - halfActionTime���Ay = 0.0f�̎���a�����߂�
		const float a = (0.0f - b) / pow(-halfActionTime, 2.0f);
		//�񎟎���y�̒l�A���܂ł̒l�Ōv�Z���āA�������͂˂Ă��鍂���ɂȂ�
		const float y = a * pow(x, 2.0f) + b;

		//���̃t�H���g�̍��W
		//���݂̐��̃t�H���g�̍��W������Ă���
		Vector2 numFontPos = m_RONumFR[frontOrBackSide]->GetPosition();
		//Y���W�ɂ͍ŏ��̍��W�����Ă���
		numFontPos.y = m_capacityPos[frontOrBackSide].y;
		//�͂˂镪������������
		numFontPos.y += y;

		//���̃t�H���g�̍��W��ݒ肷��
		m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);
	}
	else
	{
		//�ȊO�̎�

		//�ʏ�̍��W�ɖ߂��Ă���

		//���̃t�H���g�̍��W
		//���݂̐��̃t�H���g�̍��W������Ă���
		Vector2 numFontPos = m_RONumFR[frontOrBackSide]->GetPosition();
		//Y���W�ɂ͍ŏ��̍��W�����Ă���
		numFontPos.y = m_capacityPos[frontOrBackSide].y;
		//���̃t�H���g�̍��W��ݒ肷��
		m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);
	}

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
	//�t�H���g�����L�̓��������鎞��
	const float actionTime = 1.0f;

	//�L���p�V�e�B����A�C�e�������̃t�H���g�ւ̍�
	const float diffCapacityToNum = 100.0f;

	//�^�C�}�[��0.0f���H�A�܂�ŏ��̈��ڂ̃��[�v���H
	if (m_timer[frontOrBackSide] == 0.0f)
	{
		//�ŏ��̈���

		//�t�H���g�̃J���[�����������̃J���[�ɂ���
		m_RONumFR[frontOrBackSide]->SetColor({ 0.0f,0.0f,20.0f,1.0f });
	}


	//�^�C�}�[���A�N�V�����^�C���ȓ����H
	if (m_timer[frontOrBackSide] <= actionTime)
	{
		//�ȓ��̎�

		const float oneEightTime = actionTime / 4.0f;

		const int state = m_timer[frontOrBackSide] / oneEightTime;

		const float destination = 10.0f;

		float moveLen = 0.0f;

		const float timeScale = 
			(m_timer[frontOrBackSide] - oneEightTime * state) / oneEightTime;


		switch (state)
		{
		case 0:
		case 4:
			moveLen = -destination * timeScale;
			break;
		case 1:
		case 5:
			moveLen = -destination + destination * timeScale;
			break;
		case 2:
		case 6:
			moveLen = destination * timeScale;
			break;
		case 3:
		case 7:
			moveLen = destination - destination * timeScale;
			break;

		}



		//���̃t�H���g�̍��W
		//
		Vector2 numFontPos = m_capacityPos[frontOrBackSide];
		//X���W�����낦��
		numFontPos.x += diffCapacityToNum;
		numFontPos.x += moveLen;

		//���̃t�H���g�̍��W��ݒ肷��
		m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);
	}
	else
	{
		//�ȊO�̎�

		//�ʏ�̍��W�ɖ߂��Ă���

		//���̃t�H���g�̍��W
		Vector2 numFontPos = m_capacityPos[frontOrBackSide];
		numFontPos.x += diffCapacityToNum;
		//���̃t�H���g�̍��W��ݒ肷��
		m_RONumFR[frontOrBackSide]->SetPosition(numFontPos);
	}

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