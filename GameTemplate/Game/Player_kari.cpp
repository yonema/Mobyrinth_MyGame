#include "stdafx.h"
#include "Player_kari.h"
#include "Light.h"
#include "LightManager.h"

void Player_kari::InitSkeleton()
{
	m_skeleton.Init("Assets/modelData/unityChan.tks");
}
void Player_kari::InitModel()
{
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = "Assets/shader/myModel.fx";
	//�m���X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w�肷��B
	initData.m_vsEntryPointFunc = "VSMain";
	//�X�L�����b�V���p�̒��_�V�F�[�_�[�̃G���g���[�|�C���g���w��B
	initData.m_vsSkinEntryPointFunc = "VSSkinMain";
	//�X�P���g�����w�肷��B
	initData.m_skeleton = &m_skeleton;
	//���f���̏�������w�肷��B
	//3dsMax�ł̓f�t�H���gZ�A�b�v�ɂȂ��Ă��邪�A
	//���j�e�B�����̓A�j���[�V������Y�A�b�v�ɕύX����Ă���B
	initData.m_modelUpAxis = enModelUpAxisY;


	initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLightParam();
	initData.m_expandConstantBufferSize = sizeof(*CLightManager::GetInstance()->GetLightParam());

	initData.m_expandConstantBuffer2 = CLightManager::GetInstance()->GetDirectionLigData();
	initData.m_expandConstantBufferSize2 = sizeof(*CLightManager::GetInstance()->GetDirectionLigData()) * 4;

	//�쐬�����������f�[�^�����ƂɃ��f��������������A
	m_model.Init(initData);

	//�L�����R�����������B
	m_charaCon.Init(40.0f, 100.0f, g_vec3Zero);
}
void Player_kari::InitAnimation()
{
	//�A�j���[�V�����N���b�v�����[�h����B
	m_animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimClip_Run].SetLoopFlag(true);
	//�A�j���[�V�������������B
	m_animation.Init(
		m_skeleton,			//�A�j���[�V�����𗬂����ރX�P���g���B
		m_animationClips,	//�A�j���[�V�����N���b�v�B
		enAnimClip_Num		//�A�j���[�V�����̐��B
	);
}
bool Player_kari::Start()
{
	//�X�P���g�������[�h�B
	InitSkeleton();
	//���f�����������B
	InitModel();
	//�A�j���[�V�������������B
	InitAnimation();

	return true;
}
void Player_kari::Update()
{
	//�X�P���g�����X�V�B
	m_skeleton.Update(m_model.GetWorldMatrix());
	//�A�j���[�V������i�߂�B
	m_animation.Progress(1.0f / 60.0f);

	Vector3 moveSpeed;
	moveSpeed.x = g_pad[0]->GetLStickXF() * -1.0f;
	moveSpeed.z = g_pad[0]->GetLStickYF() * -1.0f;
	m_charaCon.Execute(moveSpeed, 1.0f);
	m_model.UpdateWorldMatrix(m_charaCon.GetPosition(), g_quatIdentity, g_vec3One);

	if (g_pad[0]->IsTrigger(enButtonB))
	{
		DeleteGO(FindGO<CDirectionLight>("directionLight1"));
		
	}

	if (g_pad[0]->IsTrigger(enButtonA))
	{
		float f = 0.0f;
		switch (a)
		{
		case 0:
			f = 0.6f;
			break;
		case 1:
			f = 0.8f;
			break;
		case 2:
			f = 1.0f;
			break;
		case 3:
			f = 1.0f;
			break;
		case 4:
			f = 1.0f;
			break;
		}

		if (a < 3)
		{
			lig[a] = NewGO<CDirectionLight>(0);
			lig[a]->SetDirection({ -f,0.0f,0.0f });
			lig[a]->SetColor({ f,0.0f,0.0f,0.0f });
		}
		a++;
	}




}
void Player_kari::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}