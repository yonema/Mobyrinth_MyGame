#pragma once
#include "LevelObjectBase.h"
#include "ModelRender.h"
#include "SpotLight.h"
#include "UFOLandingPoint.h"
#include "ReversibleObject.h"
#include "SoundCue.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief ���x���I�u�W�F�N�g
	*/
	namespace nsLevelObject
	{
		/**
		 * @brief ��Q�I�u�W�F�N�g
		*/
		namespace nsObstacleObject
		{

			/// <summary>
			/// UFO�N���X
			/// </summary>
			class CUFO : public ILevelObjectBase
			{
			public:		//�����ŌĂ΂�郁���o�֐�
				bool PureVirtualStart() override final;		//�X�^�[�g�֐�
				~CUFO();									//�f�X�g���N�^
				void PureVirtualUpdate() override final;	//�A�b�v�f�[�g�֐�

			public:		//�����o�֐�

				/// <summary>
				/// UFO�̈ړ����x��ݒ�
				/// </summary>
				/// <param name="moveSpeed">�ړ����x</param>
				void SetMoveSpeed(const float moveSpeed)
				{
					m_moveSpeed = moveSpeed;
				}

				/// <summary>
				/// UFO�̈ړ����x������̑��x�ɂ���
				/// </summary>
				void SetMoveSpeed()
				{
					m_moveSpeed = m_defaultSpeed;
				}

				void UFOmove();		//UFO�̈ړ�SE�͈�
				void UFOyellowlight();	//UFO�̉��F�����C�gSE�͈�

			private:	//private�ȃ����o�֐�

				void Search();		//�v���C���[��T������
				void Capture();		//�v���C���[�����������̏���
				void Transport();	//�v���C���[���^�ԏ���
				void Landing();		//���n�̏���
				void Leave();		//���n�_���痣��鏈��

				void Move();		//�ړ�����
				void GetOnStage();	//���r�E�X�̗ւ̃X�e�[�W�ɃC�C�����ɍ��킹�鏈��
				/// <summary>
				/// ��]����
				/// </summary>
				/// <returns>�v�Z���ꂽ��]</returns>
				const Quaternion Rotating();
				void UpdateLight();	//���C�g�̍X�V

			private:	//�񋓌^

				//�A�b�v�f�[�g�̃X�e�[�g�i��ԁj
				enum EnUpdateState
				{
					enSearch,		//�v���C���[��T��
					enCapture,		//�v���C���[��������
					enTransport,	//�v���C���[���^��
					enLanding,		//���n
					enLeave,		//���n�_���痣���
				};

				//�E����
				enum EnLeftOrRight
				{
					enLeft,			//��
					enRight,		//�E
				};

			private:	//�f�[�^�����o
				nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//���f�������_���[
				Vector3 m_onWayPosition = g_VEC3_ZERO;	//�E�F�C�|�C���g��̍��W
				nsMobius::CMobius* m_mobius = nullptr;				//�X�e�[�W�̃��r�E�X�̗ւ̃|�C���^
				Vector3 m_upVec = g_VEC3_UP;				//�A�b�v�x�N�g��
				bool m_leftOrRight = enRight;			//�E�������H
				float m_defaultSpeed = 1300.0f;			//�f�t�H���g�̈ړ����x
				float m_moveSpeed = 0.0f;				//�ړ����x
				int m_updateState = enSearch;			//�A�b�v�f�[�g�̃X�e�[�g�i��ԁj
				int m_searchFlag = true;				//�v���C���[��T���Ă��邩�H
				float m_timer = 0.0f;					//�^�C�}�[
				//CSpotLight* m_spotLightUp = nullptr;	//�X�|�b�g���C�g
				//CSpotLight* m_spotLightDown = nullptr;	//�X�|�b�g���C�g
				nsGraphic::nsModel::CModelRender* m_ufoLight = nullptr;		//UFO�̃��C�g
				CUFOLandingPoint* m_ufoLandingPoint = nullptr;	//���n�_�N���X
				bool m_getOnStageFlag = true;			//�X�e�[�W�̏�ɏ�鏈�����s�����H
				float m_ufoAngle = 0.0f;				//UFO�̉�]�̊p�x
				float m_ufoAngleSpeed = 100.0f;			//UFO�̉�]����X�s�[�x
				nsSound::CSoundCue* m_UFOmoveSE = nullptr;		//UFOmoveSE�̃T�E���h�L���[
				nsSound::CSoundCue* m_UFOcarrymoveSE = nullptr;	//UFOcarrymoveSE�̃T�E���h�L���[
				nsSound::CSoundCue* m_UFOyellowlightSE = nullptr;//UFOyellowlightSE�̃T�E���h�L���[
				nsSound::CSoundCue* m_UFOredlightSE = nullptr;	//UFOredlightSE�̃T�E���h�L���[
				nsSound::CSoundCue* m_UFOdiscoverySE = nullptr;	//UFOdiscoverySE�̃T�E���h�L���[

				//�f�o�b�N�p
			private:
#ifdef MY_DEBUG
				static const int m_vertNum = 8;				//���_�̐�
				CModelRender* m_dbgVertPosMR[m_vertNum];	//OBB�̒��_�����邽�߂̃��f��
				CModelRender* m_dbgRay[2] = { nullptr,nullptr };	//���C�̎n�_�ƏI�_�����邽�߂̃��f��
				CModelRender* m_dbgSpotLigPos = nullptr;		//�X�|�b�g���C�g�̈ʒu�����邽�߂̃��f��
#endif
			};

		}
	}
}