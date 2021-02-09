#include "stdafx.h"
#include "DirectionLight.h"
#include "LightData.h"


CDirectionLight::CDirectionLight()
{
	Init();
}
CDirectionLight::~CDirectionLight()
{

}

void CDirectionLight::Init()
{
	m_light.dirLigDirection.x = 1.0f;
	m_light.dirLigDirection.y = -1.0f;
	m_light.dirLigDirection.z = -1.0f;
	m_light.dirLigDirection.Normalize();			//����
	m_light.dirLigColor = { 1.0f,0.4f,0.4f,1.0f };	//�F
	m_light.eyePos = g_camera3D->GetPosition();		//���_
	m_light.specPow = 5.0f;							//�X�y�L�����̍i��
	m_light.ambinetLight = { 0.6f,0.6f,0.6f };		//����

}
bool CDirectionLight::StartSub()
{
	return true;
};

void CDirectionLight::SetDirection(const Vector3& direction)
{
	m_light.dirLigDirection = direction;
}

void CDirectionLight::SetColor(const Vector4& color)
{
	m_light.dirLigColor = color;
}