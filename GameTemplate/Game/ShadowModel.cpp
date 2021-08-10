#include "stdafx.h"
#include "ShadowModel.h"

//�R���X�g���N�^
CShadowModel::CShadowModel()
{
	return;
}
//�f�X�g���N�^
CShadowModel::~CShadowModel()
{
	RemoveShadowModel();

	return;
}

/// <summary>
/// �������֐�
/// </summary>
void CShadowModel::Init(const ModelInitData& initData)
{
	m_model.reset(new Model);
	m_model->Init(initData);
	g_graphicsEngine->GetShadowMap().AddShadowModel(*m_model);
	m_isValid = true;

	return;
}

/// <summary>
/// ���f���̏������֐�
/// </summary>
/// <param name="initData">���f���̏������f�[�^</param>
//void CShadowModel::InitModel(ModelInitData& initData)
//{
//	m_model->Init(initData);
//}

/// <summary>
/// ���f���̃p�����[�^�X�V
/// </summary>
/// <param name="pos">�ꏊ</param>
/// <param name="rot">��]</param>
/// <param name="scale">�g��</param>
void CShadowModel::UpdateModel(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
{
	m_model->UpdateWorldMatrix(pos, rot, scale);

	return;
}

/// <summary>
/// �V���h�E�p�̃��f���̓o�^����������
/// </summary>
void CShadowModel::RemoveShadowModel()
{
	//�L�����H
	if (m_isValid)
	{
		g_graphicsEngine->GetShadowMap().RemoveShadowModel(*m_model);
		m_model.reset();
		m_isValid = false;
	}

	return;
}