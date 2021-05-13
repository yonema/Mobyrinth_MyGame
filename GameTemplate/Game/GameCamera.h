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
	/// �X�e�[�W�J�n���̃J����
	/// </summary>
	void StartDirectingCamera();
	/// <summary>
	/// �Q�[�����̃J����
	/// </summary>
	void InGameCamera();

public:	//�C�����C���֐�
	void SetStartDirectingZoomInCamera(const bool b)
	{
		m_startDirectingZoomInCamera = b;
	}
	void FinishZoom()
	{
		m_zoomFinished = true;
	}
	void SetTarget(const Vector3& target)
	{
		g_camera3D->SetTarget(target);
	}
	void SetPosition(const Vector3& pos)
	{
		g_camera3D->SetPosition(pos);
	}

	void TitleMode()
	{
		m_titleMode = true;
	}
private:
	Player* m_pPlayer = nullptr;	//�v���C���[�̃|�C���^
	Vector3 m_toCameraPos;				//�����_���王�_�ւ̃x�N�g��

	StartDirecting* m_startDirecting = nullptr;
	bool m_startDirectingZoomInCamera = false; //�J�������񂹂鏈���̃t���O

	bool m_zoomFinished = false;	//Zoom���I�������

	bool m_titleMode = false;		//�^�C�g����ʂ��H
};

