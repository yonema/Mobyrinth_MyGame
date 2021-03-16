#pragma once
#include "Player.h"
#include "StartDirecting.h"

class GameCamera : public IGameObject
{
public:
	bool Start()override final;
	void Update()override final;

private:
	/// <summary>
	/// �Q�[�����̃J����
	/// </summary>
	void InGameCamera();
	/// <summary>
	/// �X�e�[�W�J�n���̃J����
	/// </summary>
	void StartDirectingCamera();

private:
	Player* m_pPlayer = nullptr;	//�v���C���[�̃|�C���^
	Vector3 m_toCameraPos;				//�����_���王�_�ւ̃x�N�g��

	StartDirecting* m_startDirecting = nullptr;
};

