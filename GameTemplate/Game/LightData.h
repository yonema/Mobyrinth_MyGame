#pragma once
#include "stdafx.h"

struct SDirectionLight
{
	Vector4 dirLigColor;
	Vector3 dirLigDirection;
	////�f�B���N�V�������C�g
	//Vector3 dirLigDirection;
	//float pad;
	//Vector4 dirLigColor;
	//Vector3 eyePos;			//�J�����̈ʒu�B
	//float specPow;				//�X�y�L�����̍i��B
	//Vector3 ambinetLight;	//�����B	
};

//�|�C���g���C�g�̍\����
struct SPointLight
{
	Vector3 ptPosition;		//�ʒu�B
	float ptRange;			//�e���͈́B
	Vector4 ptColor;		//�J���[�B
};