#pragma once
#include "stdafx.h"

//���C�g�̋��ʃp�����[�^�̍\����
struct SLightParam {
	Vector3 eyePos;				//�����̈ʒu�B
	int numDirectionLight;		//�f�B���N�V�������C�g�̐��B
	Vector3 ambientLight;		//�A���r�G���g���C�g�B
	int numPointLight;			//�|�C���g���C�g�̐��B
	float specPow;				//�X�y�L�����̍i��
	int numShadow;	//
};

//�f�B���N�V�������C�g�̃p�����[�^�̍\����
struct SDirectionLight
{
	Vector4 dirLigColor = { 0.0f,0.0f,0.0f,0.0f };	//���C�g�̃J���[
	Vector3 dirLigDirection = { 0.0f,0.0f,0.0f };	//���C�g�̕���
	float pad = 0.0f;								//�p�b�h�B�O�̂���
};

//�|�C���g���C�g�̃p�����[�^�̍\����
struct SPointLight
{
	Vector3 ptPosition;		//�ʒu�B
	float ptRange;			//�e���͈́B
	Vector4 ptColor;		//�J���[�B
};

const int g_max_shadowMap = 5;

//�V���h�E�}�b�v�p�̃p�����[�^�\����
struct ShadowParam
{
	Matrix mLVP;		//���C�g�r���[�v���W�F�N�V����
	Vector3 lightPos;	//���C�g�̈ʒu
};