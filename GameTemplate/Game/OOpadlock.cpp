#include "stdafx.h"
#include "OOpadlock.h"

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

			//ObstacleObjectのモデルのファイルパスとOBBのサイズの定数データを使用可能にする
			using namespace OOsFilepathAndObbSizeConstData;

			//スタート関数
			bool OOpadlock::StartSub()
			{
				//初期化用関数
				Init(MODEL_FILEPATH_PADLOCK, EN_OO_TYPE_BIG_PADLOCK);

				//OBBのサイズを設定
				SetOBBDirectionLength(SIZE_OBB_PADLOCK);

				//OBBのタグを設定
				SetOBBTag(nsOBB::COBB::EN_RO_TYPE_PADLOCK);

				return true;
			}
		}
	}
}