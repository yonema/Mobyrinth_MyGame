#include "stdafx.h"
#include "stage_title.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �X�e�[�W
	*/
	namespace nsStage
	{

		bool CStage_title::StartSub()
		{
			LoadLevel("Assets/level/stage_title.tkl");

			return true;
		}
		CStage_title::~CStage_title()
		{

		}
	}
}