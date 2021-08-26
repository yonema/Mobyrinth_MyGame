#include "stdafx.h"
#include "LevelObjectBase.h"
#include "Mobius.h"

//デバック用
#ifdef MY_DEBUG
//後で消す
int ILevelObjectBase::objectNumber = 0;
#endif

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

		namespace
		{
			//OBBのデフォルトの大きさ
			const Vector3 OBB_SIZE_DEFAULT = { 100.0f,100.0f,100.0f };

			//タイマーのフォントのデフォルトのテキスト
			const wchar_t* const FONT_TEXT_TIMER_DEFAULT = L"10";

			//スイッチONの時のエフェクトのファイルパス
			const char16_t* const EFFECT_FILEPATH_SWITCH_ON = u"Assets/effect2/activation.efk";
			//スイッチONの時のエフェクトの拡大率
			const float EFFECT_SCALE_SWITCH_ON = 70.0f;

			//スイッチOFFの時のエフェクトのファイルパス
			const char16_t* const EFFECT_FILEPATH_SWITCH_OFF = u"Assets/effect2/activation.efk";
			//スイッチOFFの時のエフェクトの拡大率
			const float EFFECT_SCALE_SWITCH_OFF = 70.0f;
		}


		//スタート関数
		bool ILevelObjectBase::Start()
		{
			//近くのウェイポイントを探して、イイ感じに回転する
			CheckWayPoint();

			//プレイヤーの参照を保持する
			m_player = CLevelObjectManager::GetInstance()->GetPlayer();

			//LevelObjectManagerにオブジェクトが追加されたことを報告
			CLevelObjectManager::GetInstance()->AddObject(this);

			//OBBを初期化する
			InitOBB();


#ifdef MY_DEBUG
			//デバック用
			//後で消す。
			m_objectNumber = objectNumber++;

			Vector3* vertPos = m_obb.GetBoxVertex();
			for (int i = 0; i < m_dbgOBBNum; i++)
			{
				m_dbgOBBVert[i] = NewGO<CModelRender>(0);
				m_dbgOBBVert[i]->Init("Assets/modelData/dbgBox.tkm");
				m_dbgOBBVert[i]->SetPosition(vertPos[i]);
			}

			//デバック用ここまで
#endif

	//オーバーライドしてほしい関数PureVirtualStart()
			return PureVirtualStart();

		}

		//デストラクタ
		ILevelObjectBase::~ILevelObjectBase()
		{
			//スイッチをオンにしたときのエフェクトの破棄
			if (m_swichonEffect)
				DeleteGO(m_swichonEffect);

			//スイッチをオフにしたときのエフェクトの破棄
			if (m_swichoffEffect)
				DeleteGO(m_swichoffEffect);

			//フォントのタイマーの破棄
			if (m_timerFR)
				DeleteGO(m_timerFR);

#ifdef MY_DEBUG
			//デバック用
			//後で消す
			for (int i = 0; i < m_dbgOBBNum; i++)
			{
				DeleteGO(m_dbgOBBVert[i]);
			}
			//デバック用ここまで
#endif

	//LevelObjectManagerにオブジェクトが破棄されたことを伝える
			CLevelObjectManager::GetInstance()->RemoveObject(this);

			return;
		}


		//アップデート関数
		void ILevelObjectBase::Update()
		{
			//オーバーライドされる関数
			//派生クラスのアップデート関数から先に呼ばれる
			PureVirtualUpdate();

			//OBBの場所と回転を設定する
			m_obb.SetPosition(m_position);
			m_obb.SetRotation(m_rotation);

#ifdef MY_DEBUG
			//デバック用
			//後で消す
			Vector3* vertPos = m_obb.GetBoxVertex();
			for (int i = 0; i < m_dbgOBBNum; i++)
			{
				m_dbgOBBVert[i]->SetPosition(vertPos[i]);
				m_dbgOBBVert[i]->SetRotation(m_rotation);
			}
			//デバック用ここまで
#endif

			return;
		}


		/// <summary>
		/// OBBの初期化関数
		/// </summary>
		void ILevelObjectBase::InitOBB()
		{
			//OBBの初期化データ
			nsOBB::SInitOBBData initData;
			initData.position = m_position;
			initData.width = OBB_SIZE_DEFAULT.x;
			initData.height = OBB_SIZE_DEFAULT.y;
			initData.length = OBB_SIZE_DEFAULT.z;
			initData.pivot = nsOBB::OBBConstData::OBB_PIVOT_DEFAULT;
			initData.rotation = m_rotation;

			//OBBの初期化
			m_obb.Init(initData);

			return;
		}


		/**
		 * @brief 自身の現在の座標から、ウェイポイントの番号を決定し、回転と座標をイイ感じに合わせる
		 * @param [in] checkRotaton 回転チェックを行うか？
		 * @param [in] checkPosition 座標チェックを行うか？
		*/
		void ILevelObjectBase::CheckWayPoint(const bool checkRotaton, const bool checkPosition)
		{
			//LevelObjectManagerからウェイポイントの情報を持ってくる
			//ウェイポイントの「場所」を持ってくる
			std::vector<Vector3>* wayPointPosVec
				= CLevelObjectManager::GetInstance()->GetWayPointPos();
			//ウェイポイントの「回転」を持ってくる
			std::vector<Quaternion>* wayPointRotVec
				= CLevelObjectManager::GetInstance()->GetWayPointRot();


			//一番近いウェイポイントを調べる

			//一番近い距離を調べるようの変数
			//FLT_MAX（float型の大きい数）を最初に入れておく
			float dist = FLT_MAX;
			//一番近いウェイポイントのベクターのインデックスを保持しておく用の変数
			int index = 0;
			//ウェイポイントのベクターのサイズを保持
			int vecSize = (*wayPointPosVec).size();

			//一番近いウェイポイントを調べる
			for (int i = 0; i < vecSize; i++)
			{
				//ウェイポイントと自身の間のベクトルを計算する
				Vector3 lenVec = (*wayPointPosVec)[i] - m_position;
				//ウェイポイントと自信の間の長さを計算する
				float distTmp = lenVec.Length();

				//distより近い距離なら
				if (distTmp < dist)
				{
					//一番近いウェイポイントのベクターのインデックスに
					//現在の繰り返し回数を保持させて、
					index = i;
					//distにその距離を保持させる
					dist = distTmp;
				}
			}

			//自身の左のウェイポイントのベクターのインデックス
			int lpIndex;
			//自身の右のウェイポイントのベクターのインデックス
			int rpIndex;

			//一番近いウェイポイントの左右のインデックスを調べる
			{
				//一番近いウェイポイントの右のインデックス
				int right = index - 1;
				//一番近いウェイポイントの左のインデックス
				int left = index + 1;

				//0より小さかったら
				if (right < 0)
				{
					//一周させる
					right = vecSize;
					//インデックスの最大値はベクターのサイズより1小さいから-1する
					right -= 1;
				}
				//ベクターのサイズ以上だったら
				//インデックスの最大値はベクターのサイズより1小さいから
				//　>=　で比較する
				if (left >= vecSize)
				{
					//一周させる
					left = 0;
				}

				//一番近いウェイポイントの左のウェイポイントと、自身との間のベクトル
				Vector3 leftLen = (*wayPointPosVec)[left] - m_position;
				//一番近いウェイポイントの右のウェイポイントと、自身との間のベクトル
				Vector3 rightLen = (*wayPointPosVec)[right] - m_position;

				//右と左の距離がどちらが近いか比較する
				if (leftLen.Length() <= rightLen.Length())
				{
					//左の方が近いなら
					//左のインデックスを、一番近いウェイポイントの左のウェイポイント
					//に決定する
					lpIndex = left;
					//右のインデックスを、一番近いウェイポイント
					//に決定する
					rpIndex = index;
				}
				else
				{
					//右の方が近いなら
					//みぎのインデックスを、一番近いウェイポイントの右のウェイポイント
					//に決定する
					rpIndex = right;
					//左のウェイポイントを、一番近いウェイポイント
					//に決定する
					lpIndex = index;
				}
			}

			//左右のウェイポイントの番号を確定する
			m_lpIndex = lpIndex;
			m_rpIndex = rpIndex;

			//ロックされているか？
			if (!m_isLock)
			{
				//ロックされていない

				//回転チェックをするか？
				if (checkRotaton)
				{
					//現在の座標に合わせた回転にする
					CheckRotation();
				}
				//座標チェックをするか？
				if (checkPosition)
				{
					//ウェイポイントにそろえた座標にする
					CheckPosition();
				}
			}

			return;
		}

		/**
		 * @brief 現在の座標に合わせた回転にする
		*/
		void ILevelObjectBase::CheckRotation()
		{
			//LevelObjectManagerからウェイポイントの情報を持ってくる
			//ウェイポイントの「場所」を持ってくる
			std::vector<Vector3>* wayPointPosVec
				= CLevelObjectManager::GetInstance()->GetWayPointPos();
			//ウェイポイントの「回転」を持ってくる
			std::vector<Quaternion>* wayPointRotVec
				= CLevelObjectManager::GetInstance()->GetWayPointRot();

			//左のウェイポイントから右のウェイポイントへのベクトル
			Vector3 lpToRpVec = (*wayPointPosVec)[m_rpIndex] - (*wayPointPosVec)[m_lpIndex];

			//左のウェイポイントから自身へのベクトル
			Vector3 lpToPosVec = m_position - (*wayPointPosVec)[m_lpIndex];

			//正規化した、左のウェイポイントから右のウェイポイントへのベクトル
			Vector3 normalLpToRpVec = lpToRpVec;
			normalLpToRpVec.Normalize();

			//左のウェイポイントから自身へのベクトルを、ウェイポイント上に射影した距離
			float projLpToPosLen = Dot(normalLpToRpVec, lpToPosVec);

			//自身が左右のウェイポイントの間のどれくらいの位置にいるかで
			//補完率を計算する
			float ComplementRate = projLpToPosLen / lpToRpVec.Length();

			//球面線形補完
			//を使って回転させる
			m_rotation.Slerp(ComplementRate, (*wayPointRotVec)[m_lpIndex], (*wayPointRotVec)[m_rpIndex]);

			return;
		}


		/**
		 * @brief 座標をきれいにメビウスの輪の上に配置する
		*/
		void ILevelObjectBase::CheckPosition()
		{
			//LevelObjectManagerからウェイポイントの情報を持ってくる
			//ウェイポイントの「場所」を持ってくる
			const std::vector<Vector3>* wayPointPosVec
				= CLevelObjectManager::GetInstance()->GetWayPointPos();

			//左のウェイポイントから右のウェイポイントへのベクトル
			Vector3 normalLpToRpVec = (*wayPointPosVec)[m_rpIndex] - (*wayPointPosVec)[m_lpIndex];
			normalLpToRpVec.Normalize();

			//左のウェイポイントから自身へのベクトル
			const Vector3 lpToPosVec = m_position - (*wayPointPosVec)[m_lpIndex];

			//左のウェイポイントから自身へのベクトルを、ウェイポイント上に射影した距離
			const float projLpToPosLen = Dot(normalLpToRpVec, lpToPosVec);

			//ウェイポイント上の座標を出す
			Vector3 nextPos = normalLpToRpVec;	//次の座標
			nextPos.Scale(projLpToPosLen);
			nextPos += (*wayPointPosVec)[m_lpIndex];

			//メビウスの輪にピッタリ合わせる
			Vector3 yVec = g_VEC3_UP;
			m_rotation.Apply(yVec);
			Vector3 IntersectLineVec = yVec;
			IntersectLineVec.Scale(300.0f);
			CLevelObjectManager::GetInstance()->GetMobius()->GetIntersectPosWithMobius(
				nextPos + IntersectLineVec, nextPos - IntersectLineVec, &nextPos
			);
			//メビウスの輪から上にあげる
			yVec.Scale(m_yPosLen);
			nextPos += yVec;

			//ウェイポイントから奥によらせる
			Vector3 zVec = g_VEC3_BACK;
			m_rotation.Apply(zVec);
			zVec.Scale(m_zPosLen);
			nextPos += zVec;

			//座標を確定する
			m_position = nextPos;

			return;
		}

		/// <summary>
		/// 自身が表側にあるか裏側にあるかを調べる関数
		/// </summary>
		/// <param name="reversibleObject">反転オブジェクトか？</param>
		void ILevelObjectBase::CheckFrontOrBackSide(const bool reversibleObject)
		{
			//表側か裏側か
			int nextFrontOrBackSide = CLevelObjectManager::GetInstance()->CheckFrontOrBackSide(m_lpIndex);

			//反転オブジェクトか？
			if (reversibleObject)
			{
				//反転オブジェクト
				//ステージ上の表裏のアイテム数を変更させる

				//初期値ではなかったら
				if (m_frontOrBackSide != nsCommonData::EB_NONE_SIDE)
				{
					//前の場所の反転オブジェクトの数を減算する
					CLevelObjectManager::GetInstance()->RemoveReversibleObjectNum(m_frontOrBackSide);
				}

				//次の場所の反転オブジェクトの数を加算する
				CLevelObjectManager::GetInstance()->AddReversibleObjectNum(nextFrontOrBackSide);
			}

			//現在の表側か裏側かを確定する
			m_frontOrBackSide = nextFrontOrBackSide;
		}

		/// <summary>
		/// 自身とプレイヤーの当たり判定
		/// </summary>
		/// <returns>trueが戻ってきたら当たっている</returns>
		bool ILevelObjectBase::IsHitPlayer()
		{
			//プレイヤーが見つかっていなかったら何もせずにreturn
			if (!m_player)
				return false;

			//OBB同士の当たり判定をして、
			//その結果を戻す
			return CollisionOBBs(m_player->GetOBB(), m_obb);
		}


		/// <summary>
		/// 透明オブジェクトに使用するデータを初期化する。
		/// </summary>
		void ILevelObjectBase::SetTransparentObject()
		{
			//透明オブジェクト判定に使用するフラグをtrueにする。
			m_transparentObjectFlag = true;

			//オブジェクトの重なっている判定を行わないようにする。
			m_isHitFlag = false;

			//タイマーのフォントレンダラーの生成と初期化
			m_timerFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::PRIORITY_SECOND);
			m_timerFR->Init(FONT_TEXT_TIMER_DEFAULT, g_VEC2_ZERO);
			//非表示にする
			m_timerFR->Deactivate();

			//スイッチONの時のエフェクトの生成と初期化
			m_swichonEffect = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
			m_swichonEffect->Init(EFFECT_FILEPATH_SWITCH_ON);
			//小さいので大きくしておく
			m_swichonEffect->SetScale(EFFECT_SCALE_SWITCH_ON);

			//スイッチOFFの時のエフェクトの生成と初期化
			m_swichoffEffect = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
			m_swichoffEffect->Init(EFFECT_FILEPATH_SWITCH_OFF);
			//小さいので大きくしておく
			m_swichoffEffect->SetScale(EFFECT_SCALE_SWITCH_OFF);

			//開始時に透明にするときにエフェクトを呼ばないようにするために、無効化する
			m_swichoffEffect->Deactivate();

			return;
		}


		/// <summary>
		/// 透明スイッチが押されたときに使用される関数
		/// </summary>
		void ILevelObjectBase::TransparentSwitchOn()
		{
			//スイッチONの時のエフェクトの座標と回転を設定
			m_swichonEffect->SetPosition(m_position);
			m_swichonEffect->SetRotation(m_rotation);
			//エフェクトを再生
			m_swichonEffect->Play();

			//オブジェクトの衝突判定を行うようにする。
			m_isHitFlag = true;

			//オブジェクトの当たり判定を有効にする。
			m_obb.SetExceptionFlag(false);


			//タイマーのフォントを表示する
			m_timerFR->Activate();


			//反転オブジェクト用のスイッチONの時の処理
			ReversibleSwitchOn();
		}


		/// <summary>
		/// 透明スイッチの効果が消えたときに使用される関数
		/// </summary>
		void ILevelObjectBase::TransparentSwitchOff()
		{
			//開始時にオブジェクトを透明にするときに、
			//エフェクトが再生されないようにする

			//スイッチOFFのエフェクトが有効か？
			if (m_swichoffEffect->IsActive())
			{
				//有効
				//開始時以外で呼ばれる

				//エフェクトの座標と回転を設定する
				m_swichoffEffect->SetPosition(m_position);
				m_swichoffEffect->SetRotation(m_rotation);
				//エフェクトを再生する
				m_swichoffEffect->Play();
			}
			else
			{
				//有効じゃない
				//開始時に呼ばれる

				//有効にする
				m_swichoffEffect->Activate();
			}

			//オブジェクトの衝突判定を行わないようにする。
			m_isHitFlag = false;

			//オブジェクトの当たり判定を無効にする。
			m_obb.SetExceptionFlag(true);

			//タイマーのフォントを非表示にする
			m_timerFR->Deactivate();


			//反転オブジェクト用のスイッチをOFFにする処理
			ReversibleSwitchOff();

			return;
		}


		/// <summary>
		/// OBB同士の当たり判定
		/// </summary>
		/// <param name="lhs">レベルオブジェクト1</param>
		/// <param name="rhs">レベルオブジェクト2</param>
		/// <returns>trueが戻ってきたら当たっている</returns>
		bool IsHitLevelObject
		(const ILevelObjectBase& lhs, const ILevelObjectBase& rhs)
		{
			//オブジェクトの衝突判定を行わない状態の場合
			if (lhs.GetIsHitFlag() == false || rhs.GetIsHitFlag() == false) {
				//衝突していない判定する。
				return false;
			}
			if (lhs.GetFrontOrBackSide() != rhs.GetFrontOrBackSide())
				return false;

			//OBB同士の当たり判定をして、
			//その結果を戻す
			return CollisionOBBs(lhs.GetOBB(), rhs.GetOBB());
		}

	}
}