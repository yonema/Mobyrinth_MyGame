#include "stdafx.h"
#include "ObstacleObject.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief レベルオブジェクト
	*/
	namespace nsLevelObject
	{
		/**
		 * @brief 障害オブジェクト
		*/
		namespace nsObstacleObject
		{

			//スタート関数
			bool CObstacleObject::PureVirtualStart()
			{
				//OBBWorldに自身のOBBを登録する
				//これによってPlayerが通れなくなる
				nsOBB::COBBWorld::GetInstance()->AddOBB(GetOBB());

				//表側にいるか裏側にいるか調べる
				//反転オブジェクトではないからfalseを渡す
				CheckFrontOrBackSide(false);

				//オーバーライドしてほしい関数StartSub()はここで呼ばれる。
				return StartSub();
			}


			//デストラクタ
			CObstacleObject::~CObstacleObject()
			{
				//LevelObjectManagerにこのオブジェクトは消すよ！って伝える。
				Delete();

				//OBBWorldに自身のOBBの登録を消去させる
				nsOBB::COBBWorld::GetInstance()->RemoveOBB(GetOBB());

				//モデルの消去
				DeleteGO(m_modelRender);

				return;
			}



			/// <summary>
			/// 初期化関数
			/// 最初に呼んでね。trueを戻してね。
			/// モデルのファイルパスとオブジェクトのタイプを
			/// 設定する
			/// タイプ一覧はObjectType.hを参照
			/// </summary>
			/// <param name="filePath">モデルのtkmファイルパス</param>
			/// <param name="objectType">タイプ</param>
			/// <returns>trueを戻してね</returns>
			bool CObstacleObject::Init(const char* filePath, int objectType)
			{
				//モデルレンダラーを生成
				m_modelRender = NewGO<nsGraphic::nsModel::CModelRender>(nsCommonData::PRIORITY_FIRST);

				//透明オブジェクトか？
				if (GetFlagTransparentObject())
				{
					//透明オブジェクトの場合

					//モデルの初期化データ
					ModelInitData modelInitData;
					//シェーダーを設定
					//透明オブジェクトが描画できるシェーダーを設定
					modelInitData.m_fxFilePath = "Assets/shader/model2.fx";

					//シャドウマップの登録
					modelInitData.m_expandShaderResoruceView[0] = &g_graphicsEngine->GetShadowMap().GetShadowBlur();
					//ZPrepassで作成された深度テクスチャの登録
					modelInitData.m_expandShaderResoruceView[1] = &g_graphicsEngine->GetZPrepassDepthTexture();
					//スカイキューブの登録
					modelInitData.m_expandShaderResoruceView[2] = &g_graphicsEngine->GetSkyCubeTexture();

					//カリングモードを設定
					modelInitData.m_cullMode = D3D12_CULL_MODE_NONE;

					//モデルレンダラーを初期化
					//モデルのファイルパスの設定
					modelInitData.m_tkmFilePath = filePath;
					//モデルの初期化
					m_modelRender->Init(modelInitData);

					//ディファ―ドレンダリングを行わないようにする
					m_modelRender->SetIsDefferdRender(false);

				}
				else
				{
					//透明アイテムではない時

					//普通に初期化

					//モデルレンダラーを初期化
					m_modelRender->Init(filePath, D3D12_CULL_MODE_NONE);
				}


				m_modelRender->SetShadowReceiverFlag(false);
				//オブジェクトのタイプを設定する
				SetObjectType(objectType);
				m_modelRender->SetDrawOutLineFlag(true);

				return true;
			}

			//アップデート関数
			void CObstacleObject::PureVirtualUpdate()
			{
				//プレイヤーが見つかっていなかったら
				if (!m_player)
				{
					//プレイヤーを探す
					m_player = CLevelObjectManager::GetInstance()->GetPlayer();

					//それでも見つからなかったら何もずにreturn
					if (!m_player)
						return;
				}

				//モデルレンダラーの場所と回転と拡大
				m_modelRender->SetPosition(m_position);
				m_modelRender->SetRotation(m_rotation);
				m_modelRender->SetScale(m_scale);

				//オーバーライドしてほしい関数UpdateSub()
				UpdateSub();

				return;
			}

		}
	}
}