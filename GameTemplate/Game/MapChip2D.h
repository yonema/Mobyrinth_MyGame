#pragma once
#include "CaslFile.h"
#include "SpriteRender.h"

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
		/// マップチップ2D。
		/// </summary>
		class MapChip2D : private Noncopyable
		{
		public:		//自動で呼ばれるメンバ関数
			~MapChip2D();

		public:		//ここのメンバ関数を主に使う

			/// <summary>
			/// 初期化。
			/// </summary>
			/// <param name="caslData">Caslデータ。</param>
			void Init(SCaslData* caslData);



		private:	//データメンバ
			nsGraphic::nsSprite::CSpriteRender* m_spriteRender = nullptr;	//スプライト。
		};

	}
}