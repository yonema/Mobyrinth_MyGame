#include "stdafx.h"
#include "TipsController.h"

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

		//スタート関数
		bool CTipsController::Start()
		{
			return true;
		}

		//アップデート関数
		void CTipsController::Update()
		{
			//プレイヤーに一番近いオブジェクトのタイプを取得
			int objectType = nsLevelObject::CLevelObjectManager::GetInstance()->GetNearestObjectType(1000.0f);

			//Tipsにセットする
			m_tips.SetText(objectType);

		}
	}
}