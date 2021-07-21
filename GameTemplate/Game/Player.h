#pragma once
#include "PlayerConstData.h"
#include "LevelObjectManager.h"
#include "LightData.h"
#include "DirectionLight.h"
#include "ModelRender.h"
#include "Mobius.h"
#include "OBB.h"
#include "MyCharacterController.h"
#include "SoundCue.h"
#include "GameTime.h"
#include "SoundCue.h"
#include "effect/Effect.h"



class CGameCamera;
class CReversibleObject;



/// <summary>
/// プレイヤーが操作するキャラクターのクラス
/// </summary>
class Player : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start() override final;	//スタート関数
	~Player();						//デストラクタ
	void Update() override final;	//アップデート関数

public:		//メンバ関数

	/**
	 * @brief プレイヤーの座標を設定
	 * @param [in] pos 
	*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/**
	 * @brief プレイヤーの座標を取得
	 * @return 座標
	*/
	const Vector3& GetPosition()const
	{
		return m_position;
	}

	/**
	 * @brief プレイヤーの回転を設定
	 * @param [in] rot 回転
	*/
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/**
	 * @brief プレイヤーの回転を得る
	 * @return 回転
	*/
	const Quaternion& GetRotation() const
	{
		return m_rotation;
	}

	/**
	 * @brief プレイヤーの拡大を設定する
	 * @param scale 
	*/
	void SetScale(const Vector3& scale)
	{
		m_modelRender->SetScale(scale);
	}


	/**
	 * @brief ウェイポイントの「座標」を取得
	 * @param posVec 座標のベクター
	*/
	void SetWayPointPos(std::vector<Vector3>*const posVec);

	/**
	 * @brief ウェイポイントの「回転」を取得
	 * @param rotVec 回転のベクター
	*/
	void SetWayPointRot(std::vector<Quaternion>* rotVec);


	/**
	 * @brief 補完済みの最終的なウェイポイントの回転を取得
	 * @return 補完済みの最終的なウェイポイントの回転
	*/
	const Quaternion& GetFinalWPRot()const
	{
		return m_finalWPRot;
	}

	/**
	 * @brief UFOにキャプチャされ中の回転を得る
	 * @return UFOにキャプチャされ中の回転
	*/
	const Quaternion& GetCapturedRotation() const
	{
		return m_capturedRotation;
	}
	
	/**
	 * @brief UFOにキャプチャされ中の回転を設定する
	 * @param [in] qRot UFOにキャプチャされ中の回転
	*/
	void SetCapturedRotation(const Quaternion& qRot)
	{
		m_capturedRotation = qRot;
	}

	/**
	 * @brief UFOにキャプチャされ中の座標を得る
	 * @return UFOにキャプチャされ中の座標
	*/
	const Vector3& GetCapturedPosition() const
	{
		return m_capturedPosition;
	}

	/**
	 * @brief UFOにキャプチャされ中の座標を設定する
	 * @param [in] position UFOにキャプチャされ中の座標
	*/
	void SetCapturedPosition(const Vector3 position)
	{
		m_capturedPosition = position;
	}

	/**
	 * @brief スタン中か？を得る
	 * @return スタン中か？
	*/
	bool GetStunFlag()const
	{
		return m_stunFlag;
	}

	/**
	 * @brief プレイヤーがオブジェクトを持っているかどうかを設定する。
	 * @param [in] holdFlag オブジェクトを持っているか？
	 * @param [in] reversibeObject 持たれる反転オブジェクト
	*/
	void SetHoldObject(const bool holdFlag, CReversibleObject* reversibeObject = nullptr)
	{
		m_holdObject = holdFlag;
		m_lifting = holdFlag;
		if (m_holdObject)
			m_reversibleObject = reversibeObject;
		else
			m_reversibleObject = nullptr;
	}

	/**
	 * @brief プレイヤーがオブジェクトを持っているかどうかを調べる
	 * @return オブジェクトを持っているか？
	 * @retval true 持っている
	 * @retval false 持っていない
	*/
	bool GetHoldObject()const
	{
		return m_holdObject;
	}

	/**
	 * @brief 持っている反転オブジェクトのポインタを戻す
	 * @return 持っている反転オブジェクトポインタ
	*/
	const CReversibleObject* GetReversibleObject() const
	{
		return m_reversibleObject;
	}

	/**
	 * @brief 持っている反転オブジェクトの参照を得る
	 * @return 持っている反転オブジェクトの参照
	*/
	CReversibleObject* GetReversibleObject()
	{
		return m_reversibleObject;
	}

	/**
	 * @brief プレイヤーを操作できるか？
	 * @param [in] operationFlag 操作できるか？
	*/
	void SetOperationFlag(const bool operationFlag)
	{
		//歩くSEが流れていたら
		if (m_walkSE->IsPlaying()) {
			//SEを止める
			m_walkSE->Stop();
		}
		//走るSEが流れていたら
		if (m_runSE->IsPlaying()) {
			//SEを止める
			m_runSE->Stop();
		}
		//プレイヤーを操作できるか？を設定
		m_operationFlag = operationFlag;
	}

	/**
	 * @brief タイトルか？を設定する。
	 * @param [in] titleMove 
	*/
	void SetTitleMove(const bool titleMove)
	{
		m_titleMove = titleMove;
		//タイトルか？
		if (!m_titleMove)
		{
			//タイトルではない
			//ゲーム開始時の座標を設定
			m_position = playerConstData::POSITION_GAME_START;
		}
	}


	/**
	 * @brief プレイヤーの現在のUpベクトルを得る
	 * @return Upベクトル
	*/
	const Vector3& GetUpVec()const
	{
		return m_upVec;
	}

	/**
	 * @brief 現在のプレイヤーの左側のウェイポイントの番号を得る
	 * @return 左側のウェイポイントの番号
	*/
	int GetLeftPointIndex()const
	{
		return m_lpIndex;
	}

	/**
	 * @brief プレイヤーの左側のウェイポイントを設定する
	 * @param [in] lpIndex 左側のウェイポイント
	*/
	void SetLeftPointIndex(const int lpIndex)
	{
		m_lpIndex = lpIndex;
	}

	/**
	 * @brief 現在のプレイヤーの右側のウェイポイントの番号を得る
	 * @return 右側のウェイポイント
	*/
	int GetRightPointIndex()const
	{
		return m_rpIndex;
	}

	/**
	 * @brief プレイヤーの右側のウェイポイントを設定する
	 * @param [in] rpIndex 右側のウェイポイント
	*/
	void SetRightPointIndex(const int rpIndex)
	{
		m_rpIndex = rpIndex;
	}

	/**
	 * @brief 引数で渡された番号のウェイポイントの座標を戻す
	 * @param [in] index ウェイポイントの番号
	 * @return ウェイポイントの座標
	*/
	const Vector3& GerWayPointPos(const int index)
	{
		return (*m_wayPointPos)[index];
	}

	/**
	 * @brief キャラクターが左と右どちらを向いているか調べる
	 * @return 右か左か
	 * @retval 0 enLeft
	 * @retval 1 enRight
	*/
	int GetEnLeftOrRight()const
	{
		return m_leftOrRight;
	}

	/**
	 * @brief 自身がどのウェイポイントにいるか表すステートを戻す関数
	 * @return ウェイポイントの番号
	*/
	int GetWayPointState()const
	{
		return m_wayPointState;
	}

	/**
	 * @brief 自身がどのウェイポイントにいるか表すステートを設定
	 * @param [in] wayPointState ウェイポイントのステート
	*/
	void SetWayPointState(const int wayPointState)
	{
		m_wayPointState = wayPointState;
	}

	/**
	 * @brief UFOに捕まっているか？を設定
	 * @param [in] captured UFOに捕まっているか？
	*/
	void SetCapturedUFOFlag(const bool captured)
	{
		m_capturedUFOFlag = captured;
	}

	/**
	 * @brief UFOに捕まっているか？を取得
	 * @return UFOに捕まっているか？
	*/
	bool GetCapturedUFOFlag()const
	{
		return m_capturedUFOFlag;
	}

	/**
	 * @brief 落ちているか？を設定
	 * @param [in] fallFlag 落ちているか？
	*/
	void SetFallFlag(const bool fallFlag)
	{
		m_fallFlag = fallFlag;
	}

	/**
	 * @brief 落ちているか？を取得
	 * @return 落ちているか？
	*/
	bool GetFallFalg() const
	{
		return m_fallFlag;
	}

	/**
	 * @brief OBBの参照を戻す
	 * @return OBBの参照
	*/
	COBB& GetOBB()
	{
		return m_myCharaCon.GetOBB();
	}

	/**
	 * @brief 投げているか？を設定
	 * @param [in] throwing 投げているか？
	*/
	void SetThrowing(const bool throwing)
	{
		m_throwing = throwing;
	}

	/**
	 * @brief 持ち上げ中か？を設定
	 * @param [in] lifting 持ち上げ中か？
	*/
	void SetLifting(const bool lifting) 
	{
		m_lifting = lifting;
	}

	/**
	 * @brief ゴールしたか？を設定
	 * @param [in] isGoal ゴールしたか？
	*/
	void SetGoal(const bool isGoal)
	{
		m_isGoal = isGoal;
	}


