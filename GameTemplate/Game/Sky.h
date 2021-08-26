#pragma once

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
		/**
		 * @brief 空クラスの定数データ
		*/
		namespace skyConstData
		{
			//空のモデルのファイルパス
			const char* const MODEL_FILEPATH_SKY = "Assets/modelData/preset/sky.tkm";
			//空のシェーダーファイルパス
			const char* const FX_FILEPATH_SKY = "Assets/shader/SkyCubeMap.fx";
			//空のテクスチャのデフォルトファイルパス
			const wchar_t* const TEXTURE_FILEPATH_SKY_DEFAULT = L"Assets/modelData/preset/sky.dds";
			//夕方の空のテクスチャのファイルパス
			const wchar_t* const TEXTURE_FILEPATH_SKY_EVENING = L"Assets/modelData/preset/sky.dds";
			//夜の空のテクスチャのファイルパス
			const wchar_t* const TEXTURE_FILEPATH_SKY_NIGHT = L"Assets/modelData/preset/sky.dds";
		}

		/**
		 * @brief 空クラス
		*/
		class CSky : public IGameObject
		{
		public:		//自動で呼ばれるメンバ関数
			CSky();		//コンストラクタ
			~CSky();	//デストラクタ
			bool Start()override final;		//スタート関数
			void Update()override final;	//アップデート関数
			void ForwardRender(RenderContext& rc)override final;	//描画関数

		public:		//メンバ関数

			/**
			 * @brief 座標を設定
			 * @param [in] pos 座標
			*/
			void SetPosition(const Vector3& pos)
			{
				m_position = pos;
				//値が変わったから、更新が必要
				m_isDirty = true;
			}

			/**
			 * @brief 拡大率を設定（Vector3）
			 * @param [in] scale 拡大率
			*/
			void SetScale(const Vector3& scale)
			{
				m_scale = scale;
				//値が変わったから、更新が必要
				m_isDirty = true;
			}

			/**
			 * @brief 拡大率を設定（float）
			 * @param [in] scale 拡大率
			*/
			void SetScale(const float scale)
			{
				//拡大率を初期化してから
				m_scale = g_VEC3_ONE;
				//拡大
				m_scale.Scale(scale);
				//値が変わったから、更新が必要
				m_isDirty = true;
			}

			/**
			 * @brief 自己発光カラーを設定
			 * @param [in] color 自己発光から―
			*/
			void SetModelEmissionColor(const Vector4 color)
			{
				m_model.SetModelEmissionColor(color);
				//値が変わったから、更新が
				m_isDirty = true;
			}

			/**
			 * @brief スカイキューブテクスチャのファイルパスを設定
			 * @attention Start関数より早く呼ばないと効果がない
			 * @param filepath スカイキューブテクスチャのファイルパス
			*/
			void SetSkyCubeTextureFilepath(const wchar_t* filepath)
			{
				m_skyCubeTextureFiltpath = m_skyCubeTextureFiltpath;
			}

		private:	//データメンバ
			Model m_model;							//モデルクラス
			Texture m_skyCubeTexture;				//スカイキューブテクスチャ
			Vector3 m_position = g_VEC3_ZERO;		//座標
			Vector3 m_scale = g_VEC3_ONE;			//拡大率
			Vector4 m_selfLuminous = g_VEC4_BLACK;	//自己発光色
			bool m_isDirty = false;					//ダーティフラグ
			const wchar_t* m_skyCubeTextureFiltpath = nullptr;	//スカイキューブテクスチャのファイルパス
		};

	}
}