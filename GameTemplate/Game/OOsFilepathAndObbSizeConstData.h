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
}