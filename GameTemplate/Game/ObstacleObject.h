#pragma once
#include "LevelObjectBase.h"
#include "ModelRender.h"
#include "OBBWorld.h"
#include "OOsFilepathAndObbSizeConstData.h"

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


			/// <summary>
			/// オブステークルオブジェクト（障害オブジェクト）（読み方あってる？）
			/// 障害オブジェクトを作るときはこのクラスを継承してね
			/// </summary>
			class CObstacleObject : public ILevelObjectBase
			{
			public:		//自動で呼ばれるメンバ関数
				bool PureVirtualStart()override final;		//スタート関数
				virtual ~CObstacleObject();					//デストラクタ
				void PureVirtualUpdate()override final;		//アップデート関数

			public:		//オーバーライドしたメンバ関数

				/// <summary>
				/// モデルの参照を得る
				/// </summary>
				/// <param name="frontOrBack">裏表がないので関係ない</param>
				/// <returns>モデルの参照</returns>
				nsGraphic::nsModel::CModelRender* GetModelRender(const int frontOrBack = 0) override final
				{
					return m_modelRender;
				}


			public:		//オーバーライドしてほしいメンバ関数
				virtual bool StartSub() { return true; };	//スタート関数
				virtual void UpdateSub() {};				//アップデート関数

			protected:	//protectedなメンバ関数

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
				bool Init(const char* filePath, int objectType);

				/// <summary>
				/// 自己発光色を設定する
				/// </summary>
				/// <param name="color">自己発光色</param>
				void SetModelEmissionColor(const Vector4& color)
				{
					m_modelRender->SetModelEmissionColor(color);
				}


			private:	//データメンバ
				nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//モデルレンダラー
			};

		}
	}

}