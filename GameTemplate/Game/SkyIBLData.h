#pragma once
#include "stdafx.h"


/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief スカイ
	*/
	namespace nsSky
	{
		/**
		 * @brief IBLに使う空のデータ
		*/
		struct SSkyIBLData
		{
			const wchar_t* m_skyTextureFilepath = nullptr;	//IBL用の空のテクスチャのファイルパス
			float m_luminance = 1.0f;						//IBL用の空に合わせた明るさ
		};

		/**
		 * @brief 空のIBLに使う定数バッファ用のデータ
		*/
		struct SSkyIBLConstantBuffer
		{
			Vector3 m_eyePos = g_VEC3_ZERO;
			float m_luminance = 1.0f;
		};
	}
}