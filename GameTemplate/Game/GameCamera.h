#pragma once
#include "Player.h"

class GameCamera : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;
private:
	Player* m_pPlayer = nullptr;	//�v���C���[�̃|�C���^
	Vector3 m_toCameraPos;				//�����_���王�_�ւ̃x�N�g��
};

