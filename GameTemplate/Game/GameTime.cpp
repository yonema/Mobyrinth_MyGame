#include "stdafx.h"
#include "GameTime.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief タイマー
	*/
	namespace nsTimer
	{

		void CGameTime::DrawFPS(RenderContext& rc, float elapsedTime)
		{
			//経過時間を計測するカウント
			static int count = 0;

			//経過時間を加算していく
			m_timeTotal += elapsedTime;
			//カウントを進める
			count++;

			//カウントが30まで進んだら
			if (count == 30) {
				//直近30フレームのFPSを平均して、
				//表示するFPSを出す
				m_fps = 1.0f / (m_timeTotal / count);

				//経過時間の総和とカウントを初期値に戻す
				m_timeTotal = 0.0f;
				count = 0;

			}

			//フォントの描画開始
			m_font.Begin(rc);

			//文字列の設定
			wchar_t fps[256];
			swprintf_s(fps, L"FPS = %2.4f", m_fps);
			float w = g_camera2D->GetWidth();
			float h = g_camera2D->GetHeight();
			//描画
			m_font.Draw(
				fps,
				{
					w * -0.5f,
					h * 0.5f
				},
				Vector4::White,
				0.0f,
				1.0f,
				{ 0.0f, 1.0f }
			);
			//描画終了
			m_font.End(rc);
		}
	}
}