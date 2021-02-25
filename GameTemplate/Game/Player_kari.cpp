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


	initData.m_expandConstantBuffer = CLightManager::GetInstance()->GetLightParam();
	initData.m_expandConstantBufferSize = sizeof(*CLightManager::GetInstance()->GetLightParam());

	initData.m_expandConstantBuffer2 = CLightManager::GetInstance()->GetDirectionLigData();
	initData.m_expandConstantBufferSize2 = sizeof(*CLightManager::GetInstance()->GetDirectionLigData()) * 4;

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