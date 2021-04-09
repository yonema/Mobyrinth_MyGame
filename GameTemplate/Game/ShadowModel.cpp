#include "stdafx.h"
#include "ShadowModel.h"

//�R���X�g���N�^
CShadowModel::CShadowModel()
{
}
//�f�X�g���N�^
CShadowModel::~CShadowModel()
{
	if (m_isValid)
		g_graphicsEngine->RemoveShadowModel(*m_model);
}

/// <summary>
/// �������֐�
/// </summary>
void CShadowModel::Init()
{
	m_model.reset(new Model);
	g_graphicsEngine->AddShadowModel(*m_model);
	m_isValid = true;
}

/// <summary>
/// ���f���̏������֐�
/// </summary>
/// <param name="initData">���f���̏������f�[�^</param>
void CShadowModel::InitModel(ModelInitData& initData)
{
	m_model->Init(initData);
}

/// <summary>
/// ���f���̃p�����[�^�X�V
/// </summary>
/// <param name="pos">�ꏊ</param>
/// <param name="rot">��]</param>
/// <param name="scale">�g��</param>
void CShadowModel::UpdateModel(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
{
	m_model->UpdateWorldMatrix(pos, rot, scale);
}