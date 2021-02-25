#pragma once
#include "stdafx.h"
struct SLightParam {
	Vector3 eyePos;				//�����̈ʒu�B
	int numDirectionLight;		//�f�B���N�V�������C�g�̐��B
	Vector3 ambientLight;		//�A���r�G���g���C�g�B
	int numPointLight;			//�|�C���g���C�g�̐��B
	float specPow;				//�X�y�L�����̍i��
};
struct SDirectionLight
{
	Vector4 dirLigColor = { 0.0f,0.0f,0.0f,0.0f };
	Vector3 dirLigDirection = { 0.0f,0.0f,0.0f };
	float pad = 0.0f;
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