#pragma once

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{
	/**
	 * @brief �Z�[�u
	*/
	namespace nsSave
	{
		/**
		 * @brief �Z�[�u�f�[�^�̍\����
		*/
		struct SSaveData
		{
			int highestClearStageNum;	//��ԍ����N���A�����X�e�[�W�̔ԍ�
		};

		/**
		 * @brief �Z�[�u�N���X�̒萔�f�[�^
		*/
		namespace
		{
			//�Z�[�u�f�[�^�̃t�@�C���p�X
			const char* const SAVE_DATA_FILEPATH = "Assets/saveData/saveData.dat";
			//�o�C�i���t�@�C���ŏ������ރ��[�h
			const char* const MODE_WRITE_BINARY = "wb";
			//�o�C�i���΂���œǂݍ��ރ��[�h
			const char* const MODE_READ_BINARY = "rb";
		}


		/**
		 * @brief �Z�[�u�N���X
		 * @note �R���X�g���N�^�Ń��[�h������
		*/
		class CSave
		{
		public:		//�����ŌĂ΂�郁���o�֐�
			CSave();	//�R���X�g���N�^

		public:		//�����o�֐�

			/**
			 * @brief �f�[�^�����o�̃Z�[�u�f�[�^���Z�[�u����
			*/
			void SaveData();

			/// <summary>
			/// �Z�[�u�f�[�^���f�[�^�����o�Ƀ��[�h����
			/// </summary>
			/// <returns>���[�h�ł������H</returns>
			const bool LoadData();

			/// <summary>
			/// �f�[�^�����o�ɓ����Ă���Z�[�u�f�[�^�𓾂�
			/// </summary>
			/// <returns>�Z�[�u�f�[�^</returns>
			const SSaveData& GetSaveData()const
			{
				return m_saveData;
			}

			/// <summary>
			/// �Z�[�u�f�[�^���f�[�^�����o�ɃZ�b�g����
			/// </summary>
			/// <param name="saveData">�Z�[�u�f�[�^��</param>
			void SetSaveData(const SSaveData& saveData)
			{
				m_saveData = saveData;
			}

		private:	//�f�[�^�����o

			SSaveData m_saveData;	//�Z�[�u�f�[�^
		};

	}
}