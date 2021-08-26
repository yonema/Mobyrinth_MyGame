#include "stdafx.h"
#include "FontRender.h"


/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �O���t�B�b�N
	*/
	namespace nsGraphic
	{
		/**
		 * @brief �t�H���g
		*/
		namespace nsFont
		{

			//�X�^�[�g�֐�
			bool CFontRender::Start()
			{
				return true;
			}

			//�f�X�g���N�^
			CFontRender::~CFontRender()
			{
				return;
			}


			/// <summary>
			/// �������֐��B�ŏ��ɌĂ�łˁB
			/// </summary>
			/// <param name="text">�\������e�L�X�g</param>
			/// <param name="position">���W�B{0.0f,0.0f}������</param>
			/// <param name="color">�J���[{1.0f,1.0f,1.0f,1.0f}�����F</param>
			/// <param name="rotation">��]</param>
			/// <param name="scale">�g��</param>
			/// <param name="pivot">�s�{�b�g�i��_�j</param>
			void CFontRender::Init
			(const wchar_t* text,	//�\������e�L�X�g
				Vector2 position,	//���W
				Vector4 color,		//�J���[
				float rotation,		//��]
				float scale,		//�g��
				Vector2 pivot)		//�s�{�b�g�i��_�j
			{
				//���ꂼ��������o�ϐ��ɕێ�������
				wsprintf(m_text, text);
				m_position = position;
				m_color = color;
				m_rotation = rotation;
				m_scale = scale;
				m_pivot = pivot;

				//�f�B�t�@�\�h�����_�����O�ł͂Ȃ�����
				SetIsDefferdRender(false);

				return;
			}

			/// <summary>
			/// �\������e�L�X�g��ݒ肷��
			/// </summary>
			/// <param name="text">�e�L�X�g</param>
			void CFontRender::SetText(const wchar_t* const text)
			{
				swprintf_s(m_text, text);

				return;
			}


			/// <summary>
			/// �`��p�֐�
			/// </summary>
			/// <param name="rc"></param>
			void CFontRender::FontRender(RenderContext& rc)
				//void CFontRender::ForwardRender(RenderContext& rc)
			{
				//�`��J�n
				m_font.Begin(rc);

				//�`�揈��
				m_font.Draw(m_text,
					m_position,
					m_color,
					m_rotation,
					m_scale,
					m_pivot);

				//�`��I��
				m_font.End(rc);

				return;
			}

		}
	}
}