#include "stdafx.h"
#include "FontRender.h"


/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief グラフィック
	*/
	namespace nsGraphic
	{
		/**
		 * @brief フォント
		*/
		namespace nsFont
		{

			//スタート関数
			bool CFontRender::Start()
			{
				return true;
			}

			//デストラクタ
			CFontRender::~CFontRender()
			{
				return;
			}


			/// <summary>
			/// 初期化関数。最初に呼んでね。
			/// </summary>
			/// <param name="text">表示するテキスト</param>
			/// <param name="position">座標。{0.0f,0.0f}が中央</param>
			/// <param name="color">カラー{1.0f,1.0f,1.0f,1.0f}が白色</param>
			/// <param name="rotation">回転</param>
			/// <param name="scale">拡大</param>
			/// <param name="pivot">ピボット（基点）</param>
			void CFontRender::Init
			(const wchar_t* text,	//表示するテキスト
				Vector2 position,	//座標
				Vector4 color,		//カラー
				float rotation,		//回転
				float scale,		//拡大
				Vector2 pivot)		//ピボット（基点）
			{
				//それぞれをメンバ変数に保持させる
				wsprintf(m_text, text);
				m_position = position;
				m_color = color;
				m_rotation = rotation;
				m_scale = scale;
				m_pivot = pivot;

				//ディファ―ドレンダリングではなくする
				SetIsDefferdRender(false);

				return;
			}

			/// <summary>
			/// 表示するテキストを設定する
			/// </summary>
			/// <param name="text">テキスト</param>
			void CFontRender::SetText(const wchar_t* const text)
			{
				swprintf_s(m_text, text);

				return;
			}


			/// <summary>
			/// 描画用関数
			/// </summary>
			/// <param name="rc"></param>
			void CFontRender::FontRender(RenderContext& rc)
				//void CFontRender::ForwardRender(RenderContext& rc)
			{
				//描画開始
				m_font.Begin(rc);

				//描画処理
				m_font.Draw(m_text,
					m_position,
					m_color,
					m_rotation,
					m_scale,
					m_pivot);

				//描画終了
				m_font.End(rc);

				return;
			}

		}
	}
}