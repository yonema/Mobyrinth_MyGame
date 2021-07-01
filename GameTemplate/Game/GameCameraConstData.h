#pragma once

/**
 * @brief ゲームカメラクラスの定数データ
*/
namespace gameCameraConstData
{
	//ばねカメラの最高移動速度
	const float SPEED_MOVE_MAX_SPRINGCAMERA = 5000.0f;
	//ばねカメラのスフィアコリジョンの半径
	const float RADIUS_COLLISION_SPHERE_SPRINGCAMERA = 0.0f;

	//カメラの近平面
	const float CAMERA_PLANE_NEAR = 0.5f;
	//カメラの遠平面
	const float CAMERA_PLANE_PAR = 50000.0f;

	//通常時のカメラの視点への座標
	const Vector3 POSITION_TO_CAMERA = { 0.0f,0.0f,1200.0f };

	//カメラのプレイヤーからの高さ
	const float HEIGHT_FROM_PLAYER_CAMERA = 400.0f;

}