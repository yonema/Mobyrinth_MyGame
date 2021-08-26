#include "stdafx.h"
#include "MapChip2D.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief レベル2D
	*/
	namespace nsLevel2D
	{

		/// <summary>
		/// 初期化。
		/// </summary>
		/// <param name="caslData">Caslデータ。</param>
		void MapChip2D::Init(SCaslData* caslData)
		{
			//スプライトレンダラーの生成
			m_spriteRender = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::PRIORITY_SECOND);
			//スプライトレンダラーの初期化
			m_spriteRender->Init(caslData->ddsFilePath.get(), caslData->width, caslData->height);

			//座標をスプライトレンダラーに設定
			Vector3 position = g_VEC3_ZERO;
			position.x = caslData->position.x;
			position.y = caslData->position.y;
			m_spriteRender->SetPosition(position);

			//拡大をスプライトレンダラーに設定
			Vector3 scale = g_VEC3_ONE;
			scale.x = caslData->scale.x;
			scale.y = caslData->scale.y;
			m_spriteRender->SetScale(scale);

			return;
		}


		MapChip2D::~MapChip2D()
		{
			if (m_spriteRender)
				DeleteGO(m_spriteRender);

			return;
		}

	}
}