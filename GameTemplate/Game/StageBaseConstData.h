#pragma once
#include "ObjectType.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �X�e�[�W
	*/
	namespace nsStage
	{
		/**
		 * @brief �X�e�[�W�x�[�X�̒萔�f�[�^
		*/
		namespace stageBaseConstData
		{
			/*
			* ���x�����[�h�֘A�̒萔
			*/

			//�v���C���[�̃I�u�W�F�N�g�l�[��
			constexpr const wchar_t* const LEVEL_OBJECT_NAME_PLAYER = L"player_kari";
			//���r�E�X�̗ւ̃I�u�W�F�N�g�l�[��
			constexpr const wchar_t* const LEVEL_OBJECT_NAME_MOBIUS = L"Mobius";

			//����ʍs�i���j�̃I�u�W�F�N�g�^�C�v
			constexpr int OO_TYPE_ONEWAY_LEFT = 100;
			//����ʍs�i�E�j�̃I�u�W�F�N�g�^�C�v
			constexpr int OO_TYPE_ONEWAY_RIGHT = 101;

			//�����̃I�u�W�F�N�g�^�C�v
			constexpr int TYPE_TRANSPARENT_OBJECT = 200;

			//�΁i�����j�̃I�u�W�F�N�g�^�C�v
			constexpr int RO_TYPE_FIRE_TRANSPARENT = 
				nsLevelObject::EN_RO_TYPE_FIRE + TYPE_TRANSPARENT_OBJECT;
			//�B�i�����j�̃I�u�W�F�N�g�^�C�v
			constexpr int RO_TYPE_NAIL_TRANSPARENT = 
				nsLevelObject::EN_RO_TYPE_NAIL + TYPE_TRANSPARENT_OBJECT;
			//���Ɓi�����j�̃I�u�W�F�N�g�^�C�v
			constexpr int RO_TYPE_HAMMER_TRANSPARENT = 
				nsLevelObject::EN_RO_TYPE_HAMMER + TYPE_TRANSPARENT_OBJECT;
			//���i�����j�̃I�u�W�F�N�g�^�C�v
			constexpr int RO_TYPE_KEY_TRANSPARENT = 
				nsLevelObject::EN_RO_TYPE_KEY + TYPE_TRANSPARENT_OBJECT;
			//�싞���i�����j�̃I�u�W�F�N�g�^�C�v
			constexpr int RO_TYPE_PADLOCK_TRANSPARENT = 
				nsLevelObject::EN_RO_TYPE_PADLOCK + TYPE_TRANSPARENT_OBJECT;
			//�S�[���i�����j�̃I�u�W�F�N�g�^�C�v
			constexpr int OO_TYPE_GOAL_TRANSPARENT = 
				nsLevelObject::EN_OO_TYPE_GOAL + TYPE_TRANSPARENT_OBJECT;
			//���i�����j�̃I�u�W�F�N�g�^�C�v
			constexpr int OO_TYPE_FLAME_TRANSPARENT = 
				nsLevelObject::EN_OO_TYPE_FLAME + TYPE_TRANSPARENT_OBJECT;
			//���i�����j�̃I�u�W�F�N�g�^�C�v
			constexpr int OO_TYPE_BOX_TRANSPARENT = 
				nsLevelObject::EN_OO_TYPE_BOX + TYPE_TRANSPARENT_OBJECT;


			/**
			 * @brief ���x���̃I�u�W�F�N�g�l�[�����擾
			 * @param [in] objectType �I�u�W�F�N�g�^�C�v
			 * @return �I�u�W�F�N�g�l�[��
			*/
			constexpr const wchar_t* const GetLevelObjectName(const int objectType)
			{
				//�I�u�W�F�N�g�^�C�v�ɑΉ�����I�u�W�F�N�g�l�[����߂�

				switch (objectType)
				{
					/*
					* ���]�I�u�W�F�N�g
					*/
				case nsLevelObject::EN_RO_TYPE_WATER:
					return L"mizu";
					break;
				case nsLevelObject::EN_RO_TYPE_FIRE:
					return L"fire";
					break;
				case nsLevelObject::EN_RO_TYPE_RUNNING:
					return L"kadou";
					break;
				case nsLevelObject::EN_RO_TYPE_STOP:
					return L"teishi";
					break;
				case nsLevelObject::EN_RO_TYPE_NAIL:
					return L"nail";
					break;
				case nsLevelObject::EN_RO_TYPE_HAMMER:
					return L"bar";
					break;
				case nsLevelObject::EN_RO_TYPE_KEY:
					return L"key";
					break;
				case nsLevelObject::EN_RO_TYPE_PADLOCK:
					return L"padlock";
					break;
				case nsLevelObject::EN_RO_TYPE_LEFT:
					return L"left";
					break;
				case nsLevelObject::EN_RO_TYPE_RIGHT:
					return L"right";
					break;

					/*
					* ��Q�I�u�W�F�N�g
					*/
				case nsLevelObject::EN_OO_TYPE_GOAL:
					return L"goal";
					break;
				case nsLevelObject::EN_OO_TYPE_FLAME:
					return L"bigFire";
					break;
				case nsLevelObject::EN_OO_TYPE_WALL:
					return L"wall";
					break;
				case nsLevelObject::EN_OO_TYPE_BIG_PADLOCK:
					return L"notHavePadlock";
					break;
				case nsLevelObject::EN_OO_TYPE_BOX:
					return L"box";
					break;
				case OO_TYPE_ONEWAY_LEFT:
					return L"oneway_left";
					break;
				case OO_TYPE_ONEWAY_RIGHT:
					return L"oneway_right";
					break;
				case nsLevelObject::EN_OO_TYPE_REVERSE_ALL:
					return L"reverseAll";
					break;
				case nsLevelObject::EN_OO_TYPE_UFO_CAPTURE:
					return L"UFO";
					break;
				case nsLevelObject::EN_OO_TYPE_TRANSPARENT_SWITCH:
					return L"switch";
					break;


					/*
					* �������]�I�u�W�F�N�g
					*/

				case RO_TYPE_FIRE_TRANSPARENT:
					return L"fire_switch";
					break;
				case RO_TYPE_NAIL_TRANSPARENT:
					return L"nail_switch";
					break;
				case RO_TYPE_HAMMER_TRANSPARENT:
					return L"bar_switch";
					break;
				case RO_TYPE_KEY_TRANSPARENT:
					return L"key_switch";
					break;
				case RO_TYPE_PADLOCK_TRANSPARENT:
					return L"padlock_switch";
					break;

					/*
					* ������Q�I�u�W�F�N�g
					*/

				case OO_TYPE_GOAL_TRANSPARENT:
					return L"goal_switch";
					break;
				case OO_TYPE_FLAME_TRANSPARENT:
					return L"bigFire_switch";
					break;
				case OO_TYPE_BOX_TRANSPARENT:
					return L"box_switch";
					break;
				}

				//�ǂ�ł��Ȃ�������G���[
				MessageBoxA(
					nullptr, "���x���I�u�W�F�N�g�̖��O���o�^����Ă��܂���", "�G���[", MB_OK
				);

				return nullptr;
			}

			//�E�F�C�|�C���g�̃I�u�W�F�N�g�l�[��
			constexpr const wchar_t* const OBJECT_NAME_WAYPOINT = L"waypoint";

			constexpr const wchar_t* const CHAR_GET_WAYPOINT_NUM = L"0";


			/*
			* �����A�C�e�������ʂ̃A�C�e�����֘A
			*/
			constexpr bool MODE_TRANS = true;		//�������[�h
			constexpr bool MODE_OPACITY = false;	//�s�������[�h


			//��̊g�嗦
			constexpr float MODEL_SCALE_SKY = 1000.0f;

			//�f�t�H���g�̃X�C�b�`�̃^�C��
			constexpr float SWITCH_TIME_DEFAULT = 10.0f;
			//�X�e�[�W4�̃X�C�b�`�̃^�C��
			constexpr float SWITCH_TIME_STAGE_4 = 12.0f;


			/*
			* �S�[���̃X�v���C�g�֘A�̒萔
			*/

			//�S�[���̃X�v���C�g�̃t�@�C���p�X
			constexpr const char* const SPRITE_FILEPATH_GOAL = "Assets/Image/Clear.DDS";
			//�S�[���̃X�v���C�g�̕�
			constexpr float SPRITE_WIDHT_GOAL = 1024.0f;
			//�S�[���̃X�v���C�g�̍���
			constexpr float SPRITE_HEIGHT_GOAL = 512.0f;
			//�S�[���̃X�v���C�g�̍��W
			const Vector3 SPRITE_POSITION_GOAL = { 0.0f,250.0f,0.0f };
			//�S�[���������̑҂�����
			constexpr float TIME_WAIT_GOAL = 4.0f;




			/*
			* �T�E���h�֘A�̒萔
			*/

			//�S�[����SE�̃t�@�C���p�X
			constexpr const wchar_t* const SOUND_FILEPATH_SE_GOAL = L"Assets/sound/goal.wav";
			//�S�[����SE�̉���
			constexpr float SOUND_VOLUME_SE_GOAL = 0.5f;

			//�X�e�[�W��BGM1�̃t�@�C���p�X
			constexpr const wchar_t* const SOUND_FILEPATH_BGM_STAGE_1 = L"Assets/sound/Stage1.wav";
			//�X�e�[�W��BGM2�̃t�@�C���p�X
			constexpr const wchar_t* const SOUND_FILEPATH_BGM_STAGE_2 = L"Assets/sound/Stage2.wav";
			//�X�e�[�W�̃��[�v�pBGM1�̃t�@�C���p�X
			constexpr const wchar_t* const SOUND_FILEPATH_BGM_STAGE_1_LOOP = L"Assets/sound/Stage1.wav";
			//�X�e�[�W�̃��[�v�pBGM2�̃t�@�C���p�X
			constexpr const wchar_t* const SOUND_FILEPATH_BGM_STAGE_2_LOOP = L"Assets/sound/Stage2.wav";

			//BGM�C���^���N�e�B�u�ŉ��ʂ𑝌�������l
			constexpr float SOUND_VOLUME_INC_OR_DEC_BGM_INTERACTIVE = 0.01f;


		}
	}
}