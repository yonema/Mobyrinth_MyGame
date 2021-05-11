#pragma once
#include "LightData.h"
#include "DirectionLight.h"
#include "ModelRender.h"
#include "Mobius.h"
#include "OBB.h"
#include "MyCharacterController.h"
#include "SoundCue.h"
#include "GameTime.h"

class CReversibleObject;



/// <summary>
/// プレイヤーが操作するキャラクターのクラス
/// </summary>
class Player : public IGameObject
{
public://publicなメンバ関数
	bool Start() override final;	//スタート関数
	~Player();						//デストラクタ
	void Update() override final;	//アップデート関数

	/// <summary>
	/// プレイヤーの座標を設定
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// プレイヤーの回転を設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// プレイヤーの座標を取得
	/// </summary>
	/// <returns>場所</returns>
	const Vector3& GetPosition()const
	{
		return m_position;
	}

	/// <summary>
	/// ウェイポイントの「場所」を取得
	/// </summary>
	/// <param name="vecSize">ウェイポイントのサイズ</param>
	/// <param name="posMap">場所のベクター</param>
	void SetWayPointPos(const std::size_t vecSize, std::vector<Vector3>*const posMap);

	/// <summary>
	/// ウェイポイントの「回転」を取得
	/// </summary>
	/// <param name="vecSize">ウェイポイントのサイズ</param>
	/// <param name="rotMap">回転のベクター</param>
	void SetWayPointRot(const std::size_t vecSize, std::vector<Quaternion>* rotMap);

	//ウェイポイント用のOBBを設定
	void SetWayPointOBB();

	/// <summary>
	/// 補完済みの最終的なウェイポイントの回転を取得
	/// </summary>
	/// <returns>補完済みの最終的なウェイポイントの回転</returns>
	const Quaternion& GetFinalWPRot()const
	{
		return m_finalWPRot;
	}
	
	/// <summary>
	/// プレイヤーがオブジェクトを持っているかどうかを設定する
	/// 持っている場合はtrueを渡す
	/// </summary>
	/// <param name="holdFlag">オブジェクトを持っているか？</param>
	void SetHoldObject(const bool holdFlag, CReversibleObject* reversibeObject = nullptr)
	{
		m_holdObject = holdFlag;
		if (m_holdObject)
			m_reversibleObject = reversibeObject;
		else
			m_reversibleObject = nullptr;
	}

	/// <summary>
	/// プレイヤーがオブジェクトを持っているかどうかを調べる
	/// 持っている場合はtrueが戻ってくる
	/// </summary>
	/// <returns>オブジェクトを持っているか</returns>
	const bool GetHoldObject()const
	{
		return m_holdObject;
	}

	/// <summary>
	/// 持っている反転オブジェクトの参照を得る
	/// </summary>
	/// <returns>持っている反転オブジェクトの参照</returns>
	CReversibleObject* GetReversibleObject()
	{
		return m_reversibleObject;
	}

	void SetOperationFlag(const bool b)
	{
		m_operationFlag = b;
	}
	void SetTitleMove(const bool b)
	{
		m_titleMove = b;
	}


	/// <summary>
	/// プレイヤーの現在のUpベクトルを得る
	/// </summary>
	/// <returns>Upベクトル</returns>
	const Vector3& GetUpVec()const
	{
		return m_upVec;
	}

	/// <summary>
	/// 現在のプレイヤーの左側のウェイポイントの番号を得る
	/// </summary>
	/// <returns>左側のウェイポイントの番号</returns>
	const int GetLeftPointIndex()const
	{
		return m_lpIndex;
	}

	/// <summary>
	/// プレイヤーの左側のウェイポイントを設定する
	/// </summary>
	/// <param name="lpIndex">左側のウェイポイント</param>
	void SetLeftPointIndex(const int lpIndex)
	{
		m_lpIndex = lpIndex;
	}

	/// <summary>
	/// 現在のプレイヤーの右側のウェイポイントの番号を得る
	/// </summary>
	/// <returns>右側のウェイポイントの番号</returns>
	const int GetRightPointIndex()const
	{
		return m_rpIndex;
	}

	/// <summary>
	/// プレイヤーの右側のウェイポイントを設定する
	/// </summary>
	/// <param name="lpIndex">右側のウェイポイント</param>
	void SetRightPointIndex(const int rpIndex)
	{
		m_rpIndex = rpIndex;
	}

	/// <summary>
	/// 引数で渡された番号のウェイポイントの場所を戻す
	/// </summary>
	/// <param name="index">ウェイポイントの番号</param>
	/// <returns>ウェイポイントの場所</returns>
	const Vector3& GerWayPointPos(const int index)
	{
		return (*m_wayPointPos)[index];
	}

	/// <summary>
	/// キャラクターが左と右どちらを向いているか調べる
	/// </summary>
	/// <returns>enLeft（0）かenRight（1）</returns>
	const int GetEnLeftOrRight()const
	{
		return m_leftOrRight;
	}

	/// <summary>
	/// 自身がどのウェイポイントにいるか表すステートを戻す関数
	/// </summary>
	/// <returns>ウェイポイントのステート</returns>
	const int GetWayPointState()const
	{
		return m_wayPointState;
	}

	/// <summary>
	/// 自身がどのウェイポイントにいるか表すステートを設定
	/// </summary>
	/// <param name="wayPointState">ウェイポイントのステート</param>
	void SetWayPointState(const int wayPointState)
	{
		m_wayPointState = wayPointState;
	}

