#pragma once
#include "Mobius.h"
#include "GameTime.h"
#include "UFO.h"
class ILevelObjectBase;
class Player;
class GameCamera;

class StartDirecting : public IGameObject
{
public:
	bool Start() override final;
	~StartDirecting();
	void Update() override final;

	//void SetWayPointPos(const std::size_t vecSize, std::vector<Vector3>* const posMap);
	//void SetWayPointRot(const std::size_t vecSize, std::vector<Quaternion>* rotMap);

private:
	///// <summary>
	///// ウェイポイントの更新処理
	///// </summary>
	//void CheckWayPoint();
	///// <summary>
	///// 移動処理
	///// </summary>
	//void Move();
	///// <summary>
	///// ステージに乗る
	///// </summary>
	//void GetOnStage();
	///// <summary>
	///// モデルの回転処理
	///// </summary>
	//void Rotation();
	/// <summary>
	/// プレイヤーの演出時の落下処理
	/// </summary>
	void FallPlayer();

public:	//Get、Set関数
	const bool GetStartDirecting()const
	{
		return m_checkStartDirecting;
	}

	const Vector3 GetPosition()const
	{
		return m_position;
	}
	/// <summary>
	/// 補完済みの最終的なウェイポイントの回転を取得
	/// </summary>
	/// <returns>補完済みの最終的なウェイポイントの回転</returns>
	const Quaternion GetFinalWPRot()const
	{
		return m_finalWPRot;
	}
	const int GetStartDirectingTime()const
	{
		return startDirectingTime;
	}
	/// <summary>
	/// 演出時のカメラの注視点の開始地点の座標を設定
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}
	void SetCheckStartDirecting(bool b)
	{
		m_checkStartDirecting = b;
	}

	/// <summary>
	/// ワイプが終わったかを設定
	/// </summary>
	/// <param name="wipeEndFlag">ワイプが終わった？</param>
	void SetWipeEndFlag(const bool wipeEndFlag)
	{
		m_wipeEndFlag = wipeEndFlag;
	}


private:
	Vector3 m_position = g_vec3Zero; //演出に使用する注視点座標
	Vector3 m_moveSpeed = g_vec3Zero;
	Vector3 m_onWayPosition = g_vec3Zero;		//道の上の座標
	Quaternion m_rotation = g_quatIdentity;
	Vector3 m_upVec = g_vec3Up;

	float m_padLStickXF = 0.0f;		//パッドの左スティックのX軸の入力情報

	std::vector<Vector3>* m_wayPointPos;		//ウェイポイントの「場所」のコンテナ
	std::vector<Quaternion>* m_wayPointRot;	//ウェイポイントの「回転」のコンテナ
	int m_rpIndex = 0;				//右のウェイポイントのインデックス
	int m_lpIndex = m_rpIndex + 1;	//左のウェイポイントのインデックス
	int m_wayPointState = 0;		//自身がどのウェイポイントにいるか表すステート
	int m_maxWayPointState = 0;		//ウェイポイントステートの最大の値
	Quaternion m_finalWPRot = g_quatIdentity;	//補完済みの最終的なウェイポイントの回転

	Mobius* m_mobius = nullptr;		//ステージのメビウスの輪のポインタ

	Player* m_player = nullptr;		//プレイヤーの初期化

	GameCamera* m_gameCamera = nullptr;		//ゲームカメラの初期化

	enum EnLeftOrRight
	{
		enLeft,		//左
		enRight,	//右
	};
	int m_leftOrRight = enRight;	//キャラクターの左右の向き

private:
	bool m_checkStartDirecting = true; //ステージ開始時の演出をしているかどうか。（GameCameraクラスで使用）
	bool checkZoomIn = false; //カメラのズーム処理のフラグ
	int countZoomIn = 0; //カメラのズーム中のカウント
	Vector3 m_startPosition = g_vec3Zero; //演出に使用する注視点座標の開始位置

	const int startDirectingTime = 120; //カメラが寄る演出の時間

	bool m_wipeEndFlag = false;			//ワイプが終わった？
	float m_startTimer = 0;				//演出開始までのタイマー

	float m_fallTimer = 0.0f;			//落下後から開始までの時間
	
	bool CheckAButton = false; //Aボタンを押したかの確認


public:
	bool GetCheckAButton()
	{
		return CheckAButton;
	}

	void SetCheckAButton(bool b)
	{
		CheckAButton = b;
	}


};

