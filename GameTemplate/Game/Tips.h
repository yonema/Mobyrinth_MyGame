#pragma once
#include "SpriteRender.h"
#include "FontRender.h"
#include <locale>
#include <codecvt>
#include "ObjectType.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief チップス
	*/
	namespace nsTips
	{

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

			/// <summary>
			/// タイトル画面時の処理にする
			/// </summary>
			void SetTitleMode();

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
			nsGraphic::nsSprite::CSpriteRender* m_tipsUISR = nullptr;			//「Tips」のUI
			nsGraphic::nsFont::CFontRender* m_tipsFR = nullptr;				//「Tips」のフォントレンダラー
			nsGraphic::nsFont::CFontRender* m_sentenceFR = nullptr;			//Tipsの文章を表示するフォントレンダラー
			char m_text[nsLevelObject::EN_OBJECT_TYPE_NUM][m_maxTextSize];	//ロードしたTipsの文章
		};

		//u8len関数（UTF8の文字数をカウントする関数）
		int u8len(const char* str);
		//u8mb関数：UTF-8文字の1バイト目を判定して文字のバイト数を返す関数
		int u8mb(const char chr);
	}
}
