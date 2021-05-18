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
		m_model[i]->SetShadowReceiverFlag(true);
	}
	m_model[4]->SetShadowCasterFlag(false);

	m_model[0]->SetPosition({ 200.0f,200.0f,0.0f });
	m_model[1]->SetPosition({ 0.0f,100.0f,0.0f });
	m_model[2]->SetPosition({ -200.0f,100.0f,0.0f });
	m_model[3]->SetPosition({ 0.0f,-200.0f,0.0f });
	m_model[0]->SetEmissionColor({ 0.0f,1.0f,0.0f,1.0f });

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

	Vector3 scale = g_vec3One;

	scale.Scale(75.0f);
	//m_skyModel = NewGO<CModelRender>(0);
	//m_skyModel->Init("Assets/modelData/preset/sky.tkm");
	//m_skyModel->SetScale(scale);

	scale = g_vec3One;
	scale.Scale(100.0f);

	m_sky = NewGO<CSky>(0);
	m_sky->SetScale(scale);
	//m_sky->SetPosition({ 0.0f,0.0f,-6000.0f });

	//g_engine->CreateShadow({ -1.0f,-1.0f,1.0f }, 300.0f);

	m_effect = NewGO<Effect>(0);
	m_effect->Init(u"Assets/effect/laser.efk");
	float scalef = 50.0f;
	m_effect->SetScale({ scalef ,scalef ,scalef });
	m_effect->SetPosition({ 0.0f,0.0f,-100.0f });
	m_effect->SetRotation(qRot2);


	m_fontRender = NewGO<CFontRender>(1);
	m_fontRender->Init(L"5");
	m_fontRender->SetPostRenderFlag(true);
	m_fontRender->SetScale(2.0f);

	
	m_fontRender2 = NewGO<CFontRender>(1);
	m_fontRender2->Init(L"5");
	m_fontRender2->SetPostRenderFlag(true);
	m_fontRender2->SetScale(2.0f);
	return true;
}

void testModel::Update()
{
	//PointLight();

	MoveCamera();

	if (g_pad[0]->IsTrigger(enButtonA)) {
		//再生開始。
		m_effect->Play();
	}

	m_fontAngle += 3.0f;
	if (m_fontAngle > 360.0f)
		m_fontAngle = m_fontAngle - 360.0f;
	//m_fontAngle = 45.0f;
	Vector2 numFontPosition = { 0.0f,0.0f };
	Vector2 addPosition = { 0.0f,20.0f };
	addPosition.Scale((m_fontAngle - (90.0f * static_cast<int>(m_fontAngle / 90.0f))) / 90.0f * (1 + static_cast<int>(m_fontAngle / 90.0f)));
	Vector2 bufPos = addPosition;
	const float radian = Math::DegToRad(m_fontAngle);
	addPosition.x =
		bufPos.x * std::cos(radian) - bufPos.y * std::sin(radian);
	addPosition.y =
		bufPos.x * std::sin(radian) + bufPos.y * std::cos(radian);
	numFontPosition.Add(addPosition);
	//m_fontRender2->SetRotation(radian);
	m_fontRender2->SetPosition(numFontPosition);
	//m_fontRender->SetRotation(radian);
	//m_fontRender->SetPosition(numFontPosition);

}

void testModel::PointLight()
{
	const float moveLen = 10.0f;
	const float moveSide = 750.0f;
	if (m_pointLigMoveDir)
	{
		//右
		m_pointLightPos.x += moveLen;
	}
	else
	{
		//左
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

	m_angleX += g_pad[0]->GetLStickXF() * 0.75f;
	if (m_angleX > 360.0f)
		m_angleX = 0.0f;
	else if (m_angleX < 0.0f)
		m_angleX = 360.0f;

	m_angleY += g_pad[0]->GetLStickYF() * 0.75f;
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