#include "stdafx.h"
#include "OOpadlock.h"

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

			//ObstacleObject�̃��f���̃t�@�C���p�X��OBB�̃T�C�Y�̒萔�f�[�^���g�p�\�ɂ���
			using namespace OOsFilepathAndObbSizeConstData;

			//�X�^�[�g�֐�
			bool OOpadlock::StartSub()
			{
				//�������p�֐�
				Init(MODEL_FILEPATH_PADLOCK, EN_OO_TYPE_BIG_PADLOCK);

				//OBB�̃T�C�Y��ݒ�
				SetOBBDirectionLength(SIZE_OBB_PADLOCK);

				//OBB�̃^�O��ݒ�
				SetOBBTag(nsOBB::COBB::EN_RO_TYPE_PADLOCK);

				return true;
			}
		}
	}
}