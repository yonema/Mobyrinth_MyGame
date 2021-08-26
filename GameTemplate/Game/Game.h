#pragma once
#include "Title.h"
#include "Tips.h"

/**
 * @brief ���r�����X
*/
namespace nsMobyrinth
{

	/// <summary>
	/// �Q�[���N���X
	/// </summary>
	class CGame : public IGameObject
	{
	public:		//�����ŌĂ΂�郁���o�֐�
		bool Start()override final;							//�X�^�[�g�֐�
		~CGame();											//�f�X�g���N�^

		//�f�o�b�N�p
		//��ŏ���
#ifdef MY_DEBUG
	public:
		//�f�o�b�N�p
		//��ŏ���
		void Update()override final;						//�A�b�v�f�[�g�֐�


	private:	//�f�[�^�����o
		std::vector<ILevelObjectBase*> m_lb;	//ILevelObjectBase�̏����󂯎��B
#endif

	};
}
