#include "stdafx.h"
#include "MapChip2D.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief ���x��2D
	*/
	namespace nsLevel2D
	{

		/// <summary>
		/// �������B
		/// </summary>
		/// <param name="caslData">Casl�f�[�^�B</param>
		void MapChip2D::Init(SCaslData* caslData)
		{
			//�X�v���C�g�����_���[�̐���
			m_spriteRender = NewGO<nsGraphic::nsSprite::CSpriteRender>(nsCommonData::PRIORITY_SECOND);
			//�X�v���C�g�����_���[�̏�����
			m_spriteRender->Init(caslData->ddsFilePath.get(), caslData->width, caslData->height);

			//���W���X�v���C�g�����_���[�ɐݒ�
			Vector3 position = g_VEC3_ZERO;
			position.x = caslData->position.x;
			position.y = caslData->position.y;
			m_spriteRender->SetPosition(position);

			//�g����X�v���C�g�����_���[�ɐݒ�
			Vector3 scale = g_VEC3_ONE;
			scale.x = caslData->scale.x;
			scale.y = caslData->scale.y;
			m_spriteRender->SetScale(scale);

			return;
		}


		MapChip2D::~MapChip2D()
		{
			if (m_spriteRender)
				DeleteGO(m_spriteRender);

			return;
		}

	}
}