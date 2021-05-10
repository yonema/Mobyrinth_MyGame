#include "stdafx.h"
#include "OOoneway.h"

//�X�^�[�g�֐�
bool OOoneway::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init("Assets/modelData/oneway.tkm", enOneway);

	//OBB�̃T�C�Y��ݒ�
	Vector3 obbSize;
	obbSize = { 300.0f,300.0f,400.0f };
	//OBB�̕����x�N�g���̒�����ݒ�
	GetOBB().SetDirectionLength(obbSize);

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());


	SInitOBBData initOBBData;
	initOBBData.width = 10.0f;
	initOBBData.length = 400.0f;
	initOBBData.height = 400.0f;
	initOBBData.pivot = { 0.5f,0.0f,0.5f };
	Vector3 sideVec = g_vec3Right;
	m_rotation.Apply(sideVec);
	const float sedLen = 300.0f;
	sideVec.Scale(sedLen);
	initOBBData.position = m_position + sideVec;
	m_sideOBB[enLeft].Init(initOBBData);

	return true;
}

//�f�X�g���N�^
OOoneway::~OOoneway()
{

}

//�A�b�v�f�[�g�֐�
void OOoneway::UpdateSub()
{

}