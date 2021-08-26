#pragma once
#include "LevelObjectBase.h"
#include "ModelRender.h"
#include "OBBWorld.h"
#include "OOsFilepathAndObbSizeConstData.h"

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
			/// �I�u�X�e�[�N���I�u�W�F�N�g�i��Q�I�u�W�F�N�g�j�i�ǂݕ������Ă�H�j
			/// ��Q�I�u�W�F�N�g�����Ƃ��͂��̃N���X���p�����Ă�
			/// </summary>
			class CObstacleObject : public ILevelObjectBase
			{
			public:		//�����ŌĂ΂�郁���o�֐�
				bool PureVirtualStart()override final;		//�X�^�[�g�֐�
				virtual ~CObstacleObject();					//�f�X�g���N�^
				void PureVirtualUpdate()override final;		//�A�b�v�f�[�g�֐�

			public:		//�I�[�o�[���C�h���������o�֐�

				/// <summary>
				/// ���f���̎Q�Ƃ𓾂�
				/// </summary>
				/// <param name="frontOrBack">���\���Ȃ��̂Ŋ֌W�Ȃ�</param>
				/// <returns>���f���̎Q��</returns>
				nsGraphic::nsModel::CModelRender* GetModelRender(const int frontOrBack = 0) override final
				{
					return m_modelRender;
				}


			public:		//�I�[�o�[���C�h���Ăق��������o�֐�
				virtual bool StartSub() { return true; };	//�X�^�[�g�֐�
				virtual void UpdateSub() {};				//�A�b�v�f�[�g�֐�

			protected:	//protected�ȃ����o�֐�

				/// <summary>
				/// �������֐�
				/// �ŏ��ɌĂ�łˁBtrue��߂��ĂˁB
				/// ���f���̃t�@�C���p�X�ƃI�u�W�F�N�g�̃^�C�v��
				/// �ݒ肷��
				/// �^�C�v�ꗗ��ObjectType.h���Q��
				/// </summary>
				/// <param name="filePath">���f����tkm�t�@�C���p�X</param>
				/// <param name="objectType">�^�C�v</param>
				/// <returns>true��߂��Ă�</returns>
				bool Init(const char* filePath, int objectType);

				/// <summary>
				/// ���Ȕ����F��ݒ肷��
				/// </summary>
				/// <param name="color">���Ȕ����F</param>
				void SetModelEmissionColor(const Vector4& color)
				{
					m_modelRender->SetModelEmissionColor(color);
				}


			private:	//�f�[�^�����o
				nsGraphic::nsModel::CModelRender* m_modelRender = nullptr;	//���f�������_���[
			};

		}
	}

}