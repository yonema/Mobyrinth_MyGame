#pragma once

/**
 * @brief �X�v���C�g�`��N���X�̒萔�f�[�^
*/
namespace spriteRenderConstData
{
	const Vector2 SPRITE_PIVOT_DEFALUT = { 0.5f,0.5f };			//�f�t�H���g�̃s�{�b�g
	const Vector3 SPRITE_SCALE_DEFAULT = { -1.0f,1.0f,1.0f };	//�f�t�H���g�̊g�嗦
	const float FLIP_HORIZONTAL = -1.0f;						//���E���]�̒l

	const float ALPHA_VALUE_TRANSPARENT = 0.0f;					//�����̃A���t�@�l
	const float ALPHA_VALUE_OPACITY = 1.0f;						//�s�����̃A���t�@�l

	//�V�F�[�_�[�̃f�t�H���g�̃t�@�C���p�X
	const char* const SHADER_FILEPATH_DEFAULT = "Assets/shader/sprite.fx";
	//�s�N�Z���V�F�[�_�\�̃f�t�H���g�̃G���g���[�|�C���g
	const char* const ENTRY_POINT_FUNC_PS_DEFAULT = "PSMain";
	//���_�V�F�[�_�[�̃f�t�H���g�̃G���g���[�|�C���g
	const char* const ENTRY_POINT_FUNC_VS_DEFAULT = "VSMain";
}