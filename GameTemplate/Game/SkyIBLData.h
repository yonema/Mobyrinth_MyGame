#pragma once
#include "stdafx.h"


/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �X�J�C
	*/
	namespace nsSky
	{
		/**
		 * @brief IBL�Ɏg����̃f�[�^
		*/
		struct SSkyIBLData
		{
			const wchar_t* m_skyTextureFilepath = nullptr;	//IBL�p�̋�̃e�N�X�`���̃t�@�C���p�X
			float m_luminance = 1.0f;						//IBL�p�̋�ɍ��킹�����邳
			float m_IBLRate = 0.6f;
		};

		/**
		 * @brief ���IBL�Ɏg���萔�o�b�t�@�p�̃f�[�^
		*/
		struct SSkyIBLConstantBuffer
		{
			Vector3 m_eyePos = g_VEC3_ZERO;
			float m_luminance = 1.0f;
			float m_IBLRate = 0.6f;
		};
	}
}