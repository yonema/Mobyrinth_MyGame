#pragma once

/**
 * @brief ���]�I�u�W�F�N�g�̒萔�f�[�^
*/
namespace reversibleObjectConstData
{
	//�E�F�C�|�C���g�����Z�����ւ̉��s
	constexpr float LENGHT_Z_FROMT_WEYPOINT = 100.0f;

	//�I�u�W�F�N�g�������グ��ꂽ���̍���
	constexpr float HEIGHT_HELD_UP = 330.0f;

	/**
	 * @brief �I�u�W�F�N�g�̌��݂̃X�e�[�g�i��ԁj
	 * ����ŃA�b�v�f�[�g���̏���������U��
	*/
	enum EN_OBJECT_STATE
	{
		EN_CHECK_HELD_PLAYER,	//�v���C���[�Ɏ�����邩�ǂ������ׂ�
		EN_LIFTED_PLAYER,		//�v���C���[�Ɏ����グ��ꒆ�̏��
		EN_HELD_PLAYER,			//�v���C���[�Ɏ�����Ă�����
		EN_THROWN_DOWN,			//�����Ă���I�u�W�F�N�g�����ɓ�����֐�
		EN_CANCEL,				//�����Ă���I�u�W�F�N�g�����̏�ɒu��
		EN_REPELLED,			//���ɒe�����
		EN_QUERY,				//�N�G�����Ăق����^�C�~���O

		EN_OVERLAP,				//��Q�I�u�W�F�N�g�ɏd�˂��Ă��邩�̊m�F
		//EN_OVERLAP_THROWN_DOWN, //�I�u�W�F�N�g���߂��Ă��鏈���i�c�j
	};

	/*
	* �T�E���h�֘A�̒萔
	*/

	//�������]���鎞��SE�̃t�@�C���p�X
	constexpr const wchar_t* const SOUND_FILEPATH_SE_CHANGE = L"Assets/sound/change.wav";
	//�������]����Ƃ���SE�̉���
	constexpr float SOUND_VOLUME_SE_CHANGE = 0.5f;

	//�������]���鎞��SE�̃t�@�C���p�X
	constexpr const wchar_t* const SOUND_FILEPATH_SE_THROW = L"Assets/sound/throw.wav";
	//�������]����Ƃ���SE�̉���
	constexpr float SOUND_VOLUME_SE_THROW = 0.5f;

	//�L���p�V�e�B�I�[�o�[����SE�̃t�@�C���p�X
	constexpr const wchar_t* const SOUND_FILEPATH_SE_CAPACITY_OVER = 
		L"Assets/sound/capacity_over.wav";
	//�L���p�V�e�B�I�[�o�[����SE�̉���
	constexpr float SOUND_VOLUME_SE_CAPACITY_OVER = 0.5f;


	/*
	* �G�t�F�N�g�֘A�̒萔
	*/
	
	//�S���]�̃G�t�F�N�g�̃t�@�C���p�X
	constexpr const char16_t* const EFFECT_FILEPATH_REVERSE_ALL = u"Assets/effect/reverseall2.efk";
	//�S���]�̃G�t�F�N�g�̊g�嗦
	constexpr float EFFECT_SCALE_REVERSE_ALL = 50.0f;
	//�S���]�̃G�t�F�N�g�̍��W�̍���
	constexpr float EFFECT_POSITION_HEIGHT_REVERSE_ALL = 100.0f;


	//���]�I�u�W�F�N�g��OBB�̏����T�C�Y
	const Vector3 OBB_SIZE_DEFAULT_REVERSIBLE_OBJECT = { 200.0f,200.0f,600.0f };


	/*
	* �v���C���[�Ɏ����オ��ꒆ�֘A�̒萔
	*/
	//�v���C���[�Ɏ����グ��ꒆ�̃^�C��
	constexpr float TIME_LIFTED_PLAYER = 0.45f;
	//�v���C���[�Ɏ����グ��ꒆ�̔����̃^�C��
	constexpr float TIME_LIFTED_PLAYER_HALF = TIME_LIFTED_PLAYER / 2.0f;
	//�v���C���[�Ɏ����グ��ꒆ�̐��������ɉ�������W
	constexpr float ADD_VECTOR_SCALE_HORIZONTAL_LIFTED_PLAYER = 150.0f;
	//�v���C���[�Ɏ����グ��ꒆ�́A�����O�ɑ҂^�C��
	constexpr float TIME_LIFTED_PLAYER_WAIT_BEFORE_MOVING = 0.1f;


	/*
	* ������ꒆ�֘A�̒萔
	*/

	/**
	 * @brief ���ɓ������邩�A��ɓ������邩
	*/
	enum EN_DOWN_OR_UP_THROWN
	{
		EN_START_THROWN,	//�ŏ��ɓ�������
		EN_DOWN_THROWN,		//���ɓ�������
		EN_UP_THROWN,		//��ɓ�������
	};
	//���ɓ�����ꒆ�̃^�C��
	constexpr float TIME_THROWN_DOWN = 0.5f;
	//���ɓ�����ꒆ�̔����̃^�C��
	constexpr float TIME_THROWN_DOWN_HALF = TIME_THROWN_DOWN / 2.0f;
	//���ɓ�����x�N�g���̑傫��
	constexpr float ADD_VECTOR_SCALE_HORIZONTAL_THORWN_DOWN = HEIGHT_HELD_UP * 2.0f * 1.2f;
	//�e����ċA���Ă���Ƃ��̃x�N�g���̑傫��
	constexpr float ADD_VECTOR_SCALE_HORIZONTAL_THORWN_DOWN_OVERLAP = HEIGHT_HELD_UP * 1.0f * 1.4f;
	//���ɓ�����Ƃ��̉��s�̃x�N�g���̑傫��
	constexpr float ADD_VECTOR_SCALE_DEPTH_THROWN_DOWN = 60000.0f;
	//���ɓ�����Ƃ��̉��s�̃x�N�g���̑傫���ɗݏ悷�鐔
	constexpr float POW_NUM_DEPTH_SCALE_THROWN_DOWN = 2.0f;

	/*
	* �L�����Z�����֘A�̒萔
	*/

	//�L�����Z�����̍��W�̍���
	constexpr float POSITION_HEIGHT_CANCEL = 20.0f;

	/*
	* �e�����Ƃ��֘A�̒萔
	*/

	//�e�����Ƃ��̎���
	constexpr float TIME_REPELLED = 1.0f;
	//�e�����Ƃ��̐��������ɉ�����x�N�g���̑傫��
	constexpr float ADD_VECTOR_SCALE_HORIZONTAL_REPELLED = 500.0f;
	//�e�����Ƃ��̐��������ɉ�����x�N�g���̑傫��
	constexpr float ADD_VECTOR_SCALE_VERTICAL_REPELLED = 2000.0f;
}