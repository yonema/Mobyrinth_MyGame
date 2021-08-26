#pragma once
#include "CaslFile.h"
#include "SpriteRender.h"

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
		/// �}�b�v�`�b�v2D�B
		/// </summary>
		class MapChip2D : private Noncopyable
		{
		public:		//�����ŌĂ΂�郁���o�֐�
			~MapChip2D();

		public:		//�����̃����o�֐�����Ɏg��

			/// <summary>
			/// �������B
			/// </summary>
			/// <param name="caslData">Casl�f�[�^�B</param>
			void Init(SCaslData* caslData);



		private:	//�f�[�^�����o
			nsGraphic::nsSprite::CSpriteRender* m_spriteRender = nullptr;	//�X�v���C�g�B
		};

	}
}