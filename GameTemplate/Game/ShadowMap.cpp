#include "stdafx.h"
#include "ShadowMap.h"

/// <summary>
/// 初期化関数
/// </summary>
void CShadowMap::Init()
{
	//シャドウマップのレンダーターゲットの初期化
	InitShadowMapRenderTarget();
	//ガウシアンブラーの初期化
	InitGaussianBlur();
}

/// <summary>
/// シャドウマップのレンダーターゲットの初期化
/// </summary>
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

/// <summary>
/// ガウシアンブラーの初期化
/// </summary>
void CShadowMap::InitGaussianBlur()
{
	m_gaussianBlur.Init(&m_shadowMapRenderTarget.GetRenderTargetTexture());
}

/// <summary>
/// 影を生成するライトを生成する
/// </summary>
/// <param name="direction">影を作るライトの方向</param>
/// <param name="length">ライトがどれくらい離れているか</param>
/// <param name="target">ライトが照らす目標</param>
void CShadowMap::CreateShadowMap
(const Vector3& direction, const float length, const Vector3& target)
{
	//影を生成するライトが規定数より多かったら作らない
	if (CLightManager::GetInstance()->GetShadowNum() >= g_max_shadowMap)
		return;

	//方向と距離からライトのポジションを計算する
	Vector3 dir = direction;
	dir.Normalize();
	dir.Scale(length);
	Vector3 ligPos = target - dir;

	//カメラの位置を設定。これはライトの位置。
	Camera lightCamera;
	lightCamera.SetPosition(ligPos);
	//カメラの注視点を設定。これがライトが照らしている場所。
	lightCamera.SetTarget(target);

	//上方向を設定。
	if (direction.x == 0.0f && direction.z == 0.0f && direction.y != 0.0f)
		//ライトが真下か真上を向いている場合はX方向を上方向に設定する
		lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	else
		//通常はYアップ
		lightCamera.SetUp(g_vec3Up);

	//ライトカメラを並行投影にする
	lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	lightCamera.SetWidth(1024);
	lightCamera.SetHeight(1024);
	//ライトビュープロジェクション行列を計算している。
	lightCamera.Update();


	m_shadowParam[CLightManager::GetInstance()->GetShadowNum()].lightPos = 
		lightCamera.GetPosition();
	m_shadowParam[CLightManager::GetInstance()->GetShadowNum()].mLVP =
		lightCamera.GetViewProjectionMatrix();

	CLightManager::GetInstance()->AddShadowNum();
	m_targetPos = target;
}

/// <summary>
/// 描画関数
/// </summary>
/// <param name="renderContext">レンダーコンテキスト</param>
void CShadowMap::Draw(RenderContext& renderContext)
{
	//シャドウマップにレンダリング。
	//レンダリングターゲットをシャドウマップに変更する。
	renderContext.WaitUntilToPossibleSetRenderTarget(m_shadowMapRenderTarget);
	
	renderContext.SetRenderTargetAndViewport(m_shadowMapRenderTarget);

	renderContext.ClearRenderTargetView(m_shadowMapRenderTarget);

	//シャドウマップに描画するシャドウ用モデルのリストを引っ張ってくる
	std::list<Model*>::iterator itr = m_shadowModels.begin();

	//現在のシャドウの数だけ繰り返す
	for (int shadowNum = 0; shadowNum < CLightManager::GetInstance()->GetShadowNum(); shadowNum++)
	{
		Camera lightCamera;
		lightCamera.SetPosition(m_shadowParam[shadowNum].lightPos);
		lightCamera.SetTarget(m_targetPos);
		Vector3 direction = m_targetPos - m_shadowParam[shadowNum].lightPos;
		//上方向を設定。
		if (direction.x == 0.0f && direction.z == 0.0f && direction.y != 0.0f)
			//ライトが真下か真上を向いている場合はX方向を上方向に設定する
			lightCamera.SetUp({ 1.0f,0.0f,0.0f });
		else
			//通常はYアップ
			lightCamera.SetUp(g_vec3Up);

		//ライトカメラを並行投影にする
		lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		lightCamera.SetWidth(1024);
		lightCamera.SetHeight(1024);
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

/// <summary>
/// シャドウマップに描画するシャドウ用モデルの登録
/// </summary>
/// <param name="shadowModel">登録するシャドウ用モデル</param>
void CShadowMap::AddShadowModel(Model& shadowModel)
{
	m_shadowModels.push_back(&shadowModel);
}

/// <summary>
/// シャドウマップからシャドウ用モデルを破棄する
/// </summary>
/// <param name="shadowModel">破棄するシャドウ用モデル</param>
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
/// 影を生成するライトのパラメーター設定する
/// </summary>
/// <param name="direction">影を作るライトの方向</param>
/// <param name="length">ライトがどれくらい離れているか</param>
/// <param name="target">ライトが照らす目標</param>
void CShadowMap::SetShadowParam
(const Vector3& direction, const float length, const Vector3& target)
{
	Vector3 dir = direction;
	dir.Normalize();
	dir.Scale(length);
	Vector3 ligPos = target - dir;
	//カメラの位置を設定。これはライトの位置。
	Camera lightCamera;
	lightCamera.SetPosition(ligPos);
	//カメラの注視点を設定。これがライトが照らしている場所。
	lightCamera.SetTarget(target);
	//上方向を設定。
	if (direction.x == 0.0f && direction.z == 0.0f && direction.y != 0.0f)
		//ライトが真下か真上を向いている場合はX方向を上方向に設定する
		lightCamera.SetUp({ 1.0f,0.0f,0.0f });
	else
		//通常はYアップ
		lightCamera.SetUp(g_vec3Up);

	//ライトカメラを並行投影にする
	lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	lightCamera.SetWidth(1024);
	lightCamera.SetHeight(1024);
	//ライトビュープロジェクション行列を計算している。
	lightCamera.Update();


	m_shadowParam[0].lightPos =
		lightCamera.GetPosition();
	m_shadowParam[0].mLVP =
		lightCamera.GetViewProjectionMatrix();
	m_targetPos = target;
}