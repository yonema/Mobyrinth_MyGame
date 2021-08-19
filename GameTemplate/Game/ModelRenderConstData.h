#pragma once

/**
 * @brief ���f�������_���[�̒萔�f�[�^
*/
namespace modelRenderConstData
{
	constexpr float ALPHA_VALUE_TRANSPARENT = 0.0f;					//�����̃A���t�@�l
	constexpr float ALPHA_VALUE_OPACITY = 1.0f;						//�s�����̃A���t�@�l

	//�A�j���[�V�����̃f�t�H���g�̕ۊǎ���
	constexpr float INTERPOLATE_TIME_DEFAULT = 0.2f;


	//tkm�̃t�@�C���g���q
	constexpr const char* const FILE_EXTENSION_TKM = ".tkm";
	//tks�̃t�@�C���g���q
	constexpr const char* const FILE_EXTENSION_TKS = ".tks";
	//�g���q�̒���
	constexpr size_t FILE_EXTENSION_LENGHT = 4;

	//�s�N�Z���V�F�[�_�\�̃f�t�H���g�̃G���g���[�|�C���g
	constexpr const char* const ENTRY_POINT_FUNC_PS_DEFAULT = "PSMain";
	//���_�V�F�[�_�[�̃f�t�H���g�̃G���g���[�|�C���g
	constexpr const char* const ENTRY_POINT_FUNC_VS_DEFAULT = "VSMain";
	//�X�L������̒��_�V�F�[�_�[�̃f�t�H���g�̃G���g���[�|�C���g
	constexpr const char* const ENTRY_POINT_FUNC_VS_SKIN_DEFAULT = "VSSkinMain";
	//�V�F�[�_�[�̃f�t�H���g�̃t�@�C���p�X
	constexpr const char* const SHADER_FILEPATH_DEFAULT = "Assets/shader/defferdRenderModel.fx";

	//�V�F�[�_�[�̃V���h�E�}�b�v�̃t�@�C���p�X
	constexpr const char* const SHADER_FILTPATH_SHADOW_MAP = "Assets/shader/DrawShadowMap.fx";

	//�V�F�[�_�[��Z�v���p�X�̃t�@�C���p�X
	constexpr const char* const SHADER_GILEPATH_Z_PREPASS = "Assets/shader/ZPrepass.fx";
}