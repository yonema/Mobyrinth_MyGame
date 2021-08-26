#pragma once

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief 共通データ
	*/
	namespace nsCommonData
	{

		/**
		 * @brief 表側か裏側か
		*/
		enum EN_FRONT_SIDE_OR_BACK_SIDE
		{
			EN_FRONT_SIDE,						//表側
			EN_BACK_SIDE,						//裏側
			EN_FRONT_SIDE_AND_BACK_SIDE_NUM,	//表と裏の数

			EB_NONE_SIDE = -1,					//無し
		};
	}
}