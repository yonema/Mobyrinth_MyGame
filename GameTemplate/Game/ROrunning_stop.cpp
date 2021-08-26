#include "stdafx.h"
#include "ROrunning_stop.h"

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
		 * @brief ���]�I�u�W�F�N�g
		*/
		namespace nsReversibleObject
		{

			//ReversibleObject�̃��f���̃t�@�C���p�X�̒萔�f�[�^���g�p�\�ɂ���
			using namespace ROsFilepathConstdata;

			//�X�^�[�g�֐�
			bool ROrunning_stop::StartSub()
			{
				//�������p�֐�
				Init(MODEL_FILEPATH_RUNNING, EN_RO_TYPE_RUNNING,
					MODEL_FILEPATH_STOP, EN_RO_TYPE_STOP);


				return true;
			}

			/// <summary>
			/// �N�G�����Ăق����^�C�~���O�ŌĂ΂��֐�
			/// </summary>
			void ROrunning_stop::QuerySub()
			{
				//��Q�I�u�W�F�N�g�́u�ǁv���N�G��
				QueryLOs<nsObstacleObject::OOwall>(EN_OO_TYPE_WALL, [&](nsObstacleObject::OOwall* wall) -> bool
					{
						//�u�ǁv�ƏՓ˂������̏���
						HitWall(wall);

						return true;
					}
				);

				return;
			}

			/**
			 * @brief �u�ǁv�ƏՓ˂������̏���
			 * @param [in] wall �u�ǁv�̃|�C���^
			*/
			void ROrunning_stop::HitWall(nsObstacleObject::OOwall* wall)
			{
				if (m_actionFlag)
				{
					//���g�Ɓu�ǁv���Փ˂�����
					if (IsHitLevelObject(*this, *wall))
					{
						//���g���u�ғ��v���H
						if (GetObjectType() == EN_RO_TYPE_RUNNING)
						{
							//�u�ғ��v
							//�ǂ��ғ����[�h�ɂ���
							wall->Running();
						}
						else
						{
							//�u��~�v
							//�ǂ��~���[�h�ɂ���
							wall->Stop();
						}

						//�Փ˂����ǂ̃A�h���X���|�C���^�ɕێ�
						m_pWall = wall;
						//���b�N���ɂ���
						SetLock(true);
						//�I�u�W�F�N�g���d�Ȃ��Ă��邩�𔻒肷�鏈���𓮂����Ȃ��悤�ɂ���B
						SetOverlapFlag(false);

						//�Փ˂����ǂɁA���łɒ��g�����邩�ǂ������ׂ�
						if (wall->GetRun_stop() && wall->GetRun_stop() != this)
						{
							//���g����������A���̒��g��j������
							DeleteGO(wall->GetRun_stop());
						}
						//�ǂɎ��g�̃|�C���^������
						wall->SetRun_stop(this);
					}
				}

				return;
			}

			//�A�b�v�f�[�g�֐�
			void ROrunning_stop::UpdateSub()
			{
				//�ǂ̃A�h���X��ێ����Ă�����A�ꏊ�Ɖ�]���X�V����
				if (m_pWall)
				{
					//�ǂ̏ꏊ����ǂ̂��炢�̏�̏ꏊ�ɂ��邩
					const float upVecScale = 100.0f;

					//�ǂ̉�]�𓾂�
					Quaternion qRot = m_pWall->GetRotation();
					//�A�b�v�x�N�g��
					Vector3 upVec = g_VEC3_UP;
					//�A�b�v�x�N�g���ɕǂ̉�]����Z����
					qRot.Apply(upVec);
					//���K������
					upVec.Normalize();
					//�C�C�����̑傫���ɃX�P�[������
					upVec.Scale(upVecScale);

					//�ꏊ���X�V
					SetPosition(m_pWall->GetPosition() + upVec);
					//��]���X�V
					SetRotation(qRot);
				}

				return;
			}
		}
	}
}