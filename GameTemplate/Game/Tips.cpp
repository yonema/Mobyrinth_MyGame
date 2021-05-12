#include "stdafx.h"
#include "Tips.h"

//コンストラクタ
CTips::CTips()
{
	m_tipsUISR = NewGO<CSpriteRender>(0);
	m_tipsUISR->Init("Assets/image/Tips_UI.DDS",
		512.0f * 1.5f, 256.0f * 0.8f, { 0.5f,0.5f }, AlphaBlendMode_Trans);
	m_tipsUISR->SetPosition({ 400.0f, 240.0f + 20.0f, 0.0f});
	m_tipsUISR->SetPostRenderFlag(true);
	//「Tips」のフォントレンダーの生成と初期化
	m_tipsFR = NewGO<CFontRender>(0);
	m_tipsFR->Init(L"Tips", 
		{ 200.0f,330.0f },			//座標
		{ 1.0f,1.0f,1.0f,1.0f },	//カラー
		0.0f,						//回転
		0.8f						//拡大
	);
	//ポストレンダラーで描画する
	m_tipsFR->SetPostRenderFlag(true);

	//Tipsの文章を表示するフォントレンダラーの生成と初期化
	m_sentenceFR = NewGO<CFontRender>(0);
	m_sentenceFR->Init(L"", { 220.0f,270.0f });
	//ポストレンダラーで描画する
	m_sentenceFR->SetPostRenderFlag(true);
	m_sentenceFR->SetScale(0.8f);

	//テキストの初期化
	InitText();

}

//デストラクタ
CTips::~CTips()
{
	DeleteGO(m_tipsUISR);
	DeleteGO(m_tipsFR);
	DeleteGO(m_sentenceFR);
}


/// <summary>
/// テキストの初期化
/// </summary>
void CTips::InitText()
{	
	//オブジェクトタイプの数だけ繰り返す
	for (int objectType = 0; objectType < enObjectTypeNum; objectType++)
	{
		//オブジェクトタイプによってロードするテキストを振り分ける
		switch (objectType)
		{
		case enEmpty:	//空っぽ
			LoadText(objectType, "empty");
			break;
		case enWater:	//水
			LoadText(objectType, "water");
			break;
		case enFire:	//火
			LoadText(objectType, "fire");
			break;
		case enRunning:	//稼働
			LoadText(objectType, "running");
			break;
		case enStop:	//停止
			LoadText(objectType, "stop");
			break;
		case enNail:	//釘
			LoadText(objectType, "nail");
			break;
		case enBar:		//バール
			LoadText(objectType, "bar");
			break;
		case enBigFire:		//炎
			LoadText(objectType, "bigFire");
			break;
		case enWall:		//壁
			LoadText(objectType, "wall");
			break;
		case enGoal:		//ゴール
			LoadText(objectType, "goal");
			break;
		case enBox:			//箱
			LoadText(objectType, "box");
			break;
		case enOneway:		//一方通行
			LoadText(objectType, "oneWay");
			break;
		default:		//デフォルト
			LoadText(objectType, "empty");
			break;
		}
	}
}


/// <summary>
/// テキストをロードする
/// </summary>
/// <param name="objectType">オブジェクトタイプ</param>
/// <param name="fileName">テキストファイルの名前</param>
void CTips::LoadText(const int objectType, const char* fileName)
{
	//日本語をセット
	setlocale(LC_CTYPE, "ja_JP.UTF-8");
	//setlocale(LC_CTYPE, "ja_JP.SJIS");
	//std::locale("");

	//ファイルパスの頭につける文字列
	const char* headFilePath = "Assets/text/";
	//ファイルパスの後ろにつける文字列
	const char* tailFilePath = ".txt";
	//ファイルパスの文字列
	char FilePath[m_maxTextSize];
	//ないとは思うけど、最大サイズ以上なら切り捨て
	const int maxSize = m_maxTextSize - 1;
	FilePath[maxSize] = 0;

	//ファイルパスが長すぎるときのエラー
	if (sizeof(headFilePath) + sizeof(fileName) + sizeof(tailFilePath) > maxSize)
	{
		MessageBoxA(nullptr, "ファイルパスが長すぎます。", "エラー", MB_OK);
		return;
	}

	//ファイルパスにファイルパスの頭につける文字列をコピー
	strncpy(FilePath, headFilePath, maxSize);
	//ファイルパスにファイルの名前を追加
	strncat(FilePath, fileName, maxSize - sizeof(headFilePath));
	//ファイルパスにファイルパスの後ろにつける文字列を追加
	strncat(FilePath, tailFilePath, maxSize - sizeof(FilePath));

	//ファイルを読み取りモードで開く
	auto fp = fopen(FilePath, "r");

	if (fp == nullptr)
	{
		//ファイルないよ
		const char* cantOpen = "のファイルが開けません。";
		strncat(FilePath, cantOpen,sizeof(FilePath) + sizeof(cantOpen));
		MessageBoxA(nullptr, FilePath, "エラー", MB_OK);
		return;
	}

	//ファイルの文字を入れておく文字
	char c;
	//ファイルを一文字ずつ読み取る
	//EOFまで読み続ける
	while ((c = fgetc(fp)) != EOF)
	{
		//ロードした文章を、データメンバに保持しておく
		strncat(m_text[objectType], &c, sizeof(char));
	}

	//ファイルを閉じる
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
/// ロード済みのオブジェクトタイプのテキストをセットする
/// </summary>
/// <param name="textType">オブジェクトタイプ</param>
void CTips::SetText(const int textType)
{
	//フォントレンダラーにセットするテキストはwchar_t型だらか
	//char型のデータメンバを変換する

	//wchar_t型のテキスト
	wchar_t text[m_maxTextSize];
	//char型のデータメンバをwchar_t型に変換する

	if (textType != enWater)
		int a = 1;
	
	std::string str = m_text[textType];
	mbstowcs(text, str.c_str(), m_maxTextSize);

	//mbstowcs(text, m_text[textType], m_maxTextSize);
	//テキストをフォントレンダラーにセット
	m_sentenceFR->SetText(text);
}

/// <summary>
/// テキストを何にも表示しない
/// </summary>
void CTips::ClearText()
{
	//空っぽのテキストをセット
	SetText(enEmpty);
}

void CTips::SetTitleMode()
{
	m_tipsUISR->Deactivate();
	m_tipsFR->Deactivate();
	m_sentenceFR->Deactivate();
}