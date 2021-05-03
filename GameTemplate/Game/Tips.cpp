#include "stdafx.h"
#include "Tips.h"

//�R���X�g���N�^
CTips::CTips()
{
	//�uTips�v�̃t�H���g�����_�[�̐����Ə�����
	m_tipsFR = NewGO<CFontRender>(0);
	m_tipsFR->Init(L"Tips", 
		{ 200.0f,350.0f },			//���W
		{ 1.0f,1.0f,1.0f,1.0f },	//�J���[
		0.0f,						//��]
		0.8f						//�g��
	);
	//�|�X�g�����_���[�ŕ`�悷��
	m_tipsFR->SetPostRenderFlag(true);

	//Tips�̕��͂�\������t�H���g�����_���[�̐����Ə�����
	m_sentenceFR = NewGO<CFontRender>(0);
	m_sentenceFR->Init(L"", { 220.0f,300.0f });
	//�|�X�g�����_���[�ŕ`�悷��
	m_sentenceFR->SetPostRenderFlag(true);

	//�e�L�X�g�̏�����
	InitText();

}

//�f�X�g���N�^
CTips::~CTips()
{
	DeleteGO(m_tipsFR);
	DeleteGO(m_sentenceFR);
}


/// <summary>
/// �e�L�X�g�̏�����
/// </summary>
void CTips::InitText()
{	
	//�I�u�W�F�N�g�^�C�v�̐������J��Ԃ�
	for (int objectType = 0; objectType < enObjectTypeNum; objectType++)
	{
		//�I�u�W�F�N�g�^�C�v�ɂ���ă��[�h����e�L�X�g��U�蕪����
		switch (objectType)
		{
		case enEmpty:	//�����
			LoadText(objectType, "empty");
			break;
		case enWater:	//��
			LoadText(objectType, "water");
			break;
		case enFire:	//��
			LoadText(objectType, "fire");
			break;
		default:		//�f�t�H���g
			LoadText(objectType, "empty");
			break;
		}
	}
}


/// <summary>
/// �e�L�X�g�����[�h����
/// </summary>
/// <param name="objectType">�I�u�W�F�N�g�^�C�v</param>
/// <param name="fileName">�e�L�X�g�t�@�C���̖��O</param>
void CTips::LoadText(const int objectType, const char* fileName)
{
	//���{����Z�b�g
	setlocale(LC_CTYPE, "ja_JP.UTF-8");
	//std::locale("");

	//�t�@�C���p�X�̓��ɂ��镶����
	const char* headFilePath = "Assets/text/";
	//�t�@�C���p�X�̌��ɂ��镶����
	const char* tailFilePath = ".txt";
	//�t�@�C���p�X�̕�����
	char FilePath[m_maxTextSize];
	//�Ȃ��Ƃ͎v�����ǁA�ő�T�C�Y�ȏ�Ȃ�؂�̂�
	const int maxSize = m_maxTextSize - 1;
	FilePath[maxSize] = 0;

	//�t�@�C���p�X����������Ƃ��̃G���[
	if (sizeof(headFilePath) + sizeof(fileName) + sizeof(tailFilePath) > maxSize)
	{
		MessageBoxA(nullptr, "�t�@�C���p�X���������܂��B", "�G���[", MB_OK);
		return;
	}

	//�t�@�C���p�X�Ƀt�@�C���p�X�̓��ɂ��镶������R�s�[
	strncpy(FilePath, headFilePath, maxSize);
	//�t�@�C���p�X�Ƀt�@�C���̖��O��ǉ�
	strncat(FilePath, fileName, maxSize - sizeof(headFilePath));
	//�t�@�C���p�X�Ƀt�@�C���p�X�̌��ɂ��镶�����ǉ�
	strncat(FilePath, tailFilePath, maxSize - sizeof(FilePath));

	//�t�@�C����ǂݎ�胂�[�h�ŊJ��
	auto fp = fopen(FilePath, "r");

	if (fp == nullptr)
	{
		//�t�@�C���Ȃ���
		const char* cantOpen = "�̃t�@�C�����J���܂���B";
		strncat(FilePath, cantOpen,sizeof(FilePath) + sizeof(cantOpen));
		MessageBoxA(nullptr, FilePath, "�G���[", MB_OK);
		return;
	}

	//�t�@�C���̕��������Ă�������
	char c;
	//�t�@�C�����ꕶ�����ǂݎ��
	//EOF�܂œǂݑ�����
	while ((c = fgetc(fp)) != EOF)
	{
		//���[�h�������͂��A�f�[�^�����o�ɕێ����Ă���
		strncat(m_text[objectType], &c, sizeof(char));
	}

	//�t�@�C�������
	fclose(fp);

}

/// <summary>
/// ���[�h�ς݂̃I�u�W�F�N�g�^�C�v�̃e�L�X�g���Z�b�g����
/// </summary>
/// <param name="textType">�I�u�W�F�N�g�^�C�v</param>
void CTips::SetText(const int textType)
{
	//�t�H���g�����_���[�ɃZ�b�g����e�L�X�g��wchar_t�^���炩
	//char�^�̃f�[�^�����o��ϊ�����

	//wchar_t�^�̃e�L�X�g
	wchar_t text[m_maxTextSize];
	//char�^�̃f�[�^�����o��wchar_t�^�ɕϊ�����
	mbstowcs(text, m_text[textType], m_maxTextSize);
	//�e�L�X�g���t�H���g�����_���[�ɃZ�b�g
	m_sentenceFR->SetText(text);
}

/// <summary>
/// �e�L�X�g�����ɂ��\�����Ȃ�
/// </summary>
void CTips::ClearText()
{
	//����ۂ̃e�L�X�g���Z�b�g
	SetText(enEmpty);
}