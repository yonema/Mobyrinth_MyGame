#pragma once

/**
 * @brief Player�̒萔�l
*/
namespace playerConstData
{
	/*
	* �A�j���[�V�����֘A�̒萔
	*/

	/**
	* @brief �ʏ�̃A�j���[�V�����N���b�v�B
	*/
	enum EN_ANIMATION_CLIP
	{
		EN_ANIM_CLIP_IDLE,		//�A�C�h����Ԃ̃A�j���[�V�����N���b�v
		EN_ANIM_CLIP_WALK,		//�����̃A�j���[�V�����N���b�v
		EN_ANIM_CLIP_RUN,		//����̃A�j���[�V�����N���b�v
		EN_ANIM_CLIP_CARRY,		//�����グ��A�j���[�V�����N���b�v
		EN_ANIM_CLIP_CARRY_IDEL,//�����Ă���A�C�h����Ԃ̃A�j���[�V�����N���b�v
		EN_ANIM_CLIP_CARRY_WALK,//�����Ȃ�������A�j���[�V�����N���b�v
		EN_ANIM_CLIP_CARRY_RUN,	//�����Ȃ��瑖��A�j���[�V�����N���b�v
		EN_ANIM_CLIP_THROW_L,	//�������̓�����A�j���[�V�����N���b�v
		EN_ANIM_CLIP_THROW_R,	//�E�����̓�����A�j���[�V�����N���b�v
		EN_ANIM_CLIP_FALL,		//������A�j���[�V�����N���b�v
		EN_ANIM_CLIP_NUM,		//�A�j���[�V�����N���b�v�̑���
	};

	/**
	 * @brief �S�[�����̃A�j���[�V�����N���b�v
	*/
	enum EN_GOAL_ANIMATION_CLIP
	{
		EN_GOAL_ANIM_CLIP_IDLE,	//�A�C�h����Ԃ̃A�j���[�V�����N���b�v
		EN_GOAL_ANIM_CLIP_GOAL,	//�S�[�����̃A�j���[�V�����N���b�v
		EN_GOAL_ANIM_CLIP_NUM,	//�S�[�����̃A�j���[�V�����N���b�v�̐�
	};


	/**
	 * @brief �ʏ�̃A�j���[�V�����̃t�@�C���p�X�ƃ��[�v�t���O���擾
	 * @param [in] clipNo �A�j���V�����N���b�v�̃i���o�[
	 * @param [out] filepath �A�j���[�V�����̃t�@�C���p�X
	 * @param [out] loopFlag ���[�v�t���O
	*/
	constexpr void GetAnimationFilePath(const int clipNo, const char** filepath, bool* loopFlag)
	{
		//�A�j���[�V�����N���b�v�̔ԍ��ɑΉ�����t�@�C���p�X�ƃ��[�v�t���O��ݒ肷��
		switch (clipNo)
		{
		case EN_GOAL_ANIM_CLIP_IDLE:
			*filepath = "Assets/animData/idle.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_WALK:
			*filepath = "Assets/animData/walk.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_RUN:
			*filepath = "Assets/animData/run.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_CARRY:
			*filepath = "Assets/animData/carry.tka";
			*loopFlag = false;
			break;
		case EN_ANIM_CLIP_CARRY_IDEL:
			*filepath = "Assets/animData/carryidle.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_CARRY_WALK:
			*filepath = "Assets/animData/carrywalk.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_CARRY_RUN:
			*filepath = "Assets/animData/carryrun.tka";
			*loopFlag = true;
			break;
		case EN_ANIM_CLIP_THROW_L:
			*filepath = "Assets/animData/throwL.tka";
			*loopFlag = false;
			break;
		case EN_ANIM_CLIP_THROW_R:
			*filepath = "Assets/animData/throwR.tka";
			*loopFlag = false;
			break;
		case EN_ANIM_CLIP_FALL:
			*filepath = "Assets/animData/fall.tka";
			*loopFlag = false;
			break;
		default:
			//�ǂ�ł��Ȃ�������G���[
			MessageBoxA(
				nullptr, "�A�j���[�V�����̃t�@�C���p�X���o�^����Ă��܂���", "�G���[", MB_OK
			);
			break;
		};
		return;
	}

	/**
	 * @brief 
	 * @param [in] clipNo �S�[�����̃A�j���[�V�����̃t�@�C���p�X�ƃ��[�v�t���O���擾
	 * @param [out] filepath �A�j���[�V�����̃t�@�C���p�X
	 * @param [out] loopFlag �A�j���[�V�����̃��[�v�t���O
	*/
	constexpr void GetGoalAnimationFilePath(const int clipNo, const char** filepath, bool* loopFlag)
	{
		//�A�j���[�V�����N���b�v�̔ԍ��ɑΉ�����t�@�C���p�X�ƃ��[�v�t���O��ݒ肷��
		switch (clipNo)
		{
		case EN_GOAL_ANIM_CLIP_IDLE:
			*filepath = "Assets/animData/goalidle.tka";
			*loopFlag = true;
			break;
		case EN_GOAL_ANIM_CLIP_GOAL:
			*filepath = "Assets/animData/goal.tka";
			*loopFlag = false;
			break;
		default:
			//�ǂ�ł��Ȃ�������G���[
			MessageBoxA(
				nullptr, "�A�j���[�V�����̃t�@�C���p�X���o�^����Ă��܂���", "�G���[", MB_OK
			);
			break;
		}

		return;
	}


	

