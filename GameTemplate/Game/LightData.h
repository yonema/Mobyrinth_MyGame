#pragma once
#include "stdafx.h"

//���C�g�̋��ʃp�����[�^�̍\����
struct SLightParam {
	Vector3 eyePos;				//�����̈ʒu�B
	int numDirectionLight;		//�f�B���N�V�������C�g�̐��B
	Vector3 ambientLight;		//�A���r�G���g���C�g�B
	int numPointLight;			//�|�C���g���C�g�̐��B
	float specPow;				//�X�y�L�����̍i��
	int numShadow;				//�V���h�E�̐�
	int numSpotLight;			//�X�|�b�g���C�g�̐�
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

//�X�|�b�g���C�g�̃p�����[�^�\����
struct SSpotLight
{
	Vector3 position;	//���W
	float range;		//�e���͈�
	Vector4  color;		//�J���[
	Vector3 direction;	//�ˏo����
	float angle;		//�ˏo�p�x
};

//�V���h�E�̍ő吔
const int g_max_shadowMap = 5;

//�V���h�E�}�b�v�p�̃p�����[�^�\����
struct ShadowParam
{
	Matrix mLVP;		//���C�g�r���[�v���W�F�N�V����
	Vector3 lightPos;	//���C�g�̈ʒu
};