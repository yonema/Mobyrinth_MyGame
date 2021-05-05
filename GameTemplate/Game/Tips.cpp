#include "stdafx.h"
#include "Tips.h"

//コンストラクタ
CTips::CTips()
{
	//「Tips」のフォントレンダーの生成と初期化
	m_tipsFR = NewGO<CFontRender>(0);
	m_tipsFR->Init(L"Tips", 
		{ 200.0f,350.0f },			//座標
		{ 1.0f,1.0f,1.0f,1.0f },	//カラー
		0.0f,						//回転
		0.8f						//拡大
	);
	//ポストレンダラーで描画する
	m_tipsFR->SetPostRenderFlag(true);

	//Tipsの文章を表示するフォントレンダラーの生成と初期化
	m_sentenceFR = NewGO<CFontRender>(0);
	m_sentenceFR->Init(L"", { 220.0f,300.0f });
	//ポストレンダラーで描画する
	m_sentenceFR->SetPostRenderFlag(true);

	//テキストの初期化
	InitText();

}

//デストラクタ
CTips::~CTips()
{
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
	mbstowcs(text, m_text[textType], m_maxTextSize);
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