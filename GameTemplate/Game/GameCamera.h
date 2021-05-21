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

	/// <summary>
	/// �Y�[�����I������
	/// </summary>
	void FinishZoom()
	{
		m_zoomFinished = true;
	}

	/// <summary>
	/// �J�����̒����_��ݒ肷��
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const Vector3& target)
	{
		g_camera3D->SetTarget(target);
	}

	/// <summary>
	/// �J�����̎��_��ݒ肷��
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		g_camera3D->SetPosition(pos);
	}

	/// <summary>
	/// �J�����̎��_���擾����
	/// </summary>
	/// <returns>�J�����̎��_</returns>
	const Vector3 GetPosition() const
	{
		return g_camera3D->GetPosition();
	}

	/// <summary>
	/// �^�C�g�����[�h�ɂ���
	/// </summary>
	void TitleMode()
	{
		m_titleMode = true;
	}

	/// <summary>
	/// �v���C���[�����邩�H��ݒ肷��
	/// </summary>
	/// <param name="lookPlayerFlag">�v���C���[�����邩�H</param>
	void SetLookPlayerFlag(const bool lookPlayerFlag)
	{
		m_lookPlayerFlag = lookPlayerFlag;
	}

private:	//�f�[�^�����o
	Player* m_pPlayer = nullptr;	//�v���C���[�̃|�C���^
	Vector3 m_toCameraPos;			//�����_���王�_�ւ̃x�N�g��
	bool m_lookPlayerFlag = true;	//�v���C���[�����邩�H
	StartDirecting* m_startDirecting = nullptr;
	bool m_startDirectingZoomInCamera = false; //�J�������񂹂鏈���̃t���O

	bool m_zoomFinished = false;	//Zoom���I�������

	bool m_titleMode = false;		//�^�C�g����ʂ��H
};

