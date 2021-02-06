#include "stdafx.h"
#include "Player_kari.h"
#include "Light.h"

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

	//�f�B���N�V�������C�g
	m_directionLig.dirLigDirection.x = 1.0f;
	m_directionLig.dirLigDirection.y = -1.0f;
	m_directionLig.dirLigDirection.z = -1.0f;
	m_directionLig.dirLigDirection.Normalize();			//����
	m_directionLig.dirLigColor = { 1.0f,0.4f,0.4f };	//�F
	m_directionLig.eyePos = g_camera3D->GetPosition();	//���_
	m_directionLig.specPow = 5.0f;						//�X�y�L�����̍i��
	m_directionLig.ambinetLight = { 0.6f,0.6f,0.6f };	//����

	//�|�C���g���C�g
	m_pointLig.ptPosition = { 10.0f,10.0f,0.0f };
	m_pointLig.ptRange = 100.0f;
	m_pointLig.ptColor = { 1.0f,1.0f,0.0f };


	
	//initData.m_expandConstantBuffer = &m_directionLig;
	//initData.m_expandConstantBufferSize = sizeof(m_directionLig);

	//initData.m_expandConstantBuffer2 = &m_pointLig;
	//initData.m_expandConstantBufferSize2 = sizeof(m_pointLig);

	initData.m_expandConstantBuffer = &Light::GetInstance()->m_directionLight;
	initData.m_expandConstantBufferSize = sizeof(Light::GetInstance()->m_directionLight);

	initData.m_expandConstantBuffer2 = &Light::GetInstance()->m_pointLight;
	initData.m_expandConstantBufferSize2 = sizeof(Light::GetInstance()->m_pointLight);


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

	m_pointLig.ptPosition.x += g_pad[0]->GetRStickXF() * -1.0f;
	m_pointLig.ptPosition.z += g_pad[0]->GetRStickYF() * -1.0f;
}
void Player_kari::Render(RenderContext& rc)
{
	m_model.Draw(rc);
}