private://privateなメンバ関数

	/**
	 * @brief タイトル中の動き
	*/
	void TitleMove();

	/**
	 * @brief ゲーム中の動き
	*/
	void GameMove();

	/**
	 * @brief アニメーションの初期化
	*/
	void InitAnimation();

	/**
	 * @brief モデルの初期化
	*/
	void InitModel();

	/**
	 * @brief キャラクターコントローラーの初期化
	*/
	void InitMyCharacterController();

	/**
	 * @brief サウンドの初期化
	*/
	void InitSound();

	/**
	 * @brief フラグを調べる
	 * @return フラグが立っているか？
	 * @retval true 立っている
	 * @retval false 立っていない
	*/
	bool CheckFlag();

	/**
	 * @brief 入力情報を調べる
	*/
	void CheckInput();

	/**
	 * @brief 右向きか左向きかを調べる
	*/
	void CheckLeftOrRight();

	/**
	 * @brief ウェイポイントの更新処理
	*/
	void CheckWayPoint();

	/**
	 * @brief ウェイポイントステートから
	 * 左側のウェイポイントと右側のウェイポイントを計算する
	*/
	void CalcLeftAndRightWayPoint();

	/**
	 * @brief 移動処理
	*/
	void Move();

	/**
	 * @brief スタン中の移動処理
	*/
	void StunMove();

	/**
	 * @brief ステージに乗る
	*/
	void GetOnStage();

	/**
	 * @brief スタン中のステージに乗る処理
	*/
	void StunGetOnStage();

	/**
	 * @brief モデルの回転処理
	*/
	void Rotation();

	/**
	 * @brief 衝突したOBBのタグを調べる
	*/
	void CheckHitOBBTag();

	/**
	 * @brief 炎と衝突した時の処理
	*/
	void HitFlame();

	/**
	 * @brief 壁と衝突した時の処理
	*/
	void HitWall();

	/**
	 * @brief ライトのデータを更新する
	*/
	void UpdateLightData();

	/**
	 * @brief プレイヤーを照らす影を生成するライトを更新する
	*/
	void SetShadowParam();

	/**
	 * @brief ディレクションライトを更新する
	*/
	void SetDirectionLight();

	/**
	 * @brief UFOに捕まっている時の処理
	*/
	void CapturedUFO();

	/**
	 * @brief スタートの落ちるときの処理
	*/
	void Fall();

	/**
	 * @brief ゴールしている時の処理
	*/
	void Goal();

	/**
	 * @brief アニメーションを制御する
	*/
	void AnimationController();

	/**
	 * @brief アニメーションの遷移をリセットする
	*/
	void AnimationReset();

	/**
	 * @brief SEの制御
	*/
	void SoundController();


