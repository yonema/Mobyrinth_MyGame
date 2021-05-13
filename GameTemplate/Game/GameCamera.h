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
	void SetStartDirectingZoomInCamera(const bool b)
	{
		m_startDirectingZoomInCamera = b;
	}
	void FinishZoom()
	{
		m_zoomFinished = true;
	}
	void SetTarget(const Vector3& target)
	{
		g_camera3D->SetTarget(target);
	}
	void SetPosition(const Vector3& pos)
	{
		g_camera3D->SetPosition(pos);
	}

	void TitleMode()
	{
		m_titleMode = true;
	}
private:
	Player* m_pPlayer = nullptr;	//プレイヤーのポインタ
	Vector3 m_toCameraPos;				//注視点から視点へのベクトル

	StartDirecting* m_startDirecting = nullptr;
	bool m_startDirectingZoomInCamera = false; //カメラを寄せる処理のフラグ

	bool m_zoomFinished = false;	//Zoomが終わったか

	bool m_titleMode = false;		//タイトル画面か？
};

