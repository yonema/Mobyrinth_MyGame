#include "stdafx.h"
#include "stage_kari.h"

bool stage_kari::StartSub()
{
	//�T���h�{�b�N�X�X�e�[�W
	//LoadLevel("Assets/level/stage_kari02.tkl");

	//�{�ԗp�X�e�[�W�m�F�p
	//�ǂꂩ���LoadLevel�̃R�����g�������Ċm�F���ĂˁB
	//���̑���LoadLevel�͑S���R�����g�A�E�g���ĂˁB

	//�����Y
	LoadLevel("Assets/level/Y_easy.tkl");
	//LoadLevel("Assets/level/Y_normal.tkl");
	//LoadLevel("Assets/level/Y_hard.tkl");

	//�����O
	//LoadLevel("Assets/level/O_easy.tkl");
	//LoadLevel("Assets/level/O_normal.tkl");
	//LoadLevel("Assets/level/O_hard.tkl");	

	//�����T
	//LoadLevel("Assets/level/T_easy.tkl");
	//LoadLevel("Assets/level/T_normal.tkl");
	//LoadLevel("Assets/level/T_hard.tkl");


	
	return true;
}
stage_kari::~stage_kari()
{

}


