#pragma once
#include "ObstacleObject.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief ���x���I�u�W�F�N�g
	*/
	namespace nsLevelObject
	{
		/**
		 * @brief ��Q�I�u�W�F�N�g
		*/
		namespace nsObstacleObject
		{

			/// <summary>
			/// ��Q�I�u�W�F�N�g
			/// ���I�u�W�F�N�g
			/// </summary>
			class OObox : public CObstacleObject
			{
			public:		//�����ŌĂ΂�郁���o�֐�
				bool StartSub() override final;	//�X�^�[�g�֐�

			};
		}
	}
}
