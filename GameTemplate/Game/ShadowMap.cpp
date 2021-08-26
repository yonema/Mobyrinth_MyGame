#include "stdafx.h"
#include "ShadowMap.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief グラフィック
	*/
	namespace nsGraphic
	{
		/**
		 * @brief シャドウ
		*/
		namespace nsShadow
		{

			using namespace shadowConstData;

			/// <summary>
			/// 初期化関数
			/// </summary>
			void CShadowMap::Init()
			{
				//シャドウマップのレンダーターゲットの初期化
				InitShadowMapRenderTarget();
				//ガウシアンブラーの初期化
				InitGaussianBlur();

				return;
			}

			/// <summary>
			/// シャドウマップのレンダーターゲットの初期化
			/// </summary>
			void CShadowMap::InitShadowMapRenderTarget()
			{
				//シャドウマップ描画用のレンダリングターゲットを作成する。
				float clearColor[4] =
				{ COLOR_CLEAR_SHADOWMAP,COLOR_CLEAR_SHADOWMAP,COLOR_CLEAR_SHADOWMAP,COLOR_CLEAR_SHADOWMAP };
				m_shadowMapRenderTarget.Create(
					WIDTH_RENDERTARGET_SHADOWMAP,
					HEIGHT_RENDERTARGET_SHADOWMAP,
					1,
					1,
					DXGI_FORMAT_R32G32_FLOAT,
					DXGI_FORMAT_D32_FLOAT,
					clearColor
				);

				return;
			}

			/// <summary>
			/// ガウシアンブラーの初期化
			/// </summary>
			void CShadowMap::InitGaussianBlur()
			{
				//ガウシアンブラーの初期化
				m_gaussianBlur.Init(&m_shadowMapRenderTarget.GetRenderTargetTexture());

				return;
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
				if (nsLight::CLightManager::GetInstance()->GetShadowNum() >= nsLight::g_MAX_SHADOW_NUM)
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
					lightCamera.SetUp(g_VEC3_RIGHT);
				else
					//通常はYアップ
					lightCamera.SetUp(g_VEC3_UP);

				//ライトカメラを並行投影にする
				lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
				//ライトビュープロジェクション行列を計算している。
				lightCamera.Update();

				//シャドウのパラメーターにライトの座標と設定
				m_shadowParam[nsLight::CLightManager::GetInstance()->GetShadowNum()].lightPos =
					lightCamera.GetPosition();
				//シャドウのパラメーターにライトビュープロジェクションを設定
				m_shadowParam[nsLight::CLightManager::GetInstance()->GetShadowNum()].mLVP =
					lightCamera.GetViewProjectionMatrix();

				//注視点を設定
				m_targetPos = target;

				//ライトマネージャーにライトが増えたと報告する
				nsLight::CLightManager::GetInstance()->AddShadowNum();

				return;
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



				//現在のシャドウの数だけ繰り返す
				for (
					int shadowNum = 0;
					shadowNum < nsLight::CLightManager::GetInstance()->GetShadowNum();
					shadowNum++
					)
				{
					//シャドウマップに描画するシャドウ用モデルのリストを引っ張ってくる
					std::vector<Model*>::iterator itr = m_shadowModels.begin();

					Camera lightCamera;
					lightCamera.SetPosition(m_shadowParam[shadowNum].lightPos);
					lightCamera.SetTarget(m_targetPos);
					Vector3 direction = m_targetPos - m_shadowParam[shadowNum].lightPos;
					//上方向を設定。
					if (direction.x == 0.0f && direction.z == 0.0f && direction.y != 0.0f)
						//ライトが真下か真上を向いている場合はX方向を上方向に設定する
						lightCamera.SetUp(g_VEC3_RIGHT);
					else
						//通常はYアップ
						lightCamera.SetUp(g_VEC3_UP);

					//ライトカメラを並行投影にする
					lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
					lightCamera.Update();
					for (; itr != m_shadowModels.end(); itr++)
					{
						//影モデルを描画。
						(*itr)->Draw(renderContext, lightCamera.GetViewMatrix(), lightCamera.GetProjectionMatrix());
					}
				}

				//書き込み完了待ち。
				renderContext.WaitUntilFinishDrawingToRenderTarget(m_shadowMapRenderTarget);


				//step-7 シャドウマップをぼかすためのガウシアンブラーを実行する。
				m_gaussianBlur.ExecuteOnGPU(renderContext, POWER_BLUR_DEFAULT);

				return;
			}

			/// <summary>
			/// シャドウマップに描画するシャドウ用モデルの登録
			/// </summary>
			/// <param name="shadowModel">登録するシャドウ用モデル</param>
			void CShadowMap::AddShadowModel(Model& shadowModel)
			{
				//シャドウモデルのコンテナに登録する
				m_shadowModels.push_back(&shadowModel);

				return;
			}

			/// <summary>
			/// シャドウマップからシャドウ用モデルを破棄する
			/// </summary>
			/// <param name="shadowModel">破棄するシャドウ用モデル</param>
			void CShadowMap::RemoveShadowModel(Model& shadowModel)
			{
				//一度に一つまでしか消さない！

				//シャドウモデルのコンテナのイテレーター
				std::vector<Model*>::iterator itr = m_shadowModels.begin();
				for (; itr != m_shadowModels.end(); itr++)
				{
					if (*itr == &shadowModel)
					{
						//破棄するシャドウモデルが見つかったら、破棄する
						m_shadowModels.erase(itr);
						break;
					}
				}

				return;
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
					lightCamera.SetUp(g_VEC3_RIGHT);
				else
					//通常はYアップ
					lightCamera.SetUp(g_VEC3_UP);

				//ライトカメラを並行投影にする
				lightCamera.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
				//ライトビュープロジェクション行列を計算している。
				lightCamera.Update();

				//シャドウのパラメーターにライトの座標と設定
				m_shadowParam[0].lightPos =
					lightCamera.GetPosition();
				//シャドウのパラメーターにライトビュープロジェクションを設定
				m_shadowParam[0].mLVP =
					lightCamera.GetViewProjectionMatrix();

				//注視点を設定
				m_targetPos = target;

				return;
			}

		}
	}
}