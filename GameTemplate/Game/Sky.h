#pragma once
#include "SkyConstData.h"

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

			/**
			 * @brief 空の種類を設定
			 * @attention Start関数より早く呼ばないと効果がない
			 * @param skyType 空の種類
			*/
			void SetSkyType(const skyConstData::EN_SKY_TYPE skyType)
			{
				m_skyType = skyType;
			}

		private:	//データメンバ
			Model m_model;							//モデルクラス
			Texture m_skyCubeTexture;				//スカイキューブテクスチャ
			Vector3 m_position = g_VEC3_ZERO;		//座標
			Vector3 m_scale = g_VEC3_ONE;			//拡大率
			Vector4 m_selfLuminous = g_VEC4_BLACK;	//自己発光色
			bool m_isDirty = false;					//ダーティフラグ
			skyConstData::EN_SKY_TYPE m_skyType = skyConstData::EN_SKY_DEFAULT;	//空の種類
		};

	}
}