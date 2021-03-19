#pragma once
#include "Title.h"

/// <summary>
/// �Q�[���N���X
/// </summary>
class Game : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;							//�X�^�[�g�֐�
	~Game();											//�f�X�g���N�^

	//�f�o�b�N�p
	//��ŏ���
	void PostRender(RenderContext& rc)override final;	//��ԏ�ɕ`�悷��֐�
	void Update()override final;						//�A�b�v�f�[�g�֐�


private:	//�f�[�^�����o
	int m_frameNum = 0;						//�t���[����

	//�f�o�b�N�p
	//��ŏ���
	std::vector<ILevelObjectBase*> m_lb;	//ILevelObjectBase�̏����󂯎��B

};

