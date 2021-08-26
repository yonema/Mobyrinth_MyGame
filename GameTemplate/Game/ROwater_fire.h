#pragma once
#include "ReversibleObject.h"
#include "OOflame.h"

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
		 * @brief ���]�I�u�W�F�N�g
		*/
		namespace nsReversibleObject
		{

			/// <summary>
			/// ���]�I�u�W�F�N�g�B
			/// ���A�΃I�u�W�F�N�g
			/// </summary>
			class ROwater_fire : public CReversibleObject
			{
			public:		//�����ŌĂ΂�郁���o�֐�
				bool StartSub() override final;	//�X�^�[�g�֐�

				/// <summary>
				/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
				/// </summary>
				void QuerySub()override final;

			private:	//private�ȃ����o�֐�

				/**
				 * @brief ���g���u���v�ŁA�u���v�ƏՓ˂������̏���
				 * @param [in] flame �u���v�̃|�C���^
				*/
				void WaterHitFlame(nsObstacleObject::OOflame* flame);

				/**
				 * @brief ���g���u���v�ŁA�u�΁v�ƏՓ˂������̏���
				 * @param [in] fire �u�΁v�̃|�C���^
				*/
				void WaterHitFire(ROwater_fire* fire);

				/**
				 * @brief ���g���u�΁v�ŁA�u���v�ƏՓ˂������̏���
				 * @param [in] water �u���v�̃|�C���^
				*/
				void FireHitWater(ROwater_fire* water);

			private:	//�f�[�^�����o
				//��x�ɕ����̃I�u�W�F�N�g��j�����邱�Ƃ�h�����߂̃t���O
				bool m_actionFlag = true;	//�s���ł��邩�H
			};

		}
	}
}