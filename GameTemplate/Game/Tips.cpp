#include "stdafx.h"
#include "Tips.h"

//�R���X�g���N�^
CTips::CTips()
{
	m_tipsUISR = NewGO<CSpriteRender>(0);
	m_tipsUISR->Init("Assets/image/Tips_UI.DDS",
		512.0f * 1.5f, 256.0f * 0.8f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_tipsUISR->SetPosition({ 400.0f, 240.0f + 20.0f, 0.0f});
	m_tipsUISR->SetPostRenderFlag(true);
	//�uTips�v�̃t�H���g�����_�[�̐����Ə�����
	m_tipsFR = NewGO<CFontRender>(0);
	m_tipsFR->Init(L"Tips", 
		{ 200.0f,330.0f },			//���W
		{ 1.0f,1.0f,1.0f,1.0f },	//�J���[
		0.0f,						//��]
		0.8f						//�g��
	);
	//�|�X�g�����_���[�ŕ`�悷��
	m_tipsFR->SetPostRenderFlag(true);

	//Tips�̕��͂�\������t�H���g�����_���[�̐����Ə�����
	m_sentenceFR = NewGO<CFontRender>(0);
	m_sentenceFR->Init(L"", { 220.0f,270.0f });
	//�|�X�g�����_���[�ŕ`�悷��
	m_sentenceFR->SetPostRenderFlag(true);
	m_sentenceFR->SetScale(0.8f);

	//�e�L�X�g�̏�����
	InitText();

}

//�f�X�g���N�^
CTips::~CTips()
{
	DeleteGO(m_tipsUISR);
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
		case enRunning:	//�ғ�
			LoadText(objectType, "running");
			break;
		case enStop:	//��~
			LoadText(objectType, "stop");
			break;
		case enNail:	//�B
			LoadText(objectType, "nail");
			break;
		case enBar:		//�o�[��
			LoadText(objectType, "bar");
			break;
		case enBigFire:		//��
			LoadText(objectType, "bigFire");
			break;
		case enWall:		//��
			LoadText(objectType, "wall");
			break;
		case enGoal:		//�S�[��
			LoadText(objectType, "goal");
			break;
		case enBox:			//��
			LoadText(objectType, "box");
			break;
		case enOneway:		//����ʍs
			LoadText(objectType, "oneWay");
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
	//setlocale(LC_CTYPE, "ja_JP.SJIS");
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
	//std::string utf8String = m_text[objectType];
	//setlocale(LC_CTYPE, "");
	//std::string MultiString = utf8_to_multi_cppapi(utf8String);
	//printf(m_text[objectType], MultiString.c_str());
}
std::string wide_to_multi_capi(std::wstring const& src)
{
	std::size_t converted{};
	std::vector<char> dest(src.size() * sizeof(wchar_t) + 1, '\0');
	if (::_wcstombs_s_l(&converted, dest.data(), dest.size(), src.data(), _TRUNCATE, ::_create_locale(LC_ALL, "jpn")) != 0) {
		throw std::system_error{ errno, std::system_category() };
	}
	dest.resize(std::char_traits<char>::length(dest.data()));
	dest.shrink_to_fit();
	return std::string(dest.begin(), dest.end());
}
std::wstring utf8_to_wide_cppapi(std::string const& src)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(src);
}
std::string utf8_to_multi_cppapi(std::string const& src)
{
	auto const wide = utf8_to_wide_cppapi(src);
	return wide_to_multi_capi(wide);
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

	if (textType != enWater)
		int a = 1;
	
	std::string str = m_text[textType];
	mbstowcs(text, str.c_str(), m_maxTextSize);

	//mbstowcs(text, m_text[textType], m_maxTextSize);
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

void CTips::SetTitleMode()
{
	m_tipsUISR->Deactivate();
	m_tipsFR->Deactivate();
	m_sentenceFR->Deactivate();
}