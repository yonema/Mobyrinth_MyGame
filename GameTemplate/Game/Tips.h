#pragma once
#include "FontRender.h"
#include <locale>
#include "ObjectType.h"


class CTips
{
public:
	CTips();
	~CTips();

public:
	void SetText(const int textType);
	void ClearText();
private:
	void InitText();
	void ReadText(const int textType, const char* fileName);

private:
	static const int m_maxTextSize = 256;
private:
	CFontRender* m_tipsFR = nullptr;
	CFontRender* m_sentenceFR = nullptr;
	char m_text[enObjectTypeNum][m_maxTextSize];
};

