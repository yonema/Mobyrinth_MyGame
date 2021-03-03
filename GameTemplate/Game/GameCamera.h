#pragma once
#include "Player_kari.h"

class GameCamera : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;
private:
	Player_kari* m_pPlayer = nullptr;
	Vector3 m_toCameraPos;
};

