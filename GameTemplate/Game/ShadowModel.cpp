#include "stdafx.h"
#include "ShadowModel.h"


CShadowModel::CShadowModel()
{
}
CShadowModel::~CShadowModel()
{
	if (m_isValid)
		g_graphicsEngine->RemoveShadow(*m_model);
}
void CShadowModel::Init()
{
	m_model.reset(new Model);
	g_graphicsEngine->AddShadow(*m_model);
	m_isValid = true;
}
void CShadowModel::InitModel(ModelInitData& initData)
{
	m_model->Init(initData);
}
void CShadowModel::UpdateModel(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
{
	m_model->UpdateWorldMatrix(pos, rot, scale);
}