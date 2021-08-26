#include "stdafx.h"
#include "LevelObjectBase.h"
#include "Mobius.h"

//�f�o�b�N�p
#ifdef MY_DEBUG
//��ŏ���
int ILevelObjectBase::objectNumber = 0;
#endif

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

		namespace
		{
			//OBB�̃f�t�H���g�̑傫��
			const Vector3 OBB_SIZE_DEFAULT = { 100.0f,100.0f,100.0f };

			//�^�C�}�[�̃t�H���g�̃f�t�H���g�̃e�L�X�g
			const wchar_t* const FONT_TEXT_TIMER_DEFAULT = L"10";

			//�X�C�b�`ON�̎��̃G�t�F�N�g�̃t�@�C���p�X
			const char16_t* const EFFECT_FILEPATH_SWITCH_ON = u"Assets/effect2/activation.efk";
			//�X�C�b�`ON�̎��̃G�t�F�N�g�̊g�嗦
			const float EFFECT_SCALE_SWITCH_ON = 70.0f;

			//�X�C�b�`OFF�̎��̃G�t�F�N�g�̃t�@�C���p�X
			const char16_t* const EFFECT_FILEPATH_SWITCH_OFF = u"Assets/effect2/activation.efk";
			//�X�C�b�`OFF�̎��̃G�t�F�N�g�̊g�嗦
			const float EFFECT_SCALE_SWITCH_OFF = 70.0f;
		}


		//�X�^�[�g�֐�
		bool ILevelObjectBase::Start()
		{
			//�߂��̃E�F�C�|�C���g��T���āA�C�C�����ɉ�]����
			CheckWayPoint();

			//�v���C���[�̎Q�Ƃ�ێ�����
			m_player = CLevelObjectManager::GetInstance()->GetPlayer();

			//LevelObjectManager�ɃI�u�W�F�N�g���ǉ����ꂽ���Ƃ��
			CLevelObjectManager::GetInstance()->AddObject(this);

			//OBB������������
			InitOBB();


#ifdef MY_DEBUG
			//�f�o�b�N�p
			//��ŏ����B
			m_objectNumber = objectNumber++;

			Vector3* vertPos = m_obb.GetBoxVertex();
			for (int i = 0; i < m_dbgOBBNum; i++)
			{
				m_dbgOBBVert[i] = NewGO<CModelRender>(0);
				m_dbgOBBVert[i]->Init("Assets/modelData/dbgBox.tkm");
				m_dbgOBBVert[i]->SetPosition(vertPos[i]);
			}

			//�f�o�b�N�p�����܂�
#endif

	//�I�[�o�[���C�h���Ăق����֐�PureVirtualStart()
			return PureVirtualStart();

		}

		//�f�X�g���N�^
		ILevelObjectBase::~ILevelObjectBase()
		{
			//�X�C�b�`���I���ɂ����Ƃ��̃G�t�F�N�g�̔j��
			if (m_swichonEffect)
				DeleteGO(m_swichonEffect);

			//�X�C�b�`���I�t�ɂ����Ƃ��̃G�t�F�N�g�̔j��
			if (m_swichoffEffect)
				DeleteGO(m_swichoffEffect);

			//�t�H���g�̃^�C�}�[�̔j��
			if (m_timerFR)
				DeleteGO(m_timerFR);

#ifdef MY_DEBUG
			//�f�o�b�N�p
			//��ŏ���
			for (int i = 0; i < m_dbgOBBNum; i++)
			{
				DeleteGO(m_dbgOBBVert[i]);
			}
			//�f�o�b�N�p�����܂�
#endif

	//LevelObjectManager�ɃI�u�W�F�N�g���j�����ꂽ���Ƃ�`����
			CLevelObjectManager::GetInstance()->RemoveObject(this);

			return;
		}


		//�A�b�v�f�[�g�֐�
		void ILevelObjectBase::Update()
		{
			//�I�[�o�[���C�h�����֐�
			//�h���N���X�̃A�b�v�f�[�g�֐������ɌĂ΂��
			PureVirtualUpdate();

			//OBB�̏ꏊ�Ɖ�]��ݒ肷��
			m_obb.SetPosition(m_position);
			m_obb.SetRotation(m_rotation);

#ifdef MY_DEBUG
			//�f�o�b�N�p
			//��ŏ���
			Vector3* vertPos = m_obb.GetBoxVertex();
			for (int i = 0; i < m_dbgOBBNum; i++)
			{
				m_dbgOBBVert[i]->SetPosition(vertPos[i]);
				m_dbgOBBVert[i]->SetRotation(m_rotation);
			}
			//�f�o�b�N�p�����܂�
#endif

			return;
		}


		/// <summary>
		/// OBB�̏������֐�
		/// </summary>
		void ILevelObjectBase::InitOBB()
		{
			//OBB�̏������f�[�^
			nsOBB::SInitOBBData initData;
			initData.position = m_position;
			initData.width = OBB_SIZE_DEFAULT.x;
			initData.height = OBB_SIZE_DEFAULT.y;
			initData.length = OBB_SIZE_DEFAULT.z;
			initData.pivot = nsOBB::OBBConstData::OBB_PIVOT_DEFAULT;
			initData.rotation = m_rotation;

			//OBB�̏�����
			m_obb.Init(initData);

			return;
		}


		/**
		 * @brief ���g�̌��݂̍��W����A�E�F�C�|�C���g�̔ԍ������肵�A��]�ƍ��W���C�C�����ɍ��킹��
		 * @param [in] checkRotaton ��]�`�F�b�N���s�����H
		 * @param [in] checkPosition ���W�`�F�b�N���s�����H
		*/
		void ILevelObjectBase::CheckWayPoint(const bool checkRotaton, const bool checkPosition)
		{
			//LevelObjectManager����E�F�C�|�C���g�̏��������Ă���
			//�E�F�C�|�C���g�́u�ꏊ�v�������Ă���
			std::vector<Vector3>* wayPointPosVec
				= CLevelObjectManager::GetInstance()->GetWayPointPos();
			//�E�F�C�|�C���g�́u��]�v�������Ă���
			std::vector<Quaternion>* wayPointRotVec
				= CLevelObjectManager::GetInstance()->GetWayPointRot();


			//��ԋ߂��E�F�C�|�C���g�𒲂ׂ�

			//��ԋ߂������𒲂ׂ�悤�̕ϐ�
			//FLT_MAX�ifloat�^�̑傫�����j���ŏ��ɓ���Ă���
			float dist = FLT_MAX;
			//��ԋ߂��E�F�C�|�C���g�̃x�N�^�[�̃C���f�b�N�X��ێ����Ă����p�̕ϐ�
			int index = 0;
			//�E�F�C�|�C���g�̃x�N�^�[�̃T�C�Y��ێ�
			int vecSize = (*wayPointPosVec).size();

			//��ԋ߂��E�F�C�|�C���g�𒲂ׂ�
			for (int i = 0; i < vecSize; i++)
			{
				//�E�F�C�|�C���g�Ǝ��g�̊Ԃ̃x�N�g�����v�Z����
				Vector3 lenVec = (*wayPointPosVec)[i] - m_position;
				//�E�F�C�|�C���g�Ǝ��M�̊Ԃ̒������v�Z����
				float distTmp = lenVec.Length();

				//dist���߂������Ȃ�
				if (distTmp < dist)
				{
					//��ԋ߂��E�F�C�|�C���g�̃x�N�^�[�̃C���f�b�N�X��
					//���݂̌J��Ԃ��񐔂�ێ������āA
					index = i;
					//dist�ɂ��̋�����ێ�������
					dist = distTmp;
				}
			}

			//���g�̍��̃E�F�C�|�C���g�̃x�N�^�[�̃C���f�b�N�X
			int lpIndex;
			//���g�̉E�̃E�F�C�|�C���g�̃x�N�^�[�̃C���f�b�N�X
			int rpIndex;

			//��ԋ߂��E�F�C�|�C���g�̍��E�̃C���f�b�N�X�𒲂ׂ�
			{
				//��ԋ߂��E�F�C�|�C���g�̉E�̃C���f�b�N�X
				int right = index - 1;
				//��ԋ߂��E�F�C�|�C���g�̍��̃C���f�b�N�X
				int left = index + 1;

				//0��菬����������
				if (right < 0)
				{
					//���������
					right = vecSize;
					//�C���f�b�N�X�̍ő�l�̓x�N�^�[�̃T�C�Y���1����������-1����
					right -= 1;
				}
				//�x�N�^�[�̃T�C�Y�ȏゾ������
				//�C���f�b�N�X�̍ő�l�̓x�N�^�[�̃T�C�Y���1����������
				//�@>=�@�Ŕ�r����
				if (left >= vecSize)
				{
					//���������
					left = 0;
				}

				//��ԋ߂��E�F�C�|�C���g�̍��̃E�F�C�|�C���g�ƁA���g�Ƃ̊Ԃ̃x�N�g��
				Vector3 leftLen = (*wayPointPosVec)[left] - m_position;
				//��ԋ߂��E�F�C�|�C���g�̉E�̃E�F�C�|�C���g�ƁA���g�Ƃ̊Ԃ̃x�N�g��
				Vector3 rightLen = (*wayPointPosVec)[right] - m_position;

				//�E�ƍ��̋������ǂ��炪�߂�����r����
				if (leftLen.Length() <= rightLen.Length())
				{
					//���̕����߂��Ȃ�
					//���̃C���f�b�N�X���A��ԋ߂��E�F�C�|�C���g�̍��̃E�F�C�|�C���g
					//�Ɍ��肷��
					lpIndex = left;
					//�E�̃C���f�b�N�X���A��ԋ߂��E�F�C�|�C���g
					//�Ɍ��肷��
					rpIndex = index;
				}
				else
				{
					//�E�̕����߂��Ȃ�
					//�݂��̃C���f�b�N�X���A��ԋ߂��E�F�C�|�C���g�̉E�̃E�F�C�|�C���g
					//�Ɍ��肷��
					rpIndex = right;
					//���̃E�F�C�|�C���g���A��ԋ߂��E�F�C�|�C���g
					//�Ɍ��肷��
					lpIndex = index;
				}
			}

			//���E�̃E�F�C�|�C���g�̔ԍ����m�肷��
			m_lpIndex = lpIndex;
			m_rpIndex = rpIndex;

			//���b�N����Ă��邩�H
			if (!m_isLock)
			{
				//���b�N����Ă��Ȃ�

				//��]�`�F�b�N�����邩�H
				if (checkRotaton)
				{
					//���݂̍��W�ɍ��킹����]�ɂ���
					CheckRotation();
				}
				//���W�`�F�b�N�����邩�H
				if (checkPosition)
				{
					//�E�F�C�|�C���g�ɂ��낦�����W�ɂ���
					CheckPosition();
				}
			}

			return;
		}

		/**
		 * @brief ���݂̍��W�ɍ��킹����]�ɂ���
		*/
		void ILevelObjectBase::CheckRotation()
		{
			//LevelObjectManager����E�F�C�|�C���g�̏��������Ă���
			//�E�F�C�|�C���g�́u�ꏊ�v�������Ă���
			std::vector<Vector3>* wayPointPosVec
				= CLevelObjectManager::GetInstance()->GetWayPointPos();
			//�E�F�C�|�C���g�́u��]�v�������Ă���
			std::vector<Quaternion>* wayPointRotVec
				= CLevelObjectManager::GetInstance()->GetWayPointRot();

			//���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
			Vector3 lpToRpVec = (*wayPointPosVec)[m_rpIndex] - (*wayPointPosVec)[m_lpIndex];

			//���̃E�F�C�|�C���g���玩�g�ւ̃x�N�g��
			Vector3 lpToPosVec = m_position - (*wayPointPosVec)[m_lpIndex];

			//���K�������A���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
			Vector3 normalLpToRpVec = lpToRpVec;
			normalLpToRpVec.Normalize();

			//���̃E�F�C�|�C���g���玩�g�ւ̃x�N�g�����A�E�F�C�|�C���g��Ɏˉe��������
			float projLpToPosLen = Dot(normalLpToRpVec, lpToPosVec);

			//���g�����E�̃E�F�C�|�C���g�̊Ԃ̂ǂꂭ�炢�̈ʒu�ɂ��邩��
			//�⊮�����v�Z����
			float ComplementRate = projLpToPosLen / lpToRpVec.Length();

			//���ʐ��`�⊮
			//���g���ĉ�]������
			m_rotation.Slerp(ComplementRate, (*wayPointRotVec)[m_lpIndex], (*wayPointRotVec)[m_rpIndex]);

			return;
		}


		/**
		 * @brief ���W�����ꂢ�Ƀ��r�E�X�̗ւ̏�ɔz�u����
		*/
		void ILevelObjectBase::CheckPosition()
		{
			//LevelObjectManager����E�F�C�|�C���g�̏��������Ă���
			//�E�F�C�|�C���g�́u�ꏊ�v�������Ă���
			const std::vector<Vector3>* wayPointPosVec
				= CLevelObjectManager::GetInstance()->GetWayPointPos();

			//���̃E�F�C�|�C���g����E�̃E�F�C�|�C���g�ւ̃x�N�g��
			Vector3 normalLpToRpVec = (*wayPointPosVec)[m_rpIndex] - (*wayPointPosVec)[m_lpIndex];
			normalLpToRpVec.Normalize();

			//���̃E�F�C�|�C���g���玩�g�ւ̃x�N�g��
			const Vector3 lpToPosVec = m_position - (*wayPointPosVec)[m_lpIndex];

			//���̃E�F�C�|�C���g���玩�g�ւ̃x�N�g�����A�E�F�C�|�C���g��Ɏˉe��������
			const float projLpToPosLen = Dot(normalLpToRpVec, lpToPosVec);

			//�E�F�C�|�C���g��̍��W���o��
			Vector3 nextPos = normalLpToRpVec;	//���̍��W
			nextPos.Scale(projLpToPosLen);
			nextPos += (*wayPointPosVec)[m_lpIndex];

			//���r�E�X�̗ւɃs�b�^�����킹��
			Vector3 yVec = g_VEC3_UP;
			m_rotation.Apply(yVec);
			Vector3 IntersectLineVec = yVec;
			IntersectLineVec.Scale(300.0f);
			CLevelObjectManager::GetInstance()->GetMobius()->GetIntersectPosWithMobius(
				nextPos + IntersectLineVec, nextPos - IntersectLineVec, &nextPos
			);
			//���r�E�X�̗ւ����ɂ�����
			yVec.Scale(m_yPosLen);
			nextPos += yVec;

			//�E�F�C�|�C���g���牜�ɂ�点��
			Vector3 zVec = g_VEC3_BACK;
			m_rotation.Apply(zVec);
			zVec.Scale(m_zPosLen);
			nextPos += zVec;

			//���W���m�肷��
			m_position = nextPos;

			return;
		}

		/// <summary>
		/// ���g���\���ɂ��邩�����ɂ��邩�𒲂ׂ�֐�
		/// </summary>
		/// <param name="reversibleObject">���]�I�u�W�F�N�g���H</param>
		void ILevelObjectBase::CheckFrontOrBackSide(const bool reversibleObject)
		{
			//�\����������
			int nextFrontOrBackSide = CLevelObjectManager::GetInstance()->CheckFrontOrBackSide(m_lpIndex);

			//���]�I�u�W�F�N�g���H
			if (reversibleObject)
			{
				//���]�I�u�W�F�N�g
				//�X�e�[�W��̕\���̃A�C�e������ύX������

				//�����l�ł͂Ȃ�������
				if (m_frontOrBackSide != nsCommonData::EB_NONE_SIDE)
				{
					//�O�̏ꏊ�̔��]�I�u�W�F�N�g�̐������Z����
					CLevelObjectManager::GetInstance()->RemoveReversibleObjectNum(m_frontOrBackSide);
				}

				//���̏ꏊ�̔��]�I�u�W�F�N�g�̐������Z����
				CLevelObjectManager::GetInstance()->AddReversibleObjectNum(nextFrontOrBackSide);
			}

			//���݂̕\�������������m�肷��
			m_frontOrBackSide = nextFrontOrBackSide;
		}

		/// <summary>
		/// ���g�ƃv���C���[�̓����蔻��
		/// </summary>
		/// <returns>true���߂��Ă����瓖�����Ă���</returns>
		bool ILevelObjectBase::IsHitPlayer()
		{
			//�v���C���[���������Ă��Ȃ������牽��������return
			if (!m_player)
				return false;

			//OBB���m�̓����蔻������āA
			//���̌��ʂ�߂�
			return CollisionOBBs(m_player->GetOBB(), m_obb);
		}


		/// <summary>
		/// �����I�u�W�F�N�g�Ɏg�p����f�[�^������������B
		/// </summary>
		void ILevelObjectBase::SetTransparentObject()
		{
			//�����I�u�W�F�N�g����Ɏg�p����t���O��true�ɂ���B
			m_transparentObjectFlag = true;

			//�I�u�W�F�N�g�̏d�Ȃ��Ă��锻����s��Ȃ��悤�ɂ���B
			m_isHitFlag = false;

			//�^�C�}�[�̃t�H���g�����_���[�̐����Ə�����
			m_timerFR = NewGO<nsGraphic::nsFont::CFontRender>(nsCommonData::PRIORITY_SECOND);
			m_timerFR->Init(FONT_TEXT_TIMER_DEFAULT, g_VEC2_ZERO);
			//��\���ɂ���
			m_timerFR->Deactivate();

			//�X�C�b�`ON�̎��̃G�t�F�N�g�̐����Ə�����
			m_swichonEffect = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
			m_swichonEffect->Init(EFFECT_FILEPATH_SWITCH_ON);
			//�������̂ő傫�����Ă���
			m_swichonEffect->SetScale(EFFECT_SCALE_SWITCH_ON);

			//�X�C�b�`OFF�̎��̃G�t�F�N�g�̐����Ə�����
			m_swichoffEffect = NewGO<Effect>(nsCommonData::PRIORITY_FIRST);
			m_swichoffEffect->Init(EFFECT_FILEPATH_SWITCH_OFF);
			//�������̂ő傫�����Ă���
			m_swichoffEffect->SetScale(EFFECT_SCALE_SWITCH_OFF);

			//�J�n���ɓ����ɂ���Ƃ��ɃG�t�F�N�g���Ă΂Ȃ��悤�ɂ��邽�߂ɁA����������
			m_swichoffEffect->Deactivate();

			return;
		}


		/// <summary>
		/// �����X�C�b�`�������ꂽ�Ƃ��Ɏg�p�����֐�
		/// </summary>
		void ILevelObjectBase::TransparentSwitchOn()
		{
			//�X�C�b�`ON�̎��̃G�t�F�N�g�̍��W�Ɖ�]��ݒ�
			m_swichonEffect->SetPosition(m_position);
			m_swichonEffect->SetRotation(m_rotation);
			//�G�t�F�N�g���Đ�
			m_swichonEffect->Play();

			//�I�u�W�F�N�g�̏Փ˔�����s���悤�ɂ���B
			m_isHitFlag = true;

			//�I�u�W�F�N�g�̓����蔻���L���ɂ���B
			m_obb.SetExceptionFlag(false);


			//�^�C�}�[�̃t�H���g��\������
			m_timerFR->Activate();


			//���]�I�u�W�F�N�g�p�̃X�C�b�`ON�̎��̏���
			ReversibleSwitchOn();
		}


		/// <summary>
		/// �����X�C�b�`�̌��ʂ��������Ƃ��Ɏg�p�����֐�
		/// </summary>
		void ILevelObjectBase::TransparentSwitchOff()
		{
			//�J�n���ɃI�u�W�F�N�g�𓧖��ɂ���Ƃ��ɁA
			//�G�t�F�N�g���Đ�����Ȃ��悤�ɂ���

			//�X�C�b�`OFF�̃G�t�F�N�g���L�����H
			if (m_swichoffEffect->IsActive())
			{
				//�L��
				//�J�n���ȊO�ŌĂ΂��

				//�G�t�F�N�g�̍��W�Ɖ�]��ݒ肷��
				m_swichoffEffect->SetPosition(m_position);
				m_swichoffEffect->SetRotation(m_rotation);
				//�G�t�F�N�g���Đ�����
				m_swichoffEffect->Play();
			}
			else
			{
				//�L������Ȃ�
				//�J�n���ɌĂ΂��

				//�L���ɂ���
				m_swichoffEffect->Activate();
			}

			//�I�u�W�F�N�g�̏Փ˔�����s��Ȃ��悤�ɂ���B
			m_isHitFlag = false;

			//�I�u�W�F�N�g�̓����蔻��𖳌��ɂ���B
			m_obb.SetExceptionFlag(true);

			//�^�C�}�[�̃t�H���g���\���ɂ���
			m_timerFR->Deactivate();


			//���]�I�u�W�F�N�g�p�̃X�C�b�`��OFF�ɂ��鏈��
			ReversibleSwitchOff();

			return;
		}


		/// <summary>
		/// OBB���m�̓����蔻��
		/// </summary>
		/// <param name="lhs">���x���I�u�W�F�N�g1</param>
		/// <param name="rhs">���x���I�u�W�F�N�g2</param>
		/// <returns>true���߂��Ă����瓖�����Ă���</returns>
		bool IsHitLevelObject
		(const ILevelObjectBase& lhs, const ILevelObjectBase& rhs)
		{
			//�I�u�W�F�N�g�̏Փ˔�����s��Ȃ���Ԃ̏ꍇ
			if (lhs.GetIsHitFlag() == false || rhs.GetIsHitFlag() == false) {
				//�Փ˂��Ă��Ȃ����肷��B
				return false;
			}
			if (lhs.GetFrontOrBackSide() != rhs.GetFrontOrBackSide())
				return false;

			//OBB���m�̓����蔻������āA
			//���̌��ʂ�߂�
			return CollisionOBBs(lhs.GetOBB(), rhs.GetOBB());
		}

	}
}