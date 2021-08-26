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
		 * @brief 表か裏かの定数値
		*/
		enum EN_FRONT_OR_BACK
		{
			EN_FRONT,				//表状態
			EN_BACK,				//裏状態
			EN_FRONT_AND_BACK_NUM,	//表裏の数
		};
	}
}