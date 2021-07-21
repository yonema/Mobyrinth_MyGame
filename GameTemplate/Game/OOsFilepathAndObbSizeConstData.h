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

	//���̃��f���̃t�@�C���p�X
	constexpr const char* const MODEL_FILEPATH_FLAME = "Assets/modelData/flame.tkm";
	//����OBB�̃T�C�Y
	const Vector3 SIZE_OBB_FLAME = { 300.0f,300.0f,400.0f };
}