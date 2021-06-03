#pragma once
#include "PlayerConstData.h"
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

//Playerの定数データを使えるようにする
using namespace PlayerConstData;

class GameCamera;
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
	 * @param [in] vecSize ウェイポイントのサイズ
	 * @param [in] posMap 座標のベクター
	*/
	void SetWayPointPos(const std::size_t vecSize, std::vector<Vector3>*const posMap);

	/**
	 * @brief ウェイポイントの「回転」を取得
	 * @param [in] vecSize ウェイポイントのサイズ
	 * @param [in] rotMap 回転のベクター
	*/
	void SetWayPointRot(const std::size_t vecSize, std::vector<Quaternion>* rotMap);


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
	const bool GetStunFlag()const
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
	const bool GetHoldObject()const
	{
		return m_holdObject;
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
	 * @brief タイトルか？を設定する
	 * @param [in] titleMove 
	*/
	void SetTitleMove(const bool titleMove)
	{
		m_titleMove = titleMove;
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
	const int GetLeftPointIndex()const
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
	const int GetRightPointIndex()const
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
	const int GetEnLeftOrRight()const
	{
		return m_leftOrRight;
	}

	/**
	 * @brief 自身がどのウェイポイントにいるか表すステートを戻す関数
	 * @return ウェイポイントの番号
	*/
	const int GetWayPointState()const
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
	const bool GetCapturedUFOFlag()const
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
	const bool GetFallFalg() const
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
	 * @brief プレイヤーの初期設定
	*/
	void Init();

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

private:	//定数
	
	/**
	 * @brief アニメーションクリップ。
	*/
	enum EnAnimationClip 
	{
		enAnimClip_idle,		//アイドル状態のアニメーションクリップ
		enAnimClip_walk,		//歩きのアニメーションクリップ
		enAnimClip_run,			//走りのアニメーションクリップ
		enAnimClip_carry,		//持ち上げるアニメーションクリップ
		enAnimClip_carryIdle,	//持っているアイドル状態のアニメーションクリップ
		enAnimClip_carryWalk,	//持ちながら歩くアニメーションクリップ
		enAnimClip_carryRun,	//持ちながら走るアニメーションクリップ
		enAnimClip_throw_l,		//投げるアニメーションクリップ
		enAnimClip_throw_r,		//投げるアニメーションクリップ
		enAnimClip_fall,		//落ちるアニメーションクリップ
		enAnimClip_num,			//アニメーションクリップの総数
	};

	/**
	 * @brief ゴール時のアニメーションクリップ
	*/
	enum EnGoalAnimationClip
	{
		enGoalAnimClip_idle,	//アイドル状態のアニメーションクリップ
		enGoalAnimClip_goal,	//ゴール時のアニメーションクリップ
		enGoalAnimClip_num,		//ゴール時のアニメーションクリップの数
	};

	/**
	 * @brief プレイヤーが右を向いているか左を向いているか
	*/
	enum EnLeftOrRight
	{
		enLeft,		//左
		enRight,	//右
	};

private:	//データメンバ

	/*
	* モデル、アニメーションデータ
	*/
	CModelRender* m_modelRender = nullptr;					//モデルレンダラー
	AnimationClip m_animationClips[enAnimClip_num];			//アニメーションクリップ。
	CModelRender* m_goalPlayerMR = nullptr;					//ゴール時のプレイヤーのモデル
	AnimationClip m_goalAnimationClips[enGoalAnimClip_num];	//アニメーションクリップ。

	/*
	* キャラクターのトランスフォーム
	*/
	Vector3 m_moveSpeed = g_vec3Zero;		//キャラクターの移動スピード
	Vector3 m_position = g_vec3Zero;		//キャラクターの座標
	Quaternion m_rotation = g_quatIdentity;	//キャラクターの回転

	/*
	* 移動関連
	*/
	CMyCharacterController m_myCharaCon;	//myキャラクターコントローラー
	float m_padLStickXF = 0.0f;				//パッドの左スティックのX軸の入力情報
	Vector3 m_onWayPosition = g_vec3Zero;	//道の上の座標
	Vector3 m_upVec = g_vec3Up;				//プレイヤーのUpベクトル

	/*
	* 状態
	*/
	int m_animState;									//アニメーションの状態
	int m_leftOrRight = enLeft;							//キャラクターの左右の向き
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
	bool m_stunLeftOrRight = enLeft;		//スタン中の吹っ飛ぶ方向
	float m_stunTimer = 0.0f;				//スタンのタイマー
	Vector3 m_stunMoveSpeed = g_vec3Zero;	//スタン中の吹っ飛ばすベクトル
	Vector3 m_stunDownVec = g_vec3Zero;		//スタン中の重力
	bool m_stunMoveFlag = true;				//スタン中に吹っ飛び中か？
	float m_blinkTimer = 0.0f;				//スタン状態にモデルを点滅させるタイマー
	COBB* m_hitOBB = nullptr;				//衝突したOBBのポインタ

	/*
	* キャプチャ関連
	*/
	Quaternion m_capturedRotation = m_rotation;		//キャプチャ中にカメラに渡す回転
	Vector3 m_capturedPosition = m_position;		//キャプチャ中にカメラに渡す座標

	/*
	* ゴール関連
	*/
	float m_goalTimer = 0.0f;						//ゴールの時のタイマー
	Effect* m_goalEffect = nullptr;					//ゴール時のエフェクト

	/*
	* SE関連
	*/
	CSoundCue* m_fallstartSE = nullptr;	//m_fallstartSEのサウンドキュー
	int m_fallcount = 0;				//fallSEサポート
	int m_falltimer = 0;				//fallSEサポート
	CSoundCue* m_walkSE = nullptr;		//m_walkSEのサウンドキュー
	CSoundCue* m_runSE = nullptr;		//m_runSEのサウンドキュー
	CSoundCue* m_flameHitSE = nullptr;	//炎と当たったときのサウンド

	/*
	* 別のところで生成されるオブジェクト
	*/
	Mobius* m_mobius = nullptr;							//ステージのメビウスの輪のポインタ
	CDirectionLight* m_gameDirectionLight = nullptr;	//ディレクションライトのポインタ
	GameCamera* m_gameCamera = nullptr;					//ゲームカメラ

	/// <summary>
	/// ウェイポイント関連のデータメンバ
	/// </summary>
	std::vector<Vector3>* m_wayPointPos;		//ウェイポイントの「場所」のコンテナ
	std::vector<Quaternion>* m_wayPointRot;		//ウェイポイントの「回転」のコンテナ
	int m_rpIndex = 0;							//右のウェイポイントのインデックス
	int m_lpIndex = m_rpIndex + 1;				//左のウェイポイントのインデックス
	int m_wayPointState = 0;					//自身がどのウェイポイントにいるか表すステート
	int m_maxWayPointState = 0;					//ウェイポイントステートの最大の値
	Quaternion m_finalWPRot = g_quatIdentity;	//補完済みの最終的なウェイポイントの回転
	std::vector<COBB> m_wayPointOBB;			//ウェイポイント用のOBB
};

