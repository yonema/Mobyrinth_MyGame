#include "stdafx.h"
#include "ShadowMap.h"

/// <summary>
/// �������֐�
/// </summary>
void CShadowMap::Init()
{
	//�V���h�E�}�b�v�̃����_�[�^�[�Q�b�g�̏�����
	InitShadowMapRenderTarget();
	//�K�E�V�A���u���[�̏�����
	InitGaussianBlur();
}

/// <summary>
/// �V���h�E�}�b�v�̃����_�[�^�[�Q�b�g�̏�����
/// </summary>
void CShadowMap::InitShadowMapRenderTarget()
{
	//�V���h�E�}�b�v�`��p�̃����_�����O�^�[�Q�b�g���쐬����B
	float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
	m_shadowMapRenderTarget.Create(
		2048,
		2048,
		1,
		1,
		DXGI_FORMAT_R32G32_FLOAT,
		DXGI_FORMAT_D32_FLOAT,
		clearColor
	);
}

/// <summary>
/// �K�E�V�A���u���[�̏�����
/// </summary>
void CShadowMap::InitGaussianBlur()
{
	m_gaussianBlur.Init(&m_shadowMapRenderTarget.GetRenderTargetTexture());
}

/// <summary>
/// �e�𐶐����郉�C�g�𐶐�����
/// </summary>
/// <param name="direction">�e����郉�C�g�̕���</param>
/// <param name="length">���C�g���ǂꂭ�炢����Ă��邩</param>
/// <param name="target">���C�g���Ƃ炷�ڕW</param>
void CShadowMap::CreateShadowMap
(const Vector3& direction, const float length, const Vector3& target)
{
	//�e�𐶐����郉�C�g���K�萔��葽����������Ȃ�
	if (CLightManager::GetInstance()->GetShadowNum() >= g_max_shadowMap)
		return;

	//�����Ƌ������烉�C�g�̃|�W�V�������v�Z����
	Vector3 dir = direction;
	dir.Normalize();
	dir.Scale(length);
	Vector3 ligPos = target - dir;

	//�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
	Camera lightCamera;
	lightCamera.SetPosition(ligPos);
	//�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
	lightCamera.SetTarget(target);

	//�������ݒ�B
	if (direction.x == 0.0f && direction.z == 0.0f && direction.y != 0.0f)
		//���C�g���^�����^��������Ă���ꍇ��X������������ɐݒ肷��
		lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	else
		//�ʏ��Y�A�b�v
		lightCamera.SetUp(g_vec3Up);

	//���C�g�J��������s���e�ɂ���
	lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	lightCamera.SetWidth(1024);
	lightCamera.SetHeight(1024);
	//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
	lightCamera.Update();


	m_shadowParam[CLightManager::GetInstance()->GetShadowNum()].lightPos = 
		lightCamera.GetPosition();
	m_shadowParam[CLightManager::GetInstance()->GetShadowNum()].mLVP =
		lightCamera.GetViewProjectionMatrix();

	CLightManager::GetInstance()->AddShadowNum();
	m_targetPos = target;
}

