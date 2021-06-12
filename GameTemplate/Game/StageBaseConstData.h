#pragma once
#include "ObjectType.h"

namespace stageBaseConstData
{
	//�v���C���[�̃I�u�W�F�N�g�l�[��
	const wchar_t* const LEVEL_OBJECT_NAME_PLAYER = L"player_kari";
	//���r�E�X�̗ւ̃I�u�W�F�N�g�l�[��
	const wchar_t* const LEVEL_OBJECT_NAME_MOBIUS = L"Mobius";

	//����ʍs�i���j�̃I�u�W�F�N�g�^�C�v
	const int OO_TYPE_ONEWAY_LEFT = 100;
	//����ʍs�i�E�j�̃I�u�W�F�N�g�^�C�v
	const int OO_TYPE_ONEWAY_RIGHT = 101;

	//�����̃I�u�W�F�N�g�^�C�v
	const int TYPE_TRANSPARENT_OBJECT = 200;

	//�΁i�����j�̃I�u�W�F�N�g�^�C�v
	const int RO_TYPE_FIRE_TRANSPARENT = EN_RO_TYPE_FIRE + TYPE_TRANSPARENT_OBJECT;
	//�B�i�����j�̃I�u�W�F�N�g�^�C�v
	const int RO_TYPE_NAIL_TRANSPARENT = EN_RO_TYPE_NAIL + TYPE_TRANSPARENT_OBJECT;
	//���Ɓi�����j�̃I�u�W�F�N�g�^�C�v
	const int RO_TYPE_HAMMER_TRANSPARENT = EN_RO_TYPE_HAMMER + TYPE_TRANSPARENT_OBJECT;
	//���i�����j�̃I�u�W�F�N�g�^�C�v
	const int RO_TYPE_KEY_TRANSPARENT = EN_RO_TYPE_KEY + TYPE_TRANSPARENT_OBJECT;
	//�싞���i�����j�̃I�u�W�F�N�g�^�C�v
	const int RO_TYPE_PADLOCK_TRANSPARENT = EN_RO_TYPE_PADLOCK + TYPE_TRANSPARENT_OBJECT;
	//�S�[���i�����j�̃I�u�W�F�N�g�^�C�v
	const int OO_TYPE_GOAL_TRANSPARENT = EN_OO_TYPE_GOAL + TYPE_TRANSPARENT_OBJECT;
	//���i�����j�̃I�u�W�F�N�g�^�C�v
	const int OO_TYPE_FLAME_TRANSPARENT = EN_OO_TYPE_FLAME + TYPE_TRANSPARENT_OBJECT;
	//���i�����j�̃I�u�W�F�N�g�^�C�v
	const int OO_TYPE_BOX_TRANSPARENT = EN_OO_TYPE_BOX + TYPE_TRANSPARENT_OBJECT;


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
		case EN_RO_TYPE_WATER:
			return L"mizu";
			break;
		case EN_RO_TYPE_FIRE:
			return L"fire";
			break;
		case EN_RO_TYPE_RUNNING:
			return L"kadou";
			break;
		case EN_RO_TYPE_STOP:
			return L"teishi";
			break;
		case EN_RO_TYPE_NAIL:
			return L"nail";
			break;
		case EN_RO_TYPE_HAMMER:
			return L"bar";
			break;
		case EN_RO_TYPE_KEY:
			return L"key";
			break;
		case EN_RO_TYPE_PADLOCK:
			return L"padlock";
			break;
		case EN_RO_TYPE_LEFT:
			return L"left";
			break;
		case EN_RO_TYPE_RIGHT:
			return L"right";
			break;

			/*
			* ��Q�I�u�W�F�N�g
			*/
		case EN_OO_TYPE_GOAL:
			return L"goal";
			break;
		case EN_OO_TYPE_FLAME:
			return L"bigFire";
			break;
		case EN_OO_TYPE_WALL:
			return L"wall";
			break;
		case EN_OO_TYPE_BIG_PADLOCK:
			return L"notHavePadlock";
			break;
		case EN_OO_TYPE_BOX:
			return L"box";
			break;
		case OO_TYPE_ONEWAY_LEFT:
			return L"oneway_left";
			break;
		case OO_TYPE_ONEWAY_RIGHT:
			return L"oneway_right";
			break;
		case EN_OO_TYPE_REVERSE_ALL:
			return L"reverseAll";
			break;
		case EN_GO_TYPE_UFO:
			return L"UFO";
			break;
		case EN_OO_TYPE_TRANSPARENT_SWITCH:
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
	const wchar_t* const OBJECT_NAME_WAYPOINT = L"waypoint";

	const wchar_t* const CHAR_GET_WAYPOINT_NUM = L"0";


	const bool MODE_TRANS = true;		//�������[�h
	const bool MODE_OPACITY = false;	//�s�������[�h
}