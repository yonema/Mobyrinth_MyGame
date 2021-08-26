#pragma once

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief カメラ
	*/
	namespace nsCamera
	{

		/**
		 * @brief ゲームカメラクラスの定数データ
		*/
		namespace gameCameraConstData
		{
			//ばねカメラの最高移動速度
			constexpr float SPEED_MOVE_MAX_SPRINGCAMERA = 5000.0f;
			//ばねカメラのスフィアコリジョンの半径
			constexpr float RADIUS_COLLISION_SPHERE_SPRINGCAMERA = 0.0f;

			//カメラの近平面
			constexpr float CAMERA_PLANE_NEAR = 0.5f;
			//カメラの遠平面
			constexpr float CAMERA_PLANE_PAR = 50000.0f;

			//通常時のカメラの視点への座標
			const Vector3 POSITION_TO_CAMERA = { 0.0f,0.0f,1200.0f };

			//カメラのプレイヤーからの高さ
			constexpr float HEIGHT_FROM_PLAYER_CAMERA = 400.0f;

		}
	}
}