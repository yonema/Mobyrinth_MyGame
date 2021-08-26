#include "stdafx.h"
#include "ROnail_hammer.h"

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
		 * @brief 反転オブジェクト
		*/
		namespace nsReversibleObject
		{

			//ReversibleObjectのモデルのファイルパスの定数データを使用可能にする
			using namespace ROsFilepathConstdata;
			//「釘」「金槌」の定数データを使用可能にする
			using namespace nail_hammerConstData;

			//スタート関数
			bool ROnail_hammer::StartSub()
			{
				//初期化用関数
				Init(MODEL_FILEPATH_NAIL, EN_RO_TYPE_NAIL,
					MODEL_FILEPATH_HAMMER, EN_RO_TYPE_BAR);

				//サウンドの初期化処理
				InitSound();

				//エフェクトの初期化処理
				InitEffect();

				return true;
			}

			/**
			 * @brief サウンドの初期化処理
			*/
			void ROnail_hammer::InitSound()
			{
				//壊れるときのサウンドの生成と初期化
				m_brokenSE = NewGO<nsSound::CSoundCue>(nsCommonData::PRIORITY_FIRST);
				m_brokenSE->Init(SOUNDE_FILEPATH_BREAK_BOX);
				m_brokenSE->SetVolume(SOUNDE_VOLUME_BREAK_BOX);

				return;
			}

			/**
			 * @brief エフェクトの初期化処理
			*/
			void ROnail_hammer::InitEffect()
			{
				//壊れるときのエフェクトの生成と初期化
				m_brokenEF = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
				m_brokenEF->Init(EFFECT_FILEPATH_BREAK_BOX);
				m_brokenEF->SetScale(EFFECT_SCALE_BREAK_BOX);

				return;
			}

			//デストラクタ
			ROnail_hammer::~ROnail_hammer()
			{
				DeleteGO(m_brokenSE);
				DeleteGO(m_brokenEF);
			}

			/// <summary>
			/// クエリしてほしいタイミングで呼ばれる関数
			/// </summary>
			void ROnail_hammer::QuerySub()
			{
				//自身が「釘」の時
				if (GetObjectType() == EN_RO_TYPE_NAIL)
				{

				}
				//自身が「金槌」の時
				else if (GetObjectType() == EN_RO_TYPE_BAR)
				{

					//障害オブジェクトの「箱」をクエリ
					QueryLOs<nsObstacleObject::OObox>(EN_OO_TYPE_BOX, [&](nsObstacleObject::OObox* box) -> bool
						{
							//自身が「金槌」で、「箱」と衝突した時の処理
							HammerHitBox(box);

							return true;
						}
					);

				}
			}

			/**
			 * @brief 自身が「金槌」で、「箱」と衝突した時の処理
			 * @param [in] box 箱のポインタ
			*/
			void ROnail_hammer::HammerHitBox(nsObstacleObject::OObox* box)
			{
				//自身と「箱」が衝突したら
				if (IsHitLevelObject(*this, *box))
				{
					//行動できたら
					if (m_actionFlag)
					{
						//「箱」を破棄
						DeleteGO(box);
						//自身のオブジェクトを破棄
						DeleteGO(this);
						//行動できないようにする
						m_actionFlag = false;

						//壊れるときのサウンドの再生
						m_brokenSE->Play(false);

						//アップベクトル
						Vector3 upVec = g_VEC3_UP;
						//アップベクトルを自身の回転で回す
						m_rotation.Apply(upVec);
						upVec.Scale(LENGTH_UP_POSITION_EFFECT);
						//エフェクトの座標を回転を調節する
						m_brokenEF->SetPosition(m_position + upVec);		//座標を渡す
						m_brokenEF->SetRotation(m_rotation);
						//エフェクトの再生
						m_brokenEF->Play();

					}
				}

				return;
			}
		}
	}
}