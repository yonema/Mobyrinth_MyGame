#pragma once

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �J����
	*/
	namespace nsCamera
	{

		/**
		 * @brief �Q�[���J�����N���X�̒萔�f�[�^
		*/
		namespace gameCameraConstData
		{
			//�΂˃J�����̍ō��ړ����x
			constexpr float SPEED_MOVE_MAX_SPRINGCAMERA = 5000.0f;
			//�΂˃J�����̃X�t�B�A�R���W�����̔��a
			constexpr float RADIUS_COLLISION_SPHERE_SPRINGCAMERA = 0.0f;

			//�J�����̋ߕ���
			constexpr float CAMERA_PLANE_NEAR = 0.5f;
			//�J�����̉�����
			constexpr float CAMERA_PLANE_PAR = 50000.0f;

			//�ʏ펞�̃J�����̎��_�ւ̍��W
			const Vector3 POSITION_TO_CAMERA = { 0.0f,0.0f,1200.0f };

			//�J�����̃v���C���[����̍���
			constexpr float HEIGHT_FROM_PLAYER_CAMERA = 400.0f;

		}
	}
}