#pragma once
#include "ObstacleObject.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief レベルオブジェクト
	*/
	namespace nsLevelObject
	{
		/**
		 * @brief 障害オブジェクト
		*/
		namespace nsObstacleObject
		{

			/// <summary>
			/// 障害オブジェクト
			/// 南京錠オブジェクト
			/// </summary>
			class OOpadlock : public CObstacleObject
			{
			public:		//自動で呼ばれるメンバ関数
				bool StartSub() override final;	//スタート関数
			};

		}
	}
}