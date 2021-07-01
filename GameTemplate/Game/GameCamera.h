#pragma once
#include "GameCameraConstData.h"
#include "SpriteRender.h"
#include "Player.h"
#include "StartDirecting.h"
#include "SpringCamera.h"

/**
 * @brief �Q�[�����J�����N���X
*/
class CGameCamera : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;		//�X�^�[�g�֐�
	void Update()override final;	//�A�b�v�f�[�g�֐�

private:	//private�����o�֐�

	/**
	 * @brief �Q�[�����̃J����
	*/
	void InGameCamera();


public:		//�����o�֐�

	/// <summary>
	/// �J�����̒����_��ݒ肷��
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const Vector3& target)
	{
		m_springCamera.SetTarget(target);
	}

	/// <summary>
	/// �J�����̎��_��ݒ肷��
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_springCamera.SetPosition(pos);
	}

	/// <summary>
	/// �J�����̎��_���擾����
	/// </summary>
	/// <returns>�J�����̎��_</returns>
	const Vector3& GetPosition() const
	{
		return m_springCamera.GetPosition();
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

	/// <summary>
	/// ���t���b�V��
	/// ���̊֐����Ăяo���ƁA�o�l�̈ړ������t���b�V������āAUpdate�֐����Ăяo���ƃJ�����̈ʒu��
	/// �����ɖڕW���_�ƖڕW�����_�Ɉړ����܂��B�V�[�����؂�ւ���ăJ��������C�ɐ؂�ւ������Ƃ��Ȃǂ�
	/// �g�p���Ă��������B
	/// </summary>
	void Refresh()
	{
		m_springCamera.Refresh();
	}

	/// <summary>
	/// ���C�v���I���������ݒ�
	/// </summary>
	/// <param name="wipeEndFlag">���C�v���I������H</param>
	void SetWipeEndFlag(const bool wipeEndFlag)
	{
		m_startDirecting.SetWipeEndFlag(wipeEndFlag);
	}


private:	//�f�[�^�����o

	bool m_lookPlayerFlag = true;				//�v���C���[�����邩�H
	bool m_titleMode = false;					//�^�C�g����ʂ��H

	CSpringCamera m_springCamera;				//�΂˃J����
	CStartDirecting m_startDirecting;			//�J�n���o�N���X
	Player* m_player = nullptr;					//�v���C���[�̃|�C���^

};