	/// <summary>
	/// UFOに捕まっているか？を設定
	/// </summary>
	/// <param name="captured">UFOに捕まっているか？</param>
	void SetCapturedUFOFlag(const bool captured)
	{
		m_capturedUFOFlag = captured;
	}

	void SetFallFlag(const bool fallFlag)
	{
		m_fallFlag = fallFlag;
	}

	/// <summary>
	/// OBBの参照を戻す
	/// </summary>
	/// <returns>OBBの参照</returns>
	COBB& GetOBB()
	{
		return m_myCharaCon.GetOBB();
	}


private://privateなメンバ関数

	void TitleMove();
	void GameMove();

	/// <summary>
	/// プレイヤーの初期設定
	/// </summary>
	void Init();

	/// <summary>
	/// ウェイポイントの更新処理
	/// </summary>
	void CheckWayPoint();

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// スタン中の移動処理
	/// </summary>
	void StunMove();

	/// <summary>
	/// ステージに乗る
	/// </summary>
	void GetOnStage();

	/// <summary>
	/// スタン中のステージに乗る処理
	/// </summary>
	void StunGetOnStage();

	/// <summary>
	/// モデルの回転処理
	/// </summary>
	void Rotation();

	/// <summary>
	/// 衝突したOBBのタグを調べる
	/// </summary>
	void CheckHitOBBTag();

	/// <summary>
	/// ライトのデータを更新する
	/// </summary>
	void UpdateLightData();

	/// <summary>
	/// プレイヤーを照らす影を生成するライトを更新する
	/// </summary>
	void SetShadowParam();

	/// <summary>
	/// ディレクションライトを更新する
	/// </summary>
	void SetDirectionLight();

	/// <summary>
	/// UFOに捕まっている時の処理
	/// </summary>
	void CapturedUFO();

	void Fall();

private:	//データメンバ

	/// <summary>
	/// アニメーションクリップ。
	/// </summary>
	enum EnAnimationClip {
		enAnimClip_Idle,	//歩きのアニメーションクリップ
		enAnimClip_Run,		//走りのアニメーションクリップ
		enAnimClip_Num,		//アニメーションクリップの総数
	};

	AnimationClip m_animationClips[enAnimClip_Num];	//アニメーションクリップ。

	Vector3 m_moveSpeed = g_vec3Zero;				//キャラクターの移動スピード
	Vector3 m_position = g_vec3Zero;				//キャラクターの座標
	Vector3 m_onWayPosition = g_vec3Zero;			//道の上の座標
	Quaternion m_rotation = g_quatIdentity;			//キャラクターの回転
	Vector3 m_upVec = g_vec3Up;						//プレイヤーのUpベクトル
	bool m_holdObject = false;						//オブジェクトを持っているか？
	CReversibleObject* m_reversibleObject = nullptr;	//持っている反転オブジェクトのポインタ
	
	/// <summary>
	/// プレイヤーが右を向いているか左を向いているか
	/// </summary>
	enum EnLeftOrRight
	{
		enLeft,		//左
		enRight,	//右
	};
	int m_leftOrRight = enRight;			//キャラクターの左右の向き

	float m_padLStickXF = 0.0f;				//パッドの左スティックのX軸の入力情報

	CModelRender* m_modelRender = nullptr;	//モデルレンダラー
	CMyCharacterController m_myCharaCon;	//myキャラクターコントローラー
	Mobius* m_mobius = nullptr;				//ステージのメビウスの輪のポインタ
	CDirectionLight* m_gameDirectionLight = nullptr;	//ディレクションライトのポインタ
	Vector3 m_lightDirection = g_vec3Zero;	//ライトの照らす方向

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


	bool m_operationFlag = false; //操作できるかのフラグ
	bool m_titleMove = false;
	bool m_stunFlag = false;					//スタン状態（吹っ飛び状態）か？
	float m_stunTimer = 0.0f;					//スタンのタイマー
	Vector3 m_stunMoveSpeed = g_vec3Zero;		//スタン中の吹っ飛ばすベクトル
	Vector3 m_stunDownVec = g_vec3Zero;			//スタン中の重力
	bool m_stunMoveFlag = true;					//スタン中に吹っ飛び中か？
	float m_blinkTimer = 0.0f;					//スタン状態にモデルを点滅させるタイマー
	COBB* m_hitOBB = nullptr;					//衝突したOBBのポインタ

	bool m_capturedUFOFlag = false;				//UFOにキャプチャされているか？
	bool m_fallFlag = false;					//落下中か？

public://デバック用
	//void PostRender(RenderContext& rc)override final;	//デバック用のフォントを表示するため
	//Font m_font;							//フォントを表示するためのクラス
	//CModelRender* m_dbgModel = nullptr;		//プレイヤーとステージとの当たり判定を
	//CModelRender* m_dbgModel2 = nullptr;	//取るためのレイの視点と終点とステージとの
	//CModelRender* m_dbgModel3 = nullptr;	//交差点を見るためのモデル
	//bool m_dbgHit = false;					//上のレイが当たっているか表示するための変数

	//float m_dbgDot1 = 0.0f;			//左側のウェイポイントとプレイヤーの内積を入れる
	//float m_dbgDot2 = 0.0f;			//右側のウェイポイントとプレイヤーの内積を入れる

	//CModelRender* m_dbgObbModel[8] = { nullptr };	//OBBの頂点を見るためのモデル
	//CModelRender* m_dbgObbModel2[32][8];
	//CModelRender* m_dbgStunMoveModel = nullptr;
};

