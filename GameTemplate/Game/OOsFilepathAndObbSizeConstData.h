#pragma once

/**
 * @brief ObstacleObject�̃��f���̃t�@�C���p�X��OBB�̃T�C�Y�̒萔�f�[�^
*/
namespace OOsFilepathAndObbSizeConstData
{
	//�u���v�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_BOX = "Assets/modelData/box.tkm";
	//�u���v��OBB�̃T�C�Y
	const Vector3 OBB_SIZE_BOX = { 250.0f,250.0f,250.0f };

	//�u���v�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_FLAME = "Assets/modelData/flame.tkm";
	//�u���v��OBB�̃T�C�Y
	const Vector3 SIZE_OBB_FLAME = { 300.0f,300.0f,400.0f };

	//�u�S�[���v�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_GOAL = "Assets/modelData/goal.tkm";
	//�u�S�[���v��OBB�̃T�C�Y
	const Vector3 SIZE_OBB_GOAL = { 100.0f,100.0f,100.0f };

	//�������́u����ʍs�v�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_ONEWAY_LEFT = "Assets/modelData/oneway.tkm";
	//�������̒ʂ��u����ʍs�v�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_ONEWAY_LEFT_CAN_PASS = "Assets/modelData/oneway2.tkm";
	//�E�����́u����ʍs�v�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_ONEWAY_RIGHT = "Assets/modelData/oneway_r.tkm";
	//�E�����̒ʂ��u����ʍs�v�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_ONEWAY_RIGHT_CAN_PASS = "Assets/modelData/oneway2_r.tkm";
	//�u����ʍs�v��OBB�̃T�C�Y
	const Vector3 SIZE_OBB_ONEWAY = { 300.0f,300.0f,400.0f };
	//�u����ʍs�v�̃T�C�h��OBB�̃T�C�Y
	const Vector3 SIZE_OBB_ONEWAY_SIDE = { 10.0f,400.0f,600.0f };

	//�u�싞���v�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_PADLOCK = "Assets/modelData/padlock1.tkm";
	//�u�싞���v��OBB�̃T�C�Y
	const Vector3 SIZE_OBB_PADLOCK = { 300.0f,300.0f,300.0f };

	//�u�S���]�v�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_REVERSE_ALL = "Assets/modelData/box.tkm";
	//�u�S���]�v��OBB�̃T�C�Y
	const Vector3 SIZE_OBB_REVERSE_ALL = { 10.0f,400.0f,400.0f };

	//�u�ǁv�̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_WALL = "Assets/modelData/wall.tkm";
	//�u�ǁv�̃��f���̊g�嗦
	const Vector3 MODEL_SCALE_WALL = { 400.0f,1000.0f,400.0f };
}