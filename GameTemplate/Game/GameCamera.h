#pragma once
#include "SpriteRender.h"
#include "Player.h"
#include "StartDirecting.h"
#include "SpringCamera.h"

/// <summary>
/// �Q�[�����J�����N���X
/// </summary>
class GameCamera : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;		//�X�^�[�g�֐�
	void Update()override final;	//�A�b�v�f�[�g�֐�

private:	//�����o�֐�
	/// <summary>
	/// �X�e�[�W�J�n���̃J����
	/// </summary>
	void StartDirectingCamera();
	/// <summary>
	/// �X�e�[�W�J�n���̃J�����̃t�F�[�h
	/// </summary>
	void FadeDirectingCamera();
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
	Player* m_pPlayer = nullptr;				//�v���C���[�̃|�C���^
	Vector3 m_toCameraPos;						//�����_���王�_�ւ̃x�N�g��
	bool m_lookPlayerFlag = true;				//�v���C���[�����邩�H
	StartDirecting* m_startDirecting = nullptr;
	bool m_startDirectingZoomInCamera = false;	//�J�������񂹂鏈���̃t���O

	bool m_zoomFinished = false;				//Zoom���I�������

	bool m_titleMode = false;					//�^�C�g����ʂ��H



	float m_switchingTimer = 0.0f;				//�X�C�b�`�̐؂�ւ����̃^�C�}�[
	CSpriteRender* m_fadeSR = nullptr;			//�t�F�[�h�C���A�A�E�g�Ŏg���X�v���C�g�����_���[
	float m_test = 0.0f;
	bool m_flagRotationCamera = true;			//�J�������X�e�[�W�̎������]���Ă��邩�̃t���O
	bool m_wipeEndFlag = false;					//���C�v���I������H
	float m_startTimer = 0;						//���o�J�n�܂ł̃^�C�}�[
	CSpringCamera m_springCamera;				//�΂˃J����


public:
	/// <summary>
	/// ���C�v���I���������ݒ�
	/// </summary>
	/// <param name="wipeEndFlag">���C�v���I������H</param>
	void SetWipeEndFlag(const bool wipeEndFlag)
	{
		m_wipeEndFlag = wipeEndFlag;
	}
};

