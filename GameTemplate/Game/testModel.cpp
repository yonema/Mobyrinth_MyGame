#include "stdafx.h"
#include "testModel.h"

bool testModel::Start()
{
	for (int i = 0; i < 5; i++)
	{
		m_model[i] = NewGO<CModelRender>(0);
	}
	m_model[0]->Init("Assets/modelData/grilled chicken.tkm");
	m_model[1]->Init("Assets/modelData/grilled chicken.tkm");
	m_model[2]->Init("Assets/modelData/grilled chicken.tkm");
	m_model[3]->Init("Assets/modelData/grilled chicken.tkm");
	m_model[4]->Init("Assets/modelData/wall.tkm");
	for (int i = 0; i < 5; i++)
	{
		if (i == 4)
			continue;
		m_model[i]->SetShadowCasterFlag(true);
		//m_model[i]->SetShadowReceiverFlag(true);
	}
	//m_model[0]->SetShadowCasterFlag(true);
	m_model[4]->SetShadowReceiverFlag(true);

	m_model[0]->SetPosition({ 200.0f,200.0f,0.0f });
	m_model[1]->SetPosition({ 0.0f,100.0f,0.0f });
	m_model[2]->SetPosition({ -200.0f,100.0f,0.0f });
	m_model[3]->SetPosition({ 0.0f,-200.0f,0.0f });

	m_model[4]->SetPosition({ -500.0f,000.0f,0.0f });
	Quaternion qRot;
	qRot.SetRotationDegX(90.0f);
	Quaternion qRot2;
	qRot2.SetRotationDegY(90.0f);
	qRot.Multiply(qRot2);
	m_model[4]->SetRotation(qRot);

	g_camera3D->SetPosition({ 0.0f,100.0f,-1000.0f });
	g_camera3D->SetTarget({ 0.0f,0.0f,0.0f });

	m_pointLightPos.y = 100.0f;
	m_ptLigPosModel = NewGO<CModelRender>(0);
	m_ptLigPosModel->Init("Assets/modelData/yuka.tkm");
	m_ptLigPosModel->SetPosition(m_pointLightPos);

	return true;
}

void testModel::Update()
{
	PointLight();

	MoveCamera();
}

void testModel::PointLight()
{
	const float moveLen = 10.0f;
	const float moveSide = 750.0f;
	if (m_pointLigMoveDir)
	{
		//‰E
		m_pointLightPos.x += moveLen;
	}
	else
	{
		//¶
		m_pointLightPos.x -= moveLen;
	}

	if (m_pointLightPos.x >= moveSide || m_pointLightPos.x <= -moveSide)
	{
		m_pointLigMoveDir = !m_pointLigMoveDir;
	}
	m_ptLigPosModel->SetPosition(m_pointLightPos);

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		CPointLight* pointLig = NewGO<CPointLight>(0, "PointLight");
		pointLig->SetPosition(m_pointLightPos);
		pointLig->SetColor({ 1.0f,100.0f,1.0f,1.0f });
		pointLig->SetRange(1000.0f);
	}
	else if (g_pad[0]->IsTrigger(enButtonB))
	{
		DeleteGO(FindGO<CPointLight>("PointLight"));
	}
}

void testModel::MoveCamera()
{
	Vector3 toCameraPos = { 0.0f,100.0f,-1000.0f };

	m_angleX += g_pad[0]->GetLStickXF() * 0.75;
	if (m_angleX > 80.0f)
		m_angleX = 80.0f;
	else if (m_angleX < -80.0f)
		m_angleX = -80.0f;

	m_angleY += g_pad[0]->GetLStickYF() * 0.75;
	if (m_angleY > 80.0f)
		m_angleY = 80.0f;
	else if (m_angleY < -80.0f)
		m_angleY = -80.0f;

	Quaternion qRotX;
	qRotX.SetRotationDegY(m_angleX);
	Vector3 front = { 0.0f,0.0f,1.0f };
	qRotX.Apply(front);
	Vector3 x;
	x = Cross(g_vec3Up, front);
	Quaternion qRotY;
	qRotY.SetRotationDeg(x, m_angleY);

	qRotX.Apply(toCameraPos);
	qRotY.Apply(toCameraPos);

	//toCameraPos = { -600.0f * 0.2f , 600.0f,0.0f };
	g_camera3D->SetPosition(toCameraPos);
}