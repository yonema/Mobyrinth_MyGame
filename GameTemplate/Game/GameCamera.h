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

	/// <summary>
	/// ズームが終了した
	/// </summary>
	void FinishZoom()
	{
		m_zoomFinished = true;
	}

	/// <summary>
	/// カメラの注視点を設定する
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const Vector3& target)
	{
		g_camera3D->SetTarget(target);
	}

	/// <summary>
	/// カメラの視点を設定する
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		g_camera3D->SetPosition(pos);
	}

	/// <summary>
	/// カメラの視点を取得する
	/// </summary>
	/// <returns>カメラの視点</returns>
	const Vector3 GetPosition() const
	{
		return g_camera3D->GetPosition();
	}

	/// <summary>
	/// タイトルモードにする
	/// </summary>
	void TitleMode()
	{
		m_titleMode = true;
	}

	/// <summary>
	/// プレイヤーを見るか？を設定する
	/// </summary>
	/// <param name="lookPlayerFlag">プレイヤーを見るか？</param>
	void SetLookPlayerFlag(const bool lookPlayerFlag)
	{
		m_lookPlayerFlag = lookPlayerFlag;
	}

private:	//データメンバ
	Player* m_pPlayer = nullptr;	//プレイヤーのポインタ
	Vector3 m_toCameraPos;			//注視点から視点へのベクトル
	bool m_lookPlayerFlag = true;	//プレイヤーを見るか？
	StartDirecting* m_startDirecting = nullptr;
	bool m_startDirectingZoomInCamera = false; //カメラを寄せる処理のフラグ

	bool m_zoomFinished = false;	//Zoomが終わったか

	bool m_titleMode = false;		//タイトル画面か？
};

