#pragma once

/**
 * @brief �^�C�g���̒萔�f�[�^
*/
namespace titleConstData
{
	/**
	 * @brief �^�C�g���̃X�e�[�g�i��ԁj
	*/
	enum EN_STATE_TITLE
	{
		EN_START_TITLE,		//�N�������o�̏��
		EN_TITLE_SCREEN,	//�^�C�g����ʂ̏��
		EN_STAGE_SELECT,	//�X�e�[�W�Z���N�g�̏��
		EN_STAGE_DESICION,	//�X�e�[�W�����肵�����
	};

	/**
	 * @brief �X�e�[�W�̔ԍ�
	 * @note �V�����X�e�[�W��������ꍇ�A�����ɔԍ���ǉ�
	*/
	enum EN_STAGE_LIST
	{
		EN_STAGE_1,		//�X�e�[�W1
		EN_STAGE_2,		//�X�e�[�W2
		EN_STAGE_3,		//�X�e�[�W3
		EN_STAGE_4,		//�X�e�[�W4
		EN_STAGE_5,		//�X�e�[�W5
		EN_STAGE_6,		//�X�e�[�W6
		EN_STAGE_7,		//�X�e�[�W7
		EN_STAGE_8,		//�X�e�[�W8
		EN_STAGE_9,		//�X�e�[�W9
		EN_STAGE_NUM,	//�X�e�[�W�̑���
	};

	/**
	 * @brief �X�e�[�W�̃��x���f�[�^�̃t�@�C���p�X���擾
	 * @param [in] stageNum �X�e�[�W�̔ԍ�
	 * @return �X�e�[�W�̃��x���f�[�^�̃t�@�C���p�X
	*/
	constexpr const char* const GetLevelFilePath(const int stageNum)
	{
		//�X�e�[�W�̔ԍ��ɑΉ�����t�@�C���p�X��߂�
		switch (stageNum)
		{
		case EN_STAGE_1:
			return "Assets/level/O_easy.tkl";
			break;
		case EN_STAGE_2:
			return "Assets/level/Y_easy.tkl";
			break;
		case EN_STAGE_3:
			return "Assets/level/O_normal.tkl";
			break;
		case EN_STAGE_4:
			return "Assets/level/T_hard.tkl";
			break;
		case EN_STAGE_5:
			return "Assets/level/T_normal.tkl";
			break;
		case EN_STAGE_6:
			return "Assets/level/Y_normal.tkl";
			break;
		case EN_STAGE_7:
			return "Assets/level/O_hard.tkl";
			break;
		case EN_STAGE_8:
			return "Assets/level/T_easy.tkl";
			break;
		case EN_STAGE_9:
			return "Assets/level/Y_hard.tkl";
			break;
		}

		//�ǂ�ł��Ȃ�������G���[
		MessageBoxA(
			nullptr, "�X�e�[�W�̃��x���f�[�^���o�^����Ă��܂���", "�G���[", MB_OK
		);

		return nullptr;
	}

	//�X�e�[�W4�̃X�C�b�`�̃^�C��
	const float SWITCH_TIME_STAGE_4 = 12.0f;

	//��ԍŏ��̃X�e�[�W�ԍ�
	const int STAGE_NUM_FIRST = EN_STAGE_1;

	//��ԍő�̃X�e�[�W�ԍ�
	const int STAGE_NUM_LAST = EN_STAGE_NUM - 1;


	/*
	* �^�C�g���̃X�v���C�g�֘A
	*/

	//�^�C�g����ʂ̃��x��2D�̃t�@�C���p�X
	const char* const LEVEL_2D_FILEPATH_TITLE = "Assets/level2D/Title.casl";

	//���x��2D�̃^�C�g���̃I�u�W�F�N�g�l�[��
	const char* const LEVEL_2D_OBJECT_NAME_TITLE = "Title";

	//���x��2D�́uA�{�^���������Ăˁv�̃I�u�W�F�N�g�l�[��
	const char* const LEVEL_2D_OBJECT_NAME_PRESS_A_BUTTON = "Press_A_Button";

	//�J�n�̉��o�̍ő�J�E���g
	const int MAX_COUNT_START_TITLE = 120;

	//�uA�{�^���������Ăˁv�̃A���t�@�l�̕ω��X�s�[�h
	const float SPEED_CHANGE_ALPHA_PRESS_A_BUTTON = 0.02f;

	//�uA�{�^���������Ăˁv�̕s�����Ɣ��f���邵�����l
	const float THRESHOLD_OPACITY_PRESS_A_BUTTON = 0.8f;


	/*
	* �X�e�[�W�Z���N�g�̃X�v���C�g�֘A
	*/
	
	//�X�e�[�W�Z���N�g��ʂ̃��x��2D�̃t�@�C���p�X
	const char* const LEVEL_2D_FILEPATH_STAGE_SELECTION = 
		"Assets/level2D/Stage_selection3.casl";

	//���x��2D�̃X�e�[�W�A�C�R���̃I�u�W�F�N�g�l�[��
	const char* const LEVEL_2D_OBJECT_NAME_STAGE_ICON = "Stage_icon";
	//�X�e�[�W�A�C�R���̃i���o�[���擾���邽�߂̕���
	const char* const STAGE_ICON_GET_NUM_CHAR = "n";
	//�V�ׂȂ��X�e�[�W�̏�Z�J���[
	const Vector4 SPRITE_MUL_COLOR_STAGE_PLAY_CANNOT = { 0.4f,0.4f,0.4f,1.0f };

	//���x��2D�̃X�e�[�W�N���A�̃A�C�R���̃I�u�W�F�N�g�l�[��
	const char* const LEVEL_2D_OBJECT_NAME_STAGE_CLEAR = "Stage_clear";
	//�X�e�[�W�N���A�̃i���o�[���擾���邽�߂̕���
	const char* const STAGE_CLEAR_GET_NUM_CHAR = "r";

	//���x��2D�̃J�[�\���̃I�u�W�F�N�g�l�[��
	const char* const LEVEL_2D_OBJECT_NAME_CURSOR = "cursor";

	//���x��2D�̃X�e�[�W�Z���N�g�̃I�u�W�F�N�g�l�[��
	const char* const LEVEL_2D_OBJECT_NAME_STAGE_SELECTION = "Stage_selection";

	//���x��2D�̃X�e�[�W�I��w�i�̃I�u�W�F�N�g�l�[��
	const char* const LEVEL_2D_OBJECT_NAME_STAGE_SELECTION_BASE =
		"Stage_selection_base";
	//�X�e�[�W�I��w�i�̃X�v���C�g�̏�Z�J���[
	const Vector4 SPRITE_MUL_COLOR_STAGE_SELECTION_BASE = { 1.0f,1.0f,1.0f,0.8f };


	/*
	* �T�E���h�֘A�̒萔
	*/

	//�^�C�g����BGM�̃T�E���h�̃t�@�C���p�X
	const wchar_t* const SOUND_FILEPATH_BGM_TITLE = L"Assets/sound/Title.wav";
	//�^�C�g����BGM�̃T�E���h�̉���
	const float SOUND_VOLUME_BGM_TITLE = 0.5f;

	//A�{�^����SE�̃T�E���h�̃t�@�C���p�X
	const wchar_t* const SOUND_FILEPATH_SE_BUTTON_A = L"Assets/sound/buttonA.wav";
	//A�{�^����SE�̃T�E���h�̉���
	const float SOUND_VOLUME_SE_BUTTON_A = 0.5f;

	//B�{�^����SE�̃T�E���h�̃t�@�C���p�X
	const wchar_t* const SOUND_FILEPATH_SE_BUTTON_B = L"Assets/sound/buttonB.wav";
	//B�{�^����SE�̃T�E���h�̉���
	const float SOUND_VOLUME_SE_BUTTON_B = 0.5f;

	//�J�[�\���ړ���SE�̃T�E���h�̃t�@�C���p�X
	const wchar_t* const SOUND_FILEPATH_SE_SELECT = L"Assets/sound/select.wav";
	//�J�[�\���ړ���SE�̃T�E���h�̉���
	const float SOUND_VOLUME_SE_SELECT = 0.5f;



	//���X�e�B�b�N�̓��͂�����Ɣ��f���邵�����l
	const float THRESHOLD_STICK_L_VALUE = 0.5f;

}