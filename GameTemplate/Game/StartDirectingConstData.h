#pragma once

/**
 * @brief �J�n���o�̒萔�f�[�^
*/
namespace startDirectingConstData
{
	/*
	* �X�e�[�W�̎������鉉�o�֘A�̒萔
	*/

	//�J�n���o�̍ŏ��̏����҂^�C��
	const float TIME_FIRST_WAIT_START_DIRECTING = 0.5f;
	//�X�e�[�W�̎�������ő�̊p�x
	const float ANGLE_MAX_GO_AROUND_STAGE = 360.0f;
	//�X�e�[�W�̎�������^�C��
	const float TIME_GO_AROUND_STAGE = 10.0f;
	//�J�n���o���̃J�����̎��_�ւ̍��W
	const Vector3 POSITION_TO_CAMERA = { 0.0f,0.0f,4000.0f };
	//�J�����̒����_�̍��W
	const Vector3 POSITION_TARGET_CAMERA = { 0.0f,0.0f,0.0f };
	//�X�e�[�W�̎�������I����p�x�̊���
	const float SCALE_ANGLE_FINISH_GO_AROUND_STAGE = 0.9f;



	/*
	* �t�F�[�h�֘A�̒萔
	*/

	//�t�F�[�h�A�E�g�̃^�C��
	const float TIME_FADE_OUT = 0.5f;
	//�t�F�[�h�A�E�g�ƃt�F�[�h�C���̒��Ԃ̃^�C��
	const float TIME_FADE_WAIT = TIME_FADE_OUT + 0.5f;
	//�t�F�[�h�C���̃^�C��
	const float TIME_FADE_IN = TIME_FADE_WAIT + 0.5f;


	/*
	* �v���C���[�̗������o�֘A�̒萔
	*/

	//�v���C���[�̗������x
	const float SPEED_FALL_PLAYER = 1200.0f;
	//�n�ʂ̍���
	const float HEIGHT_GROUND = 1630.0f;
	//������ɑ҂^�C��
	const float TIME_WAIT_AFTER_FELL = 1.0f;
	//�v���C���[����J�����̍��W
	const Vector3 POSITION_PLAYER_TO_CAMERA = { 0.0f,0.0f,1200.0f };

}