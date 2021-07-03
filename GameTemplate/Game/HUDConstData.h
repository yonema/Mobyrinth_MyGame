#pragma once
extern const UINT g_FRAME_BUFFER_W;	//�t���[���o�b�t�@�̕�
extern const UINT g_FRAME_BUFFER_H;	//�t���[���o�b�t�@�̍���


namespace HUDConstData
{
	/**
	 * @brief ���C�v�̎��
	*/
	enum EN_WIPE_TYPE
	{
		EN_WIPE,					//���ʂ̃��C�v
		EN_CIRCLE_WIPE,				//�~�`���C�v
		EN_VERTICAL_STRIPE_WIPE,	//�c�ȃ��C�v
		EN_HORIZONTAL_STRIPE_WIPE,	//���ȃ��C�v
		EN_CHECKERBOARD_WIPE,		//�`�F�b�J�[�{�[�h���C�v	//�����܂ł����������Ă��Ȃ�

		EN_MONOCHROME,				//���m�N�����H
		EN_SEPIA,					//�Z�s�A�����H
		EN_NEGA,					//�l�K�|�W���]
		EN_NOIS,					//�m�C�Y���H
	};

	//�f�t�H���g�̃��C�v�̕���
	const Vector2 DIR_WIPE_DEFAULT = { -1.0f,1.0f };


	//���ʂ̃��C�v��X���̍ő�T�C�Y
	const float SIZE_MAX_X_WIPE = static_cast<float>(g_FRAME_BUFFER_W);
	//���ʂ̃��C�v��Y���̍ő�T�C�Y
	const float SIZE_MAX_Y_WIPE = static_cast<float>(g_FRAME_BUFFER_H);
	//�~�`���C�v�̍ő�T�C�Y
	const float SIZE_MAX_WIPE_CIRCLE = static_cast<float>(g_FRAME_BUFFER_W) / 1.5f;
	//�c�ȃ��C�v�̍ő�T�C�Y
	constexpr float SIZE_MAX_WIPE_STRIPE_VERTICAL = 64.0f;
	//���ȃ��C�v�̍ő�T�C�Y
	constexpr float SIZE_MAX_WIPE_STRIPE_HORIZONTAL = 64.0f;
	//�`�F�b�J�[�{�[�h���C�v�̍ő�T�C�Y
	constexpr float SIZE_MAX_WIPE_CHECKERBOARD = 128.0f;

	//�f�t�H���g�̃��C�v�̃X�s�[�h
	constexpr float SPEED_WIPE_DEFAULT = 120.0f;
	//���ʂ̃��C�v�̃X�s�[�h
	constexpr float SPEED_WIPE = 1200.0f;
	//�~�`���C�v�̃X�s�[�h
	constexpr float SPEED_WIPE_CIRLE = 600.0f;

	//���C�v�p�̃V�F�[�_�[�̃t�@�C���p�X
	constexpr const char* const SHADER_FILEPATH_WIPE = "Assets/shader/Wipe.fx";
	//���C�v�̌��ɉf���X�v���C�g�̃t�@�C���p�X
	constexpr const char* const SPRITE_FILEPATH_BACK_SPRITE = "Assets/Image/backSprite2.DDS";
	//�t�F�[�h�C���A�A�E�g�Ŏg���X�v���C�g�̃t�@�C���p�X
	constexpr const char* const SPRITE_FILEPATH_FADE = "Assets/Image/black.DDS";

}