private:	//データメンバ

	/*
	* モデル、アニメーションデータ
	*/
	CModelRender* m_modelRender = nullptr;	//モデルレンダラー
	//アニメーションクリップ。
	AnimationClip m_animationClips[playerConstData::EN_ANIM_CLIP_NUM];
	CModelRender* m_goalPlayerMR = nullptr;	//ゴール時のプレイヤーのモデル
	//ゴール時のアニメーションクリップ。
	AnimationClip m_goalAnimationClips[playerConstData::EN_GOAL_ANIM_CLIP_NUM];

	/*
	* キャラクターのトランスフォーム
	*/
	Vector3 m_moveSpeed = g_VEC3_ZERO;		//キャラクターの移動スピード
	Vector3 m_position = g_VEC3_ZERO;		//キャラクターの座標
	Quaternion m_rotation = g_QUAT_IDENTITY;	//キャラクターの回転

	/*
	* 移動関連
	*/
	CMyCharacterController m_myCharaCon;	//myキャラクターコントローラー
	float m_padLStickXF = 0.0f;				//パッドの左スティックのX軸の入力情報
	Vector3 m_onWayPosition = g_VEC3_ZERO;	//道の上の座標
	Vector3 m_upVec = g_VEC3_UP;				//プレイヤーのUpベクトル

	/*
	* 状態
	*/
	//アニメーションの状態
	playerConstData::EN_ANIMATION_CLIP m_animState = playerConstData::EN_ANIM_CLIP_IDLE;
	EN_LEFT_OR_RIGHT m_leftOrRight = EN_LEFT;			//キャラクターの左右の向き
	CReversibleObject* m_reversibleObject = nullptr;	//持っている反転オブジェクトのポインタ

	/*
	* フラグ
	*/
	bool m_isDush = false;						//走り状態か？
	bool m_throwing = false;					//投げ中か？
	bool m_lifting = false;						//持ち上げ中か？
	bool m_isGoal = false;						//ゴールしたか？
	bool m_holdObject = false;					//オブジェクトを持っているか？
	bool m_operationFlag = false;				//操作できるか？
	bool m_titleMove = false;					//タイトルか？
	bool m_stunFlag = false;					//スタン状態（吹っ飛び状態）か？
	bool m_capturedUFOFlag = false;				//UFOにキャプチャされているか？
	bool m_fallFlag = false;					//落下中か？


	/*
	* スタン関連
	*/
	bool m_stunLeftOrRight = EN_LEFT;				//スタン中の吹っ飛ぶ方向
	float m_stunTimer = 0.0f;						//スタンのタイマー
	Vector3 m_stunMoveSpeedHorizontal = g_VEC3_ZERO;	//スタン中の横方向の移動速度
	Vector3 m_stunMoveSpeedVertical = g_VEC3_ZERO;	//スタン中の縦方向の移動速度
	bool m_stunMoveFlag = true;						//スタン中に吹っ飛び中か？
	float m_blinkTimer = 0.0f;						//スタン状態にモデルを点滅させるタイマー
	const COBB* m_hitOBB = nullptr;					//衝突したOBBのポインタ

	/*
	* キャプチャ関連
	*/
	Quaternion m_capturedRotation = g_QUAT_IDENTITY;	//キャプチャ中にカメラに渡す回転
	Vector3 m_capturedPosition = g_VEC3_ZERO;		//キャプチャ中にカメラに渡す座標

	/*
	* ゴール関連
	*/
	float m_goalTimer = 0.0f;						//ゴールの時のタイマー

	/*
	* SE関連
	*/
	CSoundCue* m_fallstartSE = nullptr;	//落ちるときのサウンドキュー
	int m_fallcount = 0;				//落ちるときのカウンター
	CSoundCue* m_walkSE = nullptr;		//歩くときのサウンドキュー
	CSoundCue* m_runSE = nullptr;		//走るときのサウンドキュー
	CSoundCue* m_flameHitSE = nullptr;	//炎と当たったときのサウンド

	/*
	* 別のところで生成されるオブジェクト
	*/
	CMobius* m_mobius = nullptr;						//ステージのメビウスの輪のポインタ
	CDirectionLight* m_gameDirectionLight = nullptr;	//ディレクションライトのポインタ
	CGameCamera* m_gameCamera = nullptr;				//ゲームカメラ

	/// <summary>
	/// ウェイポイント関連のデータメンバ
	/// </summary>
	std::vector<Vector3>* m_wayPointPos;		//ウェイポイントの「場所」のコンテナ
	std::vector<Quaternion>* m_wayPointRot;		//ウェイポイントの「回転」のコンテナ
	int m_rpIndex = 0;							//右のウェイポイントのインデックス
	int m_lpIndex = m_rpIndex + 1;				//左のウェイポイントのインデックス
	int m_wayPointState = 0;					//自身がどのウェイポイントにいるか表すステート
	int m_maxWayPointState = 0;					//ウェイポイントステートの最大の値
	Quaternion m_finalWPRot = g_QUAT_IDENTITY;	//補完済みの最終的なウェイポイントの回転
};

