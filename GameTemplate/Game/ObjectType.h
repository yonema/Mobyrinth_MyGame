#pragma once


/// <summary>
/// �I�u�W�F�N�g�̔ԍ�
/// �I�u�W�F�N�g�𑝂₵���炱���Ŕԍ��𑝂₷����
/// </summary>
enum EnObjectType
{
	enEmpty,			//�u����ہv

	//���]�I�u�W�F�N�g
	enWater,			//�u�j���v
	enFire,				//�u�΁v
	enBird,				//�u���v
	enFish,				//�u���v
	enGrilledChicken,	//�u�Ă����v
	enGrilledFish,		//�u�Ă����v
	enRunning,			//�u�ғ��v
	enStop,				//�u��~�v
	enWire,				//�u�j���v
	enString,			//�u�R�v
	enNail,				//�u�B�v
	enBar,				//�u�o�[���v
	enAxe,				//�u���v
	enPickaxe,			//�u��͂��v
	enKeymold,			//�u���̋��^�v
	enKey,				//�u���v
	enPadlock,			//�u�싞���v

	//��Q�I�u�W�F�N�g

	enBigFire,			//�u���v
	enWall,				//�u�ǁv
	enGoal,				//�u�S�[���v
	enNotHavePadlock,	//�u���ĂȂ��싞���v
	enBox,				//�u���v

	enObjectTypeNum		//�I�u�W�F�N�g�^�C�v�̑���
};