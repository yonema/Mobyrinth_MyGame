#pragma once
#include "SkyIBLData.h"

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
		 * @brief ��N���X�̒萔�f�[�^
		*/
		namespace skyConstData
		{
			//��̃��f���̃t�@�C���p�X
			const char* const MODEL_FILEPATH_SKY = "Assets/modelData/preset/sky.tkm";
			//��̃V�F�[�_�[�t�@�C���p�X
			const char* const FX_FILEPATH_SKY = "Assets/shader/SkyCubeMap.fx";

			/**
			 * @brief ��̎��
			*/
			enum EN_SKY_TYPE
			{
				EN_SKY_DEFAULT,		
				EN_SKY_DAY_1,		//��Ɖ_�A�����قڒP�F
				EN_SKY_DAY_2,		//�_�ƌ��Ԃ̌��A�����قڒP�F
				EN_SKY_DAY_3,		//��Ɖ_�A�����ۂ��_������
				EN_SKY_DAY_4,		//�ܓV
				EN_SKY_NIGHT_1,		//�S�ʐ���
				EN_SKY_NIGHT_2,		//���Ɖ_�A�����قڒP�F
				EN_SKY_NIGHT_3,		//��ƐԂ���
				EN_SKY_SPACE_1,		//�F��
				EN_SKY_SPACE_2,		//�[�Ă��F��
				EN_SKY_SUNRISE_1,	//���̏o�A�����قڒP�F
			};

			/**
			 * @brief ��̎�ނɉ�����IBL�Ɏg����̃f�[�^��߂�
			 * @param skyType ��̃^�C�v
			 * @return IBL�Ɏg����̃f�[�At
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
					skyIBLData.m_luminance = 1.1f;	//�����ς�
					skyIBLData.m_IBLRate = 0.4f;	//�����ς�
					break;
				case EN_SKY_DAY_4:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapDay_Toon_04.dds";
					skyIBLData.m_luminance = 1.6f;	//�����ς�
					skyIBLData.m_IBLRate = 0.9f;	//�����ς�
					break;
				case EN_SKY_NIGHT_1:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapNight.dds";
					skyIBLData.m_luminance = 1.1f;
					skyIBLData.m_IBLRate = 0.6f;
					break;
				case EN_SKY_NIGHT_2:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapNight_Toon.dds";
					skyIBLData.m_luminance = 1.2f;	//�����ς�
					skyIBLData.m_IBLRate = 0.4f;	//�����ς�
					break;
				case EN_SKY_NIGHT_3:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapNight_Toon_02.dds";
					skyIBLData.m_luminance = 3.0f;	//�����ς�
					skyIBLData.m_IBLRate = 0.8f;
					break;
				case EN_SKY_SPACE_1:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapSpace_Toon.dds";
					skyIBLData.m_luminance = 1.2f;	//�����ς�
					skyIBLData.m_IBLRate = 0.6f;
					break;
				case EN_SKY_SPACE_2:
					skyIBLData.m_skyTextureFilepath = L"Assets/modelData/preset/skyCubeMapSpace_Toon_2.dds";
					skyIBLData.m_luminance = 1.3f;	//�����ς�
					skyIBLData.m_IBLRate = 0.4f;	//�����ς�
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