#include "stdafx.h"
#include "OOpadlock.h"

//ObstacleObject�̃��f���̃t�@�C���p�X��OBB�̃T�C�Y�̒萔�f�[�^���g�p�\�ɂ���
using namespace OOsFilepathAndObbSizeConstData;

//�X�^�[�g�֐�
bool OOpadlock::StartSub()
{
	//�������p�֐�
	Init(MODEL_FILEPATH_PADLOCK, EN_OO_TYPE_BIG_PADLOCK);

	//OBB�̃T�C�Y��ݒ�
	SetOBBDirectionLength(SIZE_OBB_PADLOCK);

	//OBB�̃^�O��ݒ�
	SetOBBTag(COBB::EN_RO_TYPE_PADLOCK);

	return true;
}