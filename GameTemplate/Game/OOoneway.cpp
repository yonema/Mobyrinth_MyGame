#include "stdafx.h"
#include "OOoneway.h"

//�X�^�[�g�֐�
bool OOoneway::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init("Assets/modelData/oneway.tkm", enOneway);

	GetModelRender()->Deactivate();

	//�ʂ�鎞�̃��f���̐����Ə�����
	m_canPassMR = NewGO<CModelRender>(0);
	m_canPassMR->Init("Assets/modelData/oneway2.tkm", D3D12_CULL_MODE_NONE);
	m_canPassMR->SetPosition(m_position);
	m_canPassMR->SetRotation(m_rotation);
	m_canPassMR->SetScale(m_scale);
	m_canPassMR->Deactivate();

	
	//�ʂ�鎞�̃��f���̐����Ə�����
	m_rightCanPassMR = NewGO<CModelRender>(0);
	m_rightCanPassMR->Init("Assets/modelData/oneway2_r.tkm", D3D12_CULL_MODE_NONE);
	m_rightCanPassMR->SetPosition(m_position);
	m_rightCanPassMR->SetRotation(m_rotation);
	m_rightCanPassMR->SetScale(m_scale);
	m_rightCanPassMR->Deactivate();

	//�ʂ�鎞�̃��f���̐����Ə�����
	m_rightMR = NewGO<CModelRender>(0);
	m_rightMR->Init("Assets/modelData/oneway_r.tkm", D3D12_CULL_MODE_NONE);
	m_rightMR->SetPosition(m_position);
	m_rightMR->SetRotation(m_rotation);
	m_rightMR->SetScale(m_scale);
	m_rightMR->Deactivate();

	//���������H
	if (m_leftOrRight == enLeft)
	{
		//������

		//�������i�ʏ�̌����j�̒ʂ�Ȃ����f����L����
		GetModelRender()->Activate();
	}
	else
	{
		//�E����

		//�E�����i���Ό����j�̒ʂ�Ȃ����f����L����
		m_rightMR->Activate();

	}
	

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

	DeleteGO(m_canPassMR);
	DeleteGO(m_rightCanPassMR);
	DeleteGO(m_rightMR);
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
	{
		//�����Ă��鎞��
		//�ʂ��
		m_sideOBB[m_leftOrRight].SetExceptionFlag(true);

		//���������H
		if (m_leftOrRight == enLeft)
		{
			//������

			//�������̒ʂ�郂�f����L����
			m_canPassMR->Activate();
			//�E�����̒ʂ�郂�f���𖳌���
			m_rightCanPassMR->Deactivate();
		}
		else
		{
			//�E����

			//�������̒ʂ�郂�f���𖳌���
			m_canPassMR->Deactivate();
			//�E�����̒ʂ�郂�f����L����
			m_rightCanPassMR->Activate();
		}

		//�ʂ�Ȃ����f���𖳌���
		GetModelRender()->Deactivate();
		m_rightMR->Deactivate();

	}
	else
	{
		//�����Ă��Ȃ�����
		//�ʂ�Ȃ�
		m_sideOBB[m_leftOrRight].SetExceptionFlag(false);

		//���������H
		if (m_leftOrRight == enLeft)
		{
			//������

			//�������̒ʂ�Ȃ����f����L����
			GetModelRender()->Activate();
			//�E�����̒ʂ�Ȃ����f���𖳌���
			m_rightMR->Deactivate();
		}
		else
		{
			//�E����

			//�������̒ʂ�Ȃ����f���𖳌���
			GetModelRender()->Deactivate();
			//�E�����̒ʂ�Ȃ����f����L����
			m_rightMR->Activate();
		}

		//�ʂ�郂�f���𖳌���
		m_canPassMR->Deactivate();
		m_rightCanPassMR->Deactivate();
	}


	if (m_canPassMR->IsActive())
	{
		m_canPassMR->SetPosition(m_position);
		m_canPassMR->SetRotation(m_rotation);
		m_canPassMR->SetScale(m_scale);
	}
	if (m_rightCanPassMR->IsActive())
	{
		m_rightCanPassMR->SetPosition(m_position);
		m_rightCanPassMR->SetRotation(m_rotation);
		m_rightCanPassMR->SetScale(m_scale);
	}
	if (m_rightMR->IsActive())
	{
		m_rightMR->SetPosition(m_position);
		m_rightMR->SetRotation(m_rotation);
		m_rightMR->SetScale(m_scale);
	}
}