/// <summary>
/// �`��֐�
/// </summary>
/// <param name="renderContext">�����_�[�R���e�L�X�g</param>
void CShadowMap::Draw(RenderContext& renderContext)
{
	//�V���h�E�}�b�v�Ƀ����_�����O�B
	//�����_�����O�^�[�Q�b�g���V���h�E�}�b�v�ɕύX����B
	renderContext.WaitUntilToPossibleSetRenderTarget(m_shadowMapRenderTarget);
	
	renderContext.SetRenderTargetAndViewport(m_shadowMapRenderTarget);

	renderContext.ClearRenderTargetView(m_shadowMapRenderTarget);

	//�V���h�E�}�b�v�ɕ`�悷��V���h�E�p���f���̃��X�g�����������Ă���
	std::list<Model*>::iterator itr = m_shadowModels.begin();

	//���݂̃V���h�E�̐������J��Ԃ�
	for (int shadowNum = 0; shadowNum < CLightManager::GetInstance()->GetShadowNum(); shadowNum++)
	{
		Camera lightCamera;
		lightCamera.SetPosition(m_shadowParam[shadowNum].lightPos);
		lightCamera.SetTarget(m_targetPos);
		Vector3 direction = m_targetPos - m_shadowParam[shadowNum].lightPos;
		//�������ݒ�B
		if (direction.x == 0.0f && direction.z == 0.0f && direction.y != 0.0f)
			//���C�g���^�����^��������Ă���ꍇ��X������������ɐݒ肷��
			lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		else
			//�ʏ��Y�A�b�v
			lightCamera.SetUp(g_vec3Up);

		//���C�g�J��������s���e�ɂ���
		lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		lightCamera.SetWidth(1024);
		lightCamera.SetHeight(1024);
		lightCamera.Update();
		for (; itr != m_shadowModels.end(); itr++)
		{
			//�e���f����`��B
			(*itr)->Draw(renderContext, lightCamera.GetViewMatrix(), lightCamera.GetProjectionMatrix());
			//(*itr)->Draw(renderContext);
		}
	}

	//�������݊����҂��B
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRenderTarget);


	//step-7 �V���h�E�}�b�v���ڂ������߂̃K�E�V�A���u���[�����s����B
	m_gaussianBlur.ExecuteOnGPU(renderContext, 5.0f);
}

/// <summary>
/// �V���h�E�}�b�v�ɕ`�悷��V���h�E�p���f���̓o�^
/// </summary>
/// <param name="shadowModel">�o�^����V���h�E�p���f��</param>
void CShadowMap::AddShadowModel(Model& shadowModel)
{
	m_shadowModels.push_back(&shadowModel);
}

/// <summary>
/// �V���h�E�}�b�v����V���h�E�p���f����j������
/// </summary>
/// <param name="shadowModel">�j������V���h�E�p���f��</param>
void CShadowMap::RemoveShadowModel(Model& shadowModel)
{
	std::list<Model*>::iterator itr = m_shadowModels.begin();
	for (; itr != m_shadowModels.end(); itr++)
	{
		if (*itr == &shadowModel)
		{
			m_shadowModels.erase(itr);
			break;
		}
	}
}

/// <summary>
/// �e�𐶐����郉�C�g�̃p�����[�^�[�ݒ肷��
/// </summary>
/// <param name="direction">�e����郉�C�g�̕���</param>
/// <param name="length">���C�g���ǂꂭ�炢����Ă��邩</param>
/// <param name="target">���C�g���Ƃ炷�ڕW</param>
void CShadowMap::SetShadowParam
(const Vector3& direction, const float length, const Vector3& target)
{
	Vector3 dir = direction;
	dir.Normalize();
	dir.Scale(length);
	Vector3 ligPos = target - dir;
	//�J�����̈ʒu��ݒ�B����̓��C�g�̈ʒu�B
	Camera lightCamera;
	lightCamera.SetPosition(ligPos);
	//�J�����̒����_��ݒ�B���ꂪ���C�g���Ƃ炵�Ă���ꏊ�B
	lightCamera.SetTarget(target);
	//�������ݒ�B
	if (direction.x == 0.0f && direction.z == 0.0f && direction.y != 0.0f)
		//���C�g���^�����^��������Ă���ꍇ��X������������ɐݒ肷��
		lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	else
		//�ʏ��Y�A�b�v
		lightCamera.SetUp(g_vec3Up);

	//���C�g�J��������s���e�ɂ���
	lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	lightCamera.SetWidth(1024);
	lightCamera.SetHeight(1024);
	//���C�g�r���[�v���W�F�N�V�����s����v�Z���Ă���B
	lightCamera.Update();


	m_shadowParam[0].lightPos =
		lightCamera.GetPosition();
	m_shadowParam[0].mLVP =
		lightCamera.GetViewProjectionMatrix();
	m_targetPos = target;
}