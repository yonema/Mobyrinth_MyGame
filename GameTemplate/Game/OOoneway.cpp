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

	//OBB�̏������f�[�^
	SInitOBBData initOBBData;
	initOBBData.width = 10.0f;				//���FX
	initOBBData.length = 600.0f;			//�����FZ
	initOBBData.height = 400.0f;			//�����FY
	initOBBData.pivot = { 0.5f,0.0f,0.5f };	//��_
	initOBBData.rotation = m_rotation;		//��]
	//���ւ̃x�N�g��
	Vector3 sideVec = g_vec3Right;
	//���g�̉�]�ŉ�
	m_rotation.Apply(sideVec);
	//���ւ̃x�N�g���̒���
	const float sedLen = 300.0f;
	//���ւ̃x�N�g����L�΂�
	sideVec.Scale(sedLen);
	//���T�C�h��OBB�̍��W
	initOBBData.position = m_position + sideVec;
	//���T�C�h��OBB�̏�����
	m_sideOBB[enLeft].Init(initOBBData);
	//���ւ̃x�N�g�����E�ւ̃x�N�g���ɕϊ�
	sideVec.Scale(-1.0f);
	//�E�T�C�h��OBB�̍��W
	initOBBData.position = m_position + sideVec;
	//�E�T�C�h��OBB�̏�����
	m_sideOBB[enRight].Init(initOBBData);
	
	//���T�C�h��OBB��OBB���[���h�ɓo�^����
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		COBBWorld::GetInstance()->AddOBB(&m_sideOBB[i]);
	}

#ifdef MY_DEBUG
	Vector3* vertPos;
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		vertPos = m_sideOBB[i].GetBoxVertex();
		for (int j = 0; j < m_vertNum; j++)
		{
			m_dbgVertMR[i][j] = NewGO<CModelRender>(0);
			m_dbgVertMR[i][j]->Init("Assets/modelData/dbgBox.tkm");
			m_dbgVertMR[i][j]->SetPosition(vertPos[j]);
		}
	}
#endif
	return true;
}

//�f�X�g���N�^
OOoneway::~OOoneway()
{
	//���T�C�h��OBB��OBB���[���h�����������
	for (int i = 0; i < enLeftAndRightNum; i++)
	{
		COBBWorld::GetInstance()->RemoveOBB(&m_sideOBB[i]);
#ifdef MY_DEBUG
		for (int j = 0; j < m_vertNum; j++)
		{
			DeleteGO(m_dbgVertMR[i][j]);
		}
#endif
	}
}

//�A�b�v�f�[�g�֐�
void OOoneway::UpdateSub()
{
	//�v���C���[�����g�Ɠ��������������Ă��邩
	if (m_pPlayer->GetEnLeftOrRight() == m_leftOrRight)
		//�����Ă��鎞��
		//�ʂ��
		m_sideOBB[m_leftOrRight].SetExceptionFlag(true);
	else
		//�����Ă��Ȃ�����
		//�ʂ�Ȃ�
		m_sideOBB[m_leftOrRight].SetExceptionFlag(false);

}