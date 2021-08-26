#pragma once
#include "LightData.h"
#include "LightManager.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief ライト
	*/
	namespace nsLight
	{

		/// <summary>
		/// スポットライトクラス
		/// </summary>
		class CSpotLight : public IGameObject
		{
		public:		//自動で呼ばれるメンバ関数
			CSpotLight();	//コンストラクタ
			~CSpotLight();	//デストラクタ

		public:		//メンバ関数

			/// <summary>
			/// スポットライトの座標を設定
			/// </summary>
			/// <param name="direction">ライトの座標</param>
			void SetPosition(const Vector3& direction);

			/// <summary>
			/// スポットライトの座標を取得
			/// </summary>
			/// <returns>ライトの座標</returns>
			const Vector3& GetPosition() const
			{
				return m_light->position;
			}

			/// <summary>
			/// スポットライトの色の設定
			/// </summary>
			/// <param name="color">ライトの色</param>
			void SetColor(const Vector4& color);

			/// <summary>
			/// ポイントライトの影響範囲を設定
			/// </summary>
			/// <param name="renge">影響範囲</param>
			void SetRange(const float range);

			/// <summary>
			/// スポットライトの射出方向の設定
			/// </summary>
			/// <param name="direction">射出方向</param>
			void SetDirection(const Vector3 direction)
			{
				if (m_result)
				{
					m_light->direction = direction;
				}
			}

			/// <summary>
			/// スポットライトの射出角度の設定
			/// </summary>
			/// <param name="angle">射出角度</param>
			void SetAngle(const float angle)
			{
				if (m_result)
					m_light->angle = angle;
			}

			/// <summary>
			/// 生データを取得
			/// </summary>
			/// <returns>生データの参照</returns>
			SSpotLight* GetRawData()
			{
				return m_light;
			};

			/// <summary>
			/// 生データを設定
			/// </summary>
			/// <param name="lig">生データ</param>
			void SetRawData(SSpotLight* lig)
			{
				m_light = lig;
			}

			/// <summary>
			/// 制御ナンバーを設定
			/// </summary>
			/// <param name="num">制御ナンバー</param>
			void SetControlNumber(const int num)
			{
				m_controlNumber = num;
			}

			/// <summary>
			/// 制御ナンバーを取得
			/// </summary>
			/// <returns>制御ナンバー</returns>
			const int GetControlNumver()const
			{
				return m_controlNumber;
			}

		private:	//データメンバ
			SSpotLight* m_light = nullptr;		//ポイントライトの構造体
			int m_controlNumber;				//制御ナンバー
			bool m_result;						//ライトマネージャーに登録してあるか？

		};

	}
}