	/*
	* ���f���֘A�̒萔
	*/

	//�v���C���[�̒ʏ�̃��f���̃t�@�C���p�X
	const char* const MODEL_FILEPATH_PLAYER = "Assets/modelData/player.tkm";

	//�v���C���[�̃S�[�����̃��f���̃t�@�C���p�X
	const char* const MODEL_FILEPATH_PLAYER_GOAL = "Assets/modelData/player2.tkm";



	/*
	* �ړ����x�֘A�̒萔
	*/

	//�����Ƃ��̈ړ����x
	const float MOVE_SPEED_WALK = 1000.0f;

	//����Ƃ��̈ړ����x
	const float MOVE_SPEED_RUN = 3000.0f;



	/*
	* �G�t�F�N�g�֘A�̒萔
	*/

	//�S�[�����̃G�t�F�N�g�̃t�@�C���p�X
	const char16_t* const EFFECT_FILEPATH_GOAL = u"Assets/effect2/goal.efk";
	//�S�[�����̃G�t�F�N�g�̊g�嗦
	const Vector3 EFFECT_SCALE_GOAL = { 150.0f,150.0f,150.0f };



	/*
	* �T�E���h�֘A�̒萔
	*/

	//������Ƃ���SE�̃t�@�C���p�X
	const wchar_t* const SE_FILEPATH_FALL_START = L"Assets/sound/fallstart.wav";
	//������Ƃ���SE�̃{�����[��
	const float SE_VOLUME_FALL_START = 0.5f;

	//�����Ƃ���SE�̃t�@�C���p�X
	const wchar_t* const SE_FILEPATH_WALK = L"Assets/sound/walk2.wav";
	//�����Ƃ���SE�̃{�����[��
	const float SE_VOLUME_WALK = 1.0f;

	//����Ƃ���SE�̃t�@�C���p�X
	const wchar_t* const SE_FILEPATH_RUN = L"Assets/sound/run2.wav";
	//����Ƃ���SE�̃{�����[��
	const float SE_VOLUME_RUN = 1.0f;

	//���ɓ��������Ƃ���SE�̃t�@�C���p�X
	const wchar_t* const SE_FILEPATH_FLAME_HIT = L"Assets/sound/flameHit.wav";
	//���ɓ��������Ƃ���SE�̃{�����[��
	const float SE_VOLUME_FLAME_HIT = 1.0f;


	/*
	* �L�����R���֘A�̒萔
	*/

	//�v���C���[��OBB�̃T�C�Y
	const Vector3 OBB_SIZE_PLAYER = { 100.0f ,200.0f, 200.0f };


	/*
	* ���r�E�X�̗ւƂ̓����蔻��֘A�̒萔
	*/

	//���r�E�X�̗ւ̌�����̒���
	const float INTERSECT_LINE_FOR_MOBIUS_LEN = 300.0f;



	/*
	* ���C�g�֘A�̒萔
	*/

	//�v���C���[���Ƃ炷���C�g�̕���
	const Vector3 LIGHT_DIRECTION = { 1.0f,-1.0f,0.0f };


	/*
	* �����OBB�Ƃ̏Փˊ֘A�̒萔
	*/

	//�ǂ̉��̂������l
	const float THRESHOLD_UNDER_WALL = 0.8F;



	/*
	* �X�^���֘A�̒萔
	*/

	//�X�^�����̉������̈ړ����x
	const float MOVE_SPEED_STUN_HORIZONTAL = 675.0f;

	//�X�^�����̏�����̈ړ����x
	const float MOVE_SPEED_STUN_UP = 1000.0f;

	//�X�^�����̉������̉����x
	const float MOVE_ACCELERATION_STUN_DOWN = -1800.0f;
	
	//�X�^�����̎���
	const float TIME_STUN = 3.0f;

	//�X�^�����̓_�ŉ�
	const int NUMBER_OF_BLINKS_STUN = 24;

	//�X�^�����̓_�ł̃J���[
	const Vector4 BLINK_COLOR_STUN = { 1.5f,0.2f,0.2f,1.0f };

	/*
	* �J�n���o�֘A�̒萔
	*/

	//�Q�[���̊J�n���̍��W
	const Vector3 POSITION_GAME_START = { 0.0f, 3000.0f, 0.0f };

	/*
	* �S�[���֘A�̒萔
	*/

	//�S�[�����ɃJ�������ړ����鎞��
	const float TIME_GOAL_CAMERA_MOVE = 0.5f;

	//�S�[�����̃J�����̒����_�Ɏg��������̃x�N�g���̒���
	const float CAMERA_TRGET_UPVEC_LEN_GOAL = 200.0f;

	//�S�[�����̃J��������v���C���[�ւ̃x�N�g���̒���
	const float CAMERA_TO_PLAYER_VEC_LEN_GOAL = 100.0f;



	/*
	* �^�C�g���֘A�̒萔
	*/

	//�^�C�g�����̃p�b�h�̍��X�e�B�b�N�̓��͒l
	const float TITLE_PAD_STICK_VALUE = 0.5f;



	/*
	* �����֘A�̒萔
	*/

	//�������̍����̍ŏ��l
	const float MIN_HEIGHT_FALL = 1800.0f;

}