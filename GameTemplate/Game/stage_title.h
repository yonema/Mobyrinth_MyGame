#pragma once
#include "StageBase.h"


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
		/**
		 * @brief �^�C�g���X�e�[�W�N���X
		*/
		class CStage_title : public IStageBase
		{
		public:
			bool StartSub() override final;
			~CStage_title();
			void RetryStage()override final
			{
				NewGO<CStage_title>(0);
			}


		private:	//�f�[�^�����o
		};

	}
}