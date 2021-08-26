#pragma once
#include "SpriteRender.h"
#include "FontRender.h"
#include <locale>
#include <codecvt>
#include "ObjectType.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �`�b�v�X
	*/
	namespace nsTips
	{

		/// <summary>
		/// Tips�\���N���X
		/// </summary>
		class CTips
		{
		public:		//�����ŌĂ΂�郁���o�֐�
			CTips();	//�R���X�g���N�^
			~CTips();	//�f�X�g���N�^

		public:		//�����o�֐�

			/// <summary>
			/// ���[�h�ς݂̃I�u�W�F�N�g�^�C�v�̃e�L�X�g���Z�b�g����
			/// </summary>
			/// <param name="textType">�I�u�W�F�N�g�^�C�v</param>
			void SetText(const int textType);

			/// <summary>
			/// �e�L�X�g�����ɂ��\�����Ȃ�
			/// </summary>
			void ClearText();

			/// <summary>
			/// �^�C�g����ʎ��̏����ɂ���
			/// </summary>
			void SetTitleMode();

		private:	//private�ȃ����o�֐�

			/// <summary>
			/// �e�L�X�g�̏�����
			/// </summary>
			void InitText();

			/// <summary>
			/// �e�L�X�g�����[�h����
			/// </summary>
			/// <param name="objectType">�I�u�W�F�N�g�^�C�v</param>
			/// <param name="fileName">�e�L�X�g�t�@�C���̖��O</param>
			void LoadText(const int objectType, const char* fileName);

		private:	//static�ȃf�[�^�����o
			//�e�L�X�g�̍ő�T�C�Y
			static const int m_maxTextSize = 256;

		private:	//�f�[�^�����o
			nsGraphic::nsSprite::CSpriteRender* m_tipsUISR = nullptr;			//�uTips�v��UI
			nsGraphic::nsFont::CFontRender* m_tipsFR = nullptr;				//�uTips�v�̃t�H���g�����_���[
			nsGraphic::nsFont::CFontRender* m_sentenceFR = nullptr;			//Tips�̕��͂�\������t�H���g�����_���[
			char m_text[nsLevelObject::EN_OBJECT_TYPE_NUM][m_maxTextSize];	//���[�h����Tips�̕���
		};

		//u8len�֐��iUTF8�̕��������J�E���g����֐��j
		int u8len(const char* str);
		//u8mb�֐��FUTF-8������1�o�C�g�ڂ𔻒肵�ĕ����̃o�C�g����Ԃ��֐�
		int u8mb(const char chr);
	}
}
