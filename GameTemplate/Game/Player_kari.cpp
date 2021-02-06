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
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = "Assets/modelData/unityChan.tkm";
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/myModel.fx";
	//ノンスキンメッシュ用の頂点シェーダーのエントリーポイントを指定する。
	initData.m_vsEntryPointFunc = "VSMain";
	//スキンメッシュ用の頂点シェーダーのエントリーポイントを指定。
	initData.m_vsSkinEntryPointFunc = "VSSkinMain";
	//スケルトンを指定する。
	initData.m_skeleton = &m_skeleton;
	//モデルの上方向を指定する。
	//3dsMaxではデフォルトZアップになっているが、
	//ユニティちゃんはアニメーションでYアップに変更されている。
	initData.m_modelUpAxis = enModelUpAxisY;

	//ディレクションライト
	m_directionLig.dirLigDirection.x = 1.0f;
	m_directionLig.dirLigDirection.y = -1.0f;
	m_directionLig.dirLigDirection.z = -1.0f;
	m_directionLig.dirLigDirection.Normalize();			//方向
	m_directionLig.dirLigColor = { 1.0f,0.4f,0.4f };	//色
	m_directionLig.eyePos = g_camera3D->GetPosition();	//視点
	m_directionLig.specPow = 5.0f;						//スペキュラの絞り
	m_directionLig.ambinetLight = { 0.6f,0.6f,0.6f };	//環境光

	//ポイントライト
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


	//作成した初期化データをもとにモデルを初期化する、
	m_model.Init(initData);

	//キャラコンを初期化。
	m_charaCon.Init(40.0f, 100.0f, g_vec3Zero);
}
void Player_kari::InitAnimation()
{
	//アニメーションクリップをロードする。
	m_animationClips[enAnimClip_Idle].Load("Assets/animData/idle.tka");
	m_animationClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animationClips[enAnimClip_Run].Load("Assets/animData/walk.tka");
	m_animationClips[enAnimClip_Run].SetLoopFlag(true);
	//アニメーションを初期化。
	m_animation.Init(
		m_skeleton,			//アニメーションを流し込むスケルトン。
		m_animationClips,	//アニメーションクリップ。
		enAnimClip_Num		//アニメーションの数。
	);
}
bool Player_kari::Start()
{
	//スケルトンをロード。
	InitSkeleton();
	//モデルを初期化。
	InitModel();
	//アニメーションを初期化。
	InitAnimation();

	return true;
}
void Player_kari::Update()
{
	//スケルトンを更新。
	m_skeleton.Update(m_model.GetWorldMatrix());
	//アニメーションを進める。
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