#include "stdafx.h"
#include "Sky.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief スカイ
	*/
	namespace nsSky
	{

		//空クラスの定数データを使用可能にする
		using namespace skyConstData;

		//コンストラクタ
		CSky::CSky()
		{

		}

		//デストラクタ
		CSky::~CSky()
		{

		}

		//スタート関数
		bool CSky::Start()
		{
			//モデルの初期化データ
			ModelInitData initData;
			//tkmファイルのファイルパスを指定する。
			initData.m_tkmFilePath = MODEL_FILEPATH_SKY;
			//シェーダーファイルのファイルパスを指定する。
			initData.m_fxFilePath = FX_FILEPATH_SKY;

			SSkyIBLData skyIBLData = GetIBLLuminanceForSky(m_skyType);

			//スカイキューブテクスチャを初期化
			m_skyCubeTexture.InitFromDDSFile(skyIBLData.m_skyTextureFilepath);

			//スカイキューブテクスチャをSRVに登録
			initData.m_expandShaderResoruceView[0] = &m_skyCubeTexture;

			//モデルを初期化する
			m_model.Init(initData);
			//モデルを更新する
			m_model.UpdateWorldMatrix(m_position, g_QUAT_IDENTITY, m_scale);

			SetIsDefferdRender(false);

			g_graphicsEngine->ReInitSkyCubeTexture(skyIBLData);

			return true;
		}

		//アップデート関数
		void CSky::Update()
		{
			//ダーティフラグが立っていたら
			if (m_isDirty)
			{
				//モデルを更新
				m_model.UpdateWorldMatrix(m_position, g_QUAT_IDENTITY, m_scale);

				//ダーティフラグを折る
				m_isDirty = false;
			}

			return;
		}

		//描画関数
		void CSky::ForwardRender(RenderContext& rc)
		{
			//モデルを描画する
			m_model.Draw(rc);
		}
	}
}