#pragma once
#include "FontRender.h"
#include <locale>
#include "ObjectType.h"

/// <summary>
/// Tips表示クラス
/// </summary>
class CTips
{
public:		//自動で呼ばれるメンバ関数
	CTips();	//コンストラクタ
	~CTips();	//デストラクタ

public:		//メンバ関数

	/// <summary>
	/// ロード済みのオブジェクトタイプのテキストをセットする
	/// </summary>
	/// <param name="textType">オブジェクトタイプ</param>
	void SetText(const int textType);

	/// <summary>
	/// テキストを何にも表示しない
	/// </summary>
	void ClearText();

private:	//privateなメンバ関数

	/// <summary>
	/// テキストの初期化
	/// </summary>
	void InitText();

	/// <summary>
	/// テキストをロードする
	/// </summary>
	/// <param name="objectType">オブジェクトタイプ</param>
	/// <param name="fileName">テキストファイルの名前</param>
	void LoadText(const int objectType, const char* fileName);

private:	//staticなデータメンバ
	//テキストの最大サイズ
	static const int m_maxTextSize = 256;

private:	//データメンバ
	CFontRender* m_tipsFR = nullptr;				//「Tips」のフォントレンダラー
	CFontRender* m_sentenceFR = nullptr;			//Tipsの文章を表示するフォントレンダラー
	char m_text[enObjectTypeNum][m_maxTextSize];	//ロードしたTipsの文章
};

