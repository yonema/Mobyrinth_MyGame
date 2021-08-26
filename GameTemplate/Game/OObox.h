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
			/// 箱オブジェクト
			/// </summary>
			class OObox : public CObstacleObject
			{
			public:		//自動で呼ばれるメンバ関数
				bool StartSub() override final;	//スタート関数

			};
		}
	}
}
