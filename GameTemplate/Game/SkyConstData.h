#pragma once
#include "SkyIBLData.h"

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
		 * @brief 空クラスの定数データ
		*/
		namespace skyConstData
		{
			//空のモデルのファイルパス
			const char* const MODEL_FILEPATH_SKY = "Assets/modelData/preset/sky.tkm";
			//空のシェーダーファイルパス
			const char* const FX_FILEPATH_SKY = "Assets/shader/SkyCubeMap.fx";

			/**
			 * @brief 空の種類
			*/
			enum EN_SKY_TYPE
			{
				EN_SKY_DEFAULT,		
				EN_SKY_DAY_1,		//青空と雲、下がほぼ単色
				EN_SKY_DAY_2,		//雲と隙間の光、下がほぼ単色
				EN_SKY_DAY_3,		//青空と雲、紫っぽい雲もある
				EN_SKY_DAY_4,		//曇天
				EN_SKY_NIGHT_1,		//全面星空
				EN_SKY_NIGHT_2,		//夜空と雲、下がほぼ単色
				EN_SKY_NIGHT_3,		//夜と赤い月
				EN_SKY_SPACE_1,		//宇宙
				EN_SKY_SPACE_2,		//夕焼け宇宙
				EN_SKY_SUNRISE_1,	//日の出、下がほぼ単色
			};

			/**
			 * @brief 空の種類に応じたIBLに使う空のデータを戻す
			 * @param skyType 空のタイプ
			 * @return IBLに使う空のデーアt
			*/
			constexpr SSkyIBLData GetIBLLuminanceForSky(int skyType)
			{
				SSkyIBLData skyIBLData;

				switch (skyType)
				{
				case EN_SKY_DAY_1:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapDay_Toon.dds";
					skyIBLData.m_luminance = 1.1f;
					skyIBLData.m_IBLRate = 0.6f;
					break;
				case EN_SKY_DAY_2:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapDay_Toon_02.dds";
					skyIBLData.m_luminance = 1.1f;
					skyIBLData.m_IBLRate = 0.6f;
					break;
				case EN_SKY_DAY_3:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapDay_Toon_03.dds";
					skyIBLData.m_luminance = 1.1f;	//調整済み
					skyIBLData.m_IBLRate = 0.4f;	//調整済み
					break;
				case EN_SKY_DAY_4:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapDay_Toon_04.dds";
					skyIBLData.m_luminance = 1.6f;	//調整済み
					skyIBLData.m_IBLRate = 0.9f;	//調整済み
					break;
				case EN_SKY_NIGHT_1:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapNight.dds";
					skyIBLData.m_luminance = 1.1f;
					skyIBLData.m_IBLRate = 0.6f;
					break;
				case EN_SKY_NIGHT_2:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapNight_Toon.dds";
					skyIBLData.m_luminance = 1.2f;	//調整済み
					skyIBLData.m_IBLRate = 0.4f;	//調整済み
					break;
				case EN_SKY_NIGHT_3:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapNight_Toon_02.dds";
					skyIBLData.m_luminance = 3.0f;	//調整済み
					skyIBLData.m_IBLRate = 0.8f;
					break;
				case EN_SKY_SPACE_1:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapSpace_Toon.dds";
					skyIBLData.m_luminance = 1.2f;	//調整済み
					skyIBLData.m_IBLRate = 0.6f;
					break;
				case EN_SKY_SPACE_2:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapSpace_Toon_2.dds";
					skyIBLData.m_luminance = 1.3f;	//調整済み
					skyIBLData.m_IBLRate = 0.4f;	//調整済み
					break;
				case EN_SKY_SUNRISE_1:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapSunrise_Toon.dds";
					skyIBLData.m_luminance = 1.1f;
					skyIBLData.m_IBLRate = 0.6f;
					break;
				case EN_SKY_DEFAULT:
				default:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapDay_Toon.dds";
					skyIBLData.m_luminance = 1.1f;
					skyIBLData.m_IBLRate = 0.6f;
					break;
				}

				return skyIBLData;
			}
		}
	}
}