#include "stdafx.h"
#include "OObox.h"

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
			bool OObox::StartSub()
			{
				//�������p�֐�
				Init(MODEL_FILEPATH_BOX, EN_OO_TYPE_BOX);

				//OBB�̃T�C�Y��ݒ�
				SetOBBDirectionLength(OBB_SIZE_BOX);

				//OBB�̃^�O��ݒ�
				SetOBBTag(nsOBB::COBB::EN_OO_TYPE_BOX);

				return true;
			}
		}
	}
}