#include "stdafx.h"
#include "OObox.h"

//ObstacleObject�̃��f���̃t�@�C���p�X��OBB�̃T�C�Y�̒萔�f�[�^���g�p�\�ɂ���
using namespace OOsFilepathAndObbSizeConstData;

//�X�^�[�g�֐�
bool OObox::StartSub()
{
	//�������p�֐�
	Init(MODEL_FILEPATH_BOX, EN_OO_TYPE_BOX);

	//OBB�̃T�C�Y��ݒ�
	SetOBBDirectionLength(OBB_SIZE_BOX);

	//OBB�̃^�O��ݒ�
	SetOBBTag(COBB::EN_OO_TYPE_BOX);

	return true;
}