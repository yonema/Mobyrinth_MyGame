#pragma once
#include "Player.h"
#include "StartDirecting.h"

class GameCamera : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;

private:
	/// <summary>
	/// ステージ開始時のカメラ
	/// </summary>
	void StartDirectingCamera();
	/// <summary>
	/// ゲーム中のカメラ
	/// </summary>
	void InGameCamera();

public:	//インライン関数
	void StartDirectingZoomInCamera(const bool b)
	{
		m_startDirectingZoomInCamera = b;
	}

private:
	Player* m_pPlayer = nullptr;	//プレイヤーのポインタ
	Vector3 m_toCameraPos;				//注視点から視点へのベクトル

	StartDirecting* m_startDirecting = nullptr;
	bool m_startDirectingZoomInCamera = false; //カメラを寄せる処理のフラグ
};

