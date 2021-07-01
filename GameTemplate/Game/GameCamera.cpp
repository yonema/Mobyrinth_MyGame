#include "stdafx.h"
#include "GameCamera.h"

//ゲームカメラの定数データを使えるようにする
using namespace gameCameraConstData;

//スタート関数
bool CGameCamera::Start()
{
	//プレイヤーを探す
	m_player = FindGO<Player>(GetGameObjectName(EN_GO_TYPE_PLAYER));

	//開始演出クラスにゲームカメラ（自身）を設定する
	m_startDirecting.SetGameCamera(this);

	//ばねカメラの初期化
	m_springCamera.Init(
		*g_camera3D, SPEED_MOVE_MAX_SPRINGCAMERA, false, RADIUS_COLLISION_SPHERE_SPRINGCAMERA
	);

	//近平面、遠平面の設定
	m_springCamera.SetNear(CAMERA_PLANE_NEAR);
	m_springCamera.SetFar(CAMERA_PLANE_PAR);

	//アップベクトルを設定
	m_springCamera.SetUp(g_VEC3_AXIS_Y);

	//カメラへの座標
	Vector3 toCameraPos = POSITION_TO_CAMERA;

	//タイトル画面か？
	if (m_titleMode)
	{
		//タイトル画面
		//開始演出を行わないようにする
		m_startDirecting.SetStartDirectingFlag(false);
	}
	else
	{	
		//タイトル画面ではない。ゲーム画面。
		//開始演出を行うようにする
		m_startDirecting.SetStartDirectingFlag(true);
		//開始演出時のカメラへの座標にする
		toCameraPos = startDirectingConstData::POSITION_TO_CAMERA;
	}

	//カメラの視点の注視点を設定する
	m_springCamera.SetPosition(toCameraPos);
	m_springCamera.SetTarget(startDirectingConstData::POSITION_TARGET_CAMERA);

	return true;
}

//アップデート関数
void CGameCamera::Update()
{
	//開始演出中か？
	if (m_startDirecting.GetStartDirectingFlag() == true) 
	{
		m_startDirecting.StartDirectingUpdate();
	}
	//ゲーム中
	else 
	{
		InGameCamera();
	}

	//ばねカメラの更新
	m_springCamera.Update();

	return;
}

/**
 * @brief ゲーム中のカメラ
*/
void CGameCamera::InGameCamera()
{
	//プレイヤーを見るか？
	if (m_lookPlayerFlag)
	{
		//見る

		//プレイヤーが見つかっているか？
		if (m_player)
		{
			//プレイヤーが見つかっていたら

			//注視点から視点へのベクトルを設定する
			Vector3 toCameraPos = POSITION_TO_CAMERA;
			//プレイヤーの回転を得る
			const Quaternion qRot = m_player->GetFinalWPRot();
			//カメラへのベクトルを回す
			qRot.Apply(toCameraPos);
			//アップベクトル
			Vector3 cameraUpVec = g_VEC3_AXIS_Y;
			//アップベクトルを回す
			qRot.Apply(cameraUpVec);

			//カメラのプレイヤーからの高さ
			Vector3 cameraHeightFromPlayer = cameraUpVec;

			//タイトル画面か？
			if (m_titleMode)
			{
				//タイトル画面
				//高さを伸ばす
				cameraHeightFromPlayer.Scale(HEIGHT_FROM_PLAYER_CAMERA);
			}
			else
			{
				//ゲーム画面
				//高さをなくす
				cameraHeightFromPlayer = g_VEC3_ZERO;
			}

			//視点を設定する
			m_springCamera.
				SetPosition(m_player->GetPosition() + toCameraPos + cameraHeightFromPlayer);
			//注視点を設定する
			m_springCamera.SetTarget(m_player->GetPosition() + cameraHeightFromPlayer);
			//アップ軸を設定する
			m_springCamera.SetUp(cameraUpVec);
		}
		else
		{
			//プレイヤーが見つかっていなかったら
			//プレイヤーを探す
			m_player = FindGO<Player>(GetGameObjectName(EN_GO_TYPE_PLAYER));
		}
	}

	return;
}