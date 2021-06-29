#pragma once


/// <summary>
/// �I�u�W�F�N�g�̔ԍ�
/// �I�u�W�F�N�g�𑝂₵���炱���Ŕԍ��𑝂₷����
/// </summary>
enum EN_OBJECT_TYPE
{
	EN_OBJECT_TYPE_EMPTY,			//�u����ہv

	/*
	* ���]�I�u�W�F�N�g
	* RO(ReversibleObject)
	*/

	EN_RO_TYPE_WATER,	//�u���v
	EN_RO_TYPE_FIRE,	//�u�΁v
	EN_RO_TYPE_RUNNING,	//�u�ғ��v
	EN_RO_TYPE_STOP,	//�u��~�v
	EN_RO_TYPE_NAIL,	//�u�B�v
	EN_RO_TYPE_BAR,		//�u�o�[���v
	EN_RO_TYPE_AXE,		//�u���v
	EN_RO_TYPE_HAMMER,	//�u��͂��v
	EN_RO_TYPE_KEY,		//�u���v
	EN_RO_TYPE_PADLOCK,	//�u�싞���v
	EN_RO_TYPE_LEFT,	//�u���v
	EN_RO_TYPE_RIGHT,	//�u�E�v

	/*
	* ��Q�I�u�W�F�N�g
	* OO(ObstacleObject)
	*/

	EN_OO_TYPE_FLAME,				//�u���v
	EN_OO_TYPE_WALL,				//�u�ǁv
	EN_OO_TYPE_GOAL,				//�u�S�[���v
	EN_OO_TYPE_BIG_PADLOCK,			//�u�傫�ȓ싞���v
	EN_OO_TYPE_BOX,					//�u���v
	EN_OO_TYPE_ONEWAY,				//�u����ʍs�v
	EN_OO_TYPE_TRANSPARENT_SWITCH,	//�u�X�C�b�`�v
	EN_OO_TYPE_REVERSE_ALL,			//�u�S���]�v
	EN_OO_TYPE_UFO_CAPTURE,			//�uUFO�ɕ߂܂����v
	EN_OO_TYPE_UFO_LANDING_POINT,	//�uUFO�̉^�ѐ�v

	EN_OBJECT_TYPE_NUM				//�I�u�W�F�N�g�^�C�v�̑���
};