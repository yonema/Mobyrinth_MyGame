#include "stdafx.h"
#include "Tips.h"


CTips::CTips()
{
	m_tipsFR = NewGO<CFontRender>(0);
	m_tipsFR->Init(L"Tips", 
		{ 200.0f,350.0f },			//���W
		{ 1.0f,1.0f,1.0f,1.0f },	//�J���[
		0.0f,						//��]
		0.8f						//�g��
	);
	m_tipsFR->SetPostRenderFlag(true);

	m_sentenceFR = NewGO<CFontRender>(0);
	m_sentenceFR->Init(L"", { 220.0f,300.0f });
	m_sentenceFR->SetPostRenderFlag(true);

	InitText();

}

CTips::~CTips()
{
	DeleteGO(m_tipsFR);
	DeleteGO(m_sentenceFR);
}



void CTips::InitText()
{	
	for (int textType = 0; textType < enObjectTypeNum; textType++)
	{
		switch (textType)
		{
		case enEmpty:
			ReadText(textType, "empty");
			break;
		case enWater:
			ReadText(textType, "water");
			break;
		case enFire:
			ReadText(textType, "fire");
			break;
		default:
			ReadText(textType, "empty");
			break;
		}
	}
}

void CTips::ReadText(const int textType, const char* fileName)
{
	//���{����Z�b�g
	setlocale(LC_CTYPE, "ja_JP.UTF-8");
	//std::locale("");

	const char* headFilePath = "Assets/text/";
	const char* tailFilePath = ".txt";
	char FilePath[m_maxTextSize];
	//�Ȃ��Ƃ͎v�����ǁA�ő�T�C�Y�ȏ�Ȃ�؂�̂�
	const int maxSize = m_maxTextSize - 1;
	FilePath[maxSize] = 0;
	strncpy(FilePath, headFilePath, maxSize);
	strncat(FilePath, fileName, maxSize - sizeof(headFilePath));
	strncat(FilePath, tailFilePath, maxSize - sizeof(FilePath));

	auto fp = fopen(FilePath, "r");

	if (fp == nullptr)
	{
		//�t�@�C���Ȃ���
		return;
	}

	char c;
	while (EOF != (c = fgetc(fp)))
	{
		strncat(m_text[textType], &c, sizeof(char));
	}


	fclose(fp);

}

void CTips::SetText(const int textType)
{
	wchar_t text[m_maxTextSize];
	mbstowcs(text, m_text[textType], m_maxTextSize);
	m_sentenceFR->SetText(text);
}

void CTips::ClearText()
{
	SetText(enEmpty);
}