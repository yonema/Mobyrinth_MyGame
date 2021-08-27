#pragma once
//#include "CommonData.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	//�v���C���[�N���X
	namespace nsPlayer { class Player; }

	/**
	 * @brief ���x���I�u�W�F�N�g
	*/
	namespace nsLevelObject
	{
		//�X�e�[�W�̃��r�E�X�̗փN���X
		namespace nsMobius { class CMobius; }

		//���x���I�u�W�F�N�g�̒��ۃN���X
		class ILevelObjectBase;

		/// <summary>
		/// ���x���I�u�W�F�N�g�̃}�l�[�W���[
		/// </summary>
		class CLevelObjectManager
		{
		private:	//�����ŌĂ΂�郁���o�֐�
			CLevelObjectManager();		//�R���X�g���N�^��private�ɉB��
			~CLevelObjectManager();		//�f�X�g���N�^��private�ɉB��

		private:	//static�ȃf�[�^�����o
			static CLevelObjectManager* m_instance;	//���g�̗B��̃C���X�^���X�����ϐ�

		public:		//static�ȃ����o�֐�

			/// <summary>
			/// �V���O���g���p�^�[��
			/// �B��̃C���X�^���X�����֐�
			/// �ŏ��ɌĂ�łˁI
			/// </summary>
			static void CreateInstance()
			{
				//�B��̃C���X�^���X�𐶐�����
				m_instance = new CLevelObjectManager;
			}

			/// <summary>
			/// �C���X�^���X�������֐�
			/// </summary>
			static void DeleteInstance()
			{
				//�B��̃C���X�^���X��j������
				delete m_instance;
			}

			/// <summary>
			/// �B��C���X�^���X�̎Q�Ƃ�߂��֐�
			/// </summary>
			/// <returns>�B��C���X�^���X�̎Q��</returns>
			static CLevelObjectManager* GetInstance()
			{
				return m_instance;
			}

		public:		//�����o�֐�

			/// <summary>
			/// �v���C���[�̎Q�Ƃ𓾂�
			/// </summary>
			/// <returns>�v���C���[�̎Q��</returns>
			nsMobyrinth::nsPlayer::Player* GetPlayer()const
			{
				return m_player;
			}

			/// <summary>
			/// �v���C���[�̎Q�Ƃ�ݒ肷��
			/// </summary>
			/// <param name="player">�v���C���[�̎Q��</param>
			void SetPlayer(nsMobyrinth::nsPlayer::Player* player)
			{
				m_player = player;
			}


			/// <summary>
			/// �E�F�C�|�C���g�́u�ꏊ�v��������
			/// </summary>
			/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
			/// <param name="posMap">�ꏊ�̃}�b�v</param>
			void InitWayPointPos(std::map<int, Vector3>& posMap);

			/// <summary>
			/// �E�F�C�|�C���g�́u��]�v��������
			/// </summary>
			/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
			/// <param name="rotMap">��]�̃}�b�v</param>
			void InitWayPointRot(std::map<int, Quaternion>& rotMap);

			/// <summary>
			/// �E�F�C�|�C���g�́u�ꏊ�v�̎Q�Ƃ𓾂�
			/// </summary>
			/// <returns>�E�F�C�|�C���g�́u�ꏊ�v�̎Q��</returns>
			std::vector<Vector3>* GetWayPointPos()
			{
				return &m_wayPointPos;
			}

			/// <summary>
			/// �E�F�C�|�C���g�́u��]�v�̎Q�Ƃ𓾂�
			/// </summary>
			/// <returns>�E�F�C�|�C���g�́u��]�v�̎Q��</returns>
			std::vector<Quaternion>* GetWayPointRot()
			{
				return &m_wayPointRot;
			}

			/**
			 * @brief �E�F�C�|�C���g��ł̈ړ�����v�Z����֐�
			 * @param [in] rpIndex ���݂̉E���̃E�F�C�|�C���g�̃C���f�b�N�X
			 * @param [in] pos ���݂̍��W
			 * @param [in] dist �ړ����鋗��
			 * @param [in] leftOrRight �E���Ɉړ����邩�������ړ����邩�B0:��,1:�E
			 * @param [out] pNextIndex �ړ���̃E�F�C�|�C���g�̃C���f�b�N�X
			 * @return �ړ���̍��W
			*/
			const Vector3 CalcWayPointNextPos
			(const int rpIndex, const Vector3& pos, const float dist, const bool leftOrRight, int* pNextIndex = nullptr);

			/// <summary>
			/// ���g�iLevelObjectManager�j�ɃI�u�W�F�N�g��ǉ�����
			/// </summary>
			/// <param name="object">�ǉ�����I�u�W�F�N�g</param>
			void AddObject(ILevelObjectBase* object);

			/// <summary>
			/// ���g�iLevelObjectManager�j����I�u�W�F�N�g��j������
			/// </summary>
			/// <param name="object">�j������I�u�W�F�N�g</param>
			void RemoveObject(ILevelObjectBase* object);

			/// <summary>
			/// ���g�iLevelObjectManager�j�ɓo�^���Ă���I�u�W�F�N�g�̒�����
			/// �N�G���i�₢���킹�j������B
			/// �^�C�v�ꗗ��ObjectType.h���Q��
			/// </summary>
			/// <typeparam name="T">�N�G������I�u�W�F�N�g�̌^</typeparam>
			/// <param name="objectType">�N�G������I�u�W�F�N�g�̃^�C�v</param>
			/// <param name="func">�N�G�����Ď��s�������֐�</param>
			template<class T>
			void QueryLevelObjects(const int objectType, std::function<bool(T* lo)> func)
			{
				//���g�iLevelObjectManager�j�ɓo�^���Ă���I�u�W�F�N�g�̒�����
				//������o���ăN�G�����Ă���
				for (auto lo : m_levelObjects) {
					//�w�肵���^�C�v�ƈ�v����I�u�W�F�N�g�𒲂ׂ�
					if (lo->GetObjectType() == objectType) {
						//�������B
						T* p = dynamic_cast<T*>(lo);
						//�֐������s����
						if (func(p) == false) {
							//�N�G�����f�B
							return;
						}
					}
				}
			}

			/**
			 * @brief �S�Ẵ��x���I�u�W�F�N�g�𒲂ׂāA�Փ˂��Ă��Ȃ������ׂ�
			 * @param [in] thisObject ���g�̃C���X�^���X 
			 * @param [out] hitObject �Փ˂����I�u�W�F�N�g
			 * @return �Փ˂��Ă��邩�H
			*/
			bool IsHitAllLevelObjects(const ILevelObjectBase& thisObject, const ILevelObjectBase*** hitObject);


			/// <summary>
			/// ���x���I�u�W�F�N�g�����ׂď�������
			/// </summary>
			void AllDeleteLOs();

			/// <summary>
			/// �\�����������́A���]�I�u�W�F�N�g�̐������Z����
			/// </summary>
			/// <param name="frontOrBackSide">�\����������</param>
			void AddReversibleObjectNum(const int frontOrBackSide)
			{
				m_reversibleObjectNum[frontOrBackSide]++;
			}

			/// <summary>
			/// �\�����������́A���]�I�u�W�F�N�g�̐������Z����
			/// </summary>
			/// <param name="frontOrBackSide">�\����������</param>
			void RemoveReversibleObjectNum(const int frontOrBackSide)
			{
				m_reversibleObjectNum[frontOrBackSide]--;
			}

			/// <summary>
			/// ���]�I�u�W�F�N�g�́A�\���Ɨ����̂��ꂼ��̐���߂��B
			/// �z��̐擪�A�h���X��߂�
			/// </summary>
			/// <returns>���̔z��̐擪�A�h���X</returns>
			const int* GetReversibleObjectNum() const
			{
				return m_reversibleObjectNum;
			}

			/// <summary>
			/// ���]�I�u�W�F�N�g�́A�\���Ɨ����̂��ꂼ��̍ő吔��ݒ肷��
			/// </summary>
			/// <param name="frontOfBackSide">�\�����������H</param>
			/// <param name="maxNum">�ő吔</param>
			void SetReversibleObjectMaxNum(const int frontOfBackSide, const int maxNum)
			{
				m_reversibleObjectMaxNum[frontOfBackSide] = maxNum;
			}

			/// <summary>
			/// ���]�I�u�W�F�N�g�́A�\���Ɨ����̂��ꂼ��̍ő吔��߂��B
			/// �z��̐擪�A�h���X��߂�
			/// </summary>
			/// <returns>���̔z��̐擪�A�h���X</returns>
			const int* GetReversibleObjectMaxNum()
			{
				return m_reversibleObjectMaxNum;
			}


			/// <summary>
			/// �v���C���[�Ɉ�ԋ߂��I�u�W�F�N�g�̃I�u�W�F�N�g�^�C�v��߂�
			/// </summary>
			/// <returns>�I�u�W�F�N�g�^�C�v</returns>
			const int GetNearestObjectType(const float nearDist = 500.0f);


			/// <summary>
			/// �z�u���Ă����郌�x���I�u�W�F�N�g�����̎Q�Ƃ̃x�N�^�[
			/// </summary>
			/// <returns>���x���I�u�W�F�N�g�����̎Q�Ƃ̃x�N�^�[</returns>
			std::vector<ILevelObjectBase*> GetLevelObjects()
			{
				return m_levelObjects;
			}

			/// <summary>
			/// �X�e�[�W�̃��r�E�X�̗ւ̎Q�Ƃ𓾂�
			/// </summary>
			/// <returns>���r�E�X�̗ւ̎Q��</returns>
			nsMobius::CMobius* GetMobius()const
			{
				return m_mobius;
			}

			/// <summary>
			/// �X�e�[�W�̃��r�E�X�̗ւ̎Q�Ƃ�ݒ肷��
			/// </summary>
			/// <param name="mobius">���r�E�X�̗ւ̎Q��</param>
			void SetMobius(nsMobius::CMobius* mobius)
			{
				m_mobius = mobius;
			}

			/**
			 * @brief �\���ɂ���̂������ɂ���̂��𒲂ׂ�
			 * @param [in] lpIndex ���g�̍����̃E�F�C�|�C���g�̔ԍ�
			 * @return �\����������
			*/
			const int CheckFrontOrBackSide(const int lpIndex);

			/**
			 * @brief �\���Ɨ����̂������l�̏��������̎擾
			 * @return �\���Ɨ����̂������l�̏�������
			*/
			const int GetFrontOrBackSideThresholdSmall() const
			{
				return m_frontOrBackSideThresholdSmall;
			}

			/**
			 * @brief �\���Ɨ����̂������l�̑傫�����̎擾
			 * @return �\���Ɨ����̂������l�̑傫����
			*/
			const int GetFrontOrBackSideThresholdBig() const
			{
				return m_frontOrBackSideThresholdBig;
			}

		private:	//private�ȃ����o�֐�

			/**
			 * @brief �\���Ɨ����̂������l�̏�����
			*/
			void InitFrontOrBackSideThreshold();

		private:	//�f�[�^�����o
			nsMobyrinth::nsPlayer::Player* m_player = nullptr;		//�v���C���[�̃|�C���^
			nsMobius::CMobius* m_mobius = nullptr;					//�X�e�[�W�̃��r�E�X�̗ւ̃|�C���^
			std::vector<ILevelObjectBase*> m_levelObjects;	//�C���X�^���X�������x���I�u�W�F�N�g�̔z��

			/*
			* �E�F�C�|�C���g�֘A
			*/
			std::vector<Vector3> m_wayPointPos;		//�E�F�C�|�C���g�́u�ꏊ�v�̃R���e�i
			std::vector<Quaternion> m_wayPointRot;	//�E�F�C�|�C���g�́u��]�v�̃R���e�i

			/*
			* �\�������֘A
			*/
			int m_frontOrBackSideThresholdSmall = 0;	//�\���Ɨ����̂������l�̏�������
			int m_frontOrBackSideThresholdBig = 0;		//�\���Ɨ����̂������l�̑傫����
			//���]�I�u�W�F�N�g�́A�\���Ɨ����̂��ꂼ��̐�
			int m_reversibleObjectNum[nsCommonData::EN_FRONT_SIDE_AND_BACK_SIDE_NUM] = { 0,0 };
			//���]�I�u�W�F�N�g�́A�\���Ɨ����̂��ꂼ��̍ő吔
			int m_reversibleObjectMaxNum[nsCommonData::EN_FRONT_SIDE_AND_BACK_SIDE_NUM] = { 0,0 };

		};


		/// <summary>
		/// LevelObjectManager�ɓo�^���Ă���I�u�W�F�N�g�̒�����
		/// �N�G���i�₢���킹�j������B
		/// �^�C�v�ꗗ��ObjectType.h���Q��
		/// </summary>
		/// <typeparam name="T">�N�G������I�u�W�F�N�g�̌^</typeparam>
		/// <param name="objectType">�N�G������I�u�W�F�N�g�̃^�C�v</param>
		/// <param name="func">�N�G�����Ď��s�������֐�</param>
		template<class T>
		static inline void QueryLOs(const int objectType, std::function<bool(T* lo)> func)
		{
			//LevelObjectManager�̎Q�Ƃ������Ă��āA
			//�N�G���̃����o�֐������������Ă���B
			return CLevelObjectManager::GetInstance()->QueryLevelObjects(objectType, func);
		}

		/**
		 * @brief �S�Ẵ��x���I�u�W�F�N�g�𒲂ׂāA�Փ˂��Ă��邩���ׂ�
		 * @param [in] thisObject ���g�̃C���X�^���X
		 * @param [out] hitObject �Փ˂����I�u�W�F�N�g
		 * @return �Փ˂��Ă��邩�H
		*/
		static inline bool IsHitAllLOs(
			const ILevelObjectBase& thisObject,
			const ILevelObjectBase** hitObject = nullptr
		)
		{
			return CLevelObjectManager::GetInstance()->IsHitAllLevelObjects(thisObject, &hitObject);
		}
	}
}