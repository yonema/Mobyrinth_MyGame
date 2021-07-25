#pragma once

/**
 * @brief ObstacleObjectのモデルのファイルパスとOBBのサイズの定数データ
*/
namespace OOsFilepathAndObbSizeConstData
{
	//「箱」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_BOX = "Assets/modelData/box.tkm";
	//「箱」のOBBのサイズ
	const Vector3 OBB_SIZE_BOX = { 250.0f,250.0f,250.0f };

	//「炎」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_FLAME = "Assets/modelData/flame.tkm";
	//「炎」のOBBのサイズ
	const Vector3 SIZE_OBB_FLAME = { 300.0f,300.0f,400.0f };

	//「ゴール」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_GOAL = "Assets/modelData/goal.tkm";
	//「ゴール」のOBBのサイズ
	const Vector3 SIZE_OBB_GOAL = { 100.0f,100.0f,100.0f };

	//左向きの「一方通行」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_ONEWAY_LEFT = "Assets/modelData/oneway.tkm";
	//左向きの通れる「一方通行」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_ONEWAY_LEFT_CAN_PASS = "Assets/modelData/oneway2.tkm";
	//右向きの「一方通行」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_ONEWAY_RIGHT = "Assets/modelData/oneway_r.tkm";
	//右向きの通れる「一方通行」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_ONEWAY_RIGHT_CAN_PASS = "Assets/modelData/oneway2_r.tkm";
	//「一方通行」のOBBのサイズ
	const Vector3 SIZE_OBB_ONEWAY = { 300.0f,300.0f,400.0f };
	//「一方通行」のサイドのOBBのサイズ
	const Vector3 SIZE_OBB_ONEWAY_SIDE = { 10.0f,400.0f,600.0f };

	//「南京錠」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_PADLOCK = "Assets/modelData/padlock1.tkm";
	//「南京錠」のOBBのサイズ
	const Vector3 SIZE_OBB_PADLOCK = { 300.0f,300.0f,300.0f };

	//「全反転」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_REVERSE_ALL = "Assets/modelData/box.tkm";
	//「全反転」のOBBのサイズ
	const Vector3 SIZE_OBB_REVERSE_ALL = { 10.0f,400.0f,400.0f };

	//「壁」のモデルのファイルパス
	constexpr const char* const MODEL_FILEPATH_WALL = "Assets/modelData/wall.tkm";
	//「壁」のモデルの拡大率
	const Vector3 MODEL_SCALE_WALL = { 400.0f,1000.0f,400.0f };
}