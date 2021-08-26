#pragma once
#include "physics/PhysicsStaticObject.h"
#include "ModelRender.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief tkLevel
	*/
	namespace nsTkLevel
	{

		struct LevelObjectData;
		class CMapChip
		{
		public://�����o�֐�
			/// <summary>
			/// �}�b�v�`�b�v�̍쐬
			/// ���x���̃f�[�^�����Ƃ�
			/// ���f���̕\���ƐÓI�����I�u�W�F�N�g�̏�����
			/// </summary>
			/// <param name="objData">���x���̃I�u�W�F�N�g�̃f�[�^</param>
			CMapChip(const LevelObjectData& objData);
			~CMapChip();

		private://�f�[�^�����o
			PhysicsStaticObject m_physicsStaticObject;			//!<�ÓI�����I�u�W�F�N�g�B
			nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;				//���f�������_���[
		};

	}
}