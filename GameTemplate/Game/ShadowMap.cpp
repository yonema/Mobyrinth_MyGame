#include "stdafx.h"
#include "ShadowMap.h"

void CShadowMap::Init()
{
	InitShadowMapRenderTarget();
	InitGaussianBlur();
}



void CShadowMap::InitShadowMapRenderTarget()
{
	//シャドウマップ描画用のレンダリングターゲットを作成する。
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

void CShadowMap::InitGaussianBlur()
{
	m_gaussianBlur.Init(&m_shadowMapRenderTarget.GetRenderTargetTexture());
}


void CShadowMap::CreateShadowMap(const Vector3& direction, const float length)
{
	if (CLightManager::GetInstance()->GetShadowNum() >= g_max_shadowMap)
		return;

	Vector3 dir = direction;
	dir.Normalize();
	Vector3 ligPos = g_vec3Zero - dir;
	ligPos.Scale(length);
	//カメラの位置を設定。これはライトの位置。
	Camera lightCamera;
	lightCamera.SetPosition(ligPos);
	//カメラの注視点を設定。これがライトが照らしている場所。
	lightCamera.SetTarget(g_vec3Zero);
	//上方向を設定。今回はライトが真下を向いているので、X方向を上にしている。
	lightCamera.SetUp({1.0f,0.0f,0.0f});
	//lightCamera.SetUp(g_vec3Up);

	//ライトビュープロジェクション行列を計算している。
	lightCamera.Update();


	m_shadowParam[CLightManager::GetInstance()->GetShadowNum()].lightPos = 
		lightCamera.GetPosition();
	m_shadowParam[CLightManager::GetInstance()->GetShadowNum()].mLVP =
		lightCamera.GetViewProjectionMatrix();

	CLightManager::GetInstance()->AddShadowNum();

}

void CShadowMap::Draw(RenderContext& renderContext)
{
	//シャドウマップにレンダリング。
	//レンダリングターゲットをシャドウマップに変更する。
	renderContext.WaitUntilToPossibleSetRenderTarget(m_shadowMapRenderTarget);
	
	renderContext.SetRenderTargetAndViewport(m_shadowMapRenderTarget);

	renderContext.ClearRenderTargetView(m_shadowMapRenderTarget);

	
	std::list<Model*>::iterator itr = m_shadowModels.begin();
	for (int shadowNum = 0; shadowNum < CLightManager::GetInstance()->GetShadowNum(); shadowNum++)
	{
		Camera lightCamera;
		lightCamera.SetPosition(m_shadowParam[shadowNum].lightPos);
		lightCamera.SetTarget(g_vec3Zero);
		lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		//lightCamera.SetUp(g_vec3Up);
		lightCamera.Update();
		for (; itr != m_shadowModels.end(); itr++)
		{
			//影モデルを描画。
			(*itr)->Draw(renderContext, lightCamera.GetViewMatrix(), lightCamera.GetProjectionMatrix());
			//(*itr)->Draw(renderContext);
		}
	}

	//書き込み完了待ち。
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRenderTarget);


	//step-7 シャドウマップをぼかすためのガウシアンブラーを実行する。
	m_gaussianBlur.ExecuteOnGPU(renderContext, 5.0f);
}

void CShadowMap::AddShadow(Model& shadowModel)
{
	m_shadowModels.push_back(&shadowModel);
}
void CShadowMap::RemoveShadow(Model& shadowModel)
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