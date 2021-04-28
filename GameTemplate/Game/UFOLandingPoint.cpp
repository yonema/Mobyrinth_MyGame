#include "stdafx.h"
#include "UFOLandingPoint.h"

//�X�^�[�g�֐�
bool CUFOLandingPoint::StartSub()
{
	//���f���̏������ƃ^�C�v�̐ݒ�
	Init("Assets/modelData/grilled_chicken.tkm", enEmpty);

	

	//OBBWorld�Ɏ��g��OBB�̓o�^������������
	COBBWorld::GetInstance()->RemoveOBB(&GetOBB());

	//���Ȕ����F��ݒ�
	SetEmissionColor({ 0.5f,0.5f,0.0f,1.0f });

	GetOBB().SetDirectionLength({ 2000.0f,750.0f,500.0f });
	GetOBB().SetPivot({ 0.5f,0.0f,0.5f });

	//�f�o�b�N�p
	//��ŏ���
	Vector3* vertPos = GetOBB().GetBoxVertex();
	for (int i = 0; i < m_vertNum; i++)
	{
		//OBB�̒��_�����邽�߂̃��f��
		m_dbgVertPosMR[i] = NewGO<CModelRender>(0);
		m_dbgVertPosMR[i]->Init("Assets/modelData/dbgBox.tkm");
		m_dbgVertPosMR[i]->SetPosition(vertPos[i]);
		m_dbgVertPosMR[i]->SetRotation(m_rotation);

	}


	return true;
}

