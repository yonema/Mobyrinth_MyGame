#pragma once
#include <time.h>
#include "HUDConstData.h"
#include "SpriteRenderConstData.h"

/// <summary>
/// ��ʓ]���N���X
/// </summary>
class CHUD
{
public:		//�����ŌĂ΂�郁���o�֐�
	CHUD();			//�R���X�g���N�^
	~CHUD() {};		//�f�X�g���N�^

public:		//�O���t�B�b�N�G���W���ŌĂ΂�郁���o�֐�
	void Init();					//�������֐�
	void UpdateParam();				//�p�����[�^�̃A�b�v�f�[�g�֐�
	void Draw(RenderContext& rc);	//�`��֐�

public:		//�����o�֐�

	/// <summary>
	/// ���C�v�C�����J�n����
	/// </summary>
	void WipeIn()
	{
		//���C�v�C���ɐݒ肵��
		SetInOrOut(EN_IN);
		//���C�v�J�n
		WipeStart();
	}

	/// <summary>
	/// ���C�v�A�E�g���J�n����
	/// </summary>
	void WipeOut()
	{
		//���C�v�A�E�g�ɐݒ肵��
		SetInOrOut(EN_OUT);
		//���C�v�J�n
		WipeStart();
	}

	/// <summary>
	/// ���C�v�̎�ނ������_���ɂ��āA���C�v�A�E�g�J�n
	/// </summary>
	void RandomWipeOut();


	/// <summary>
	/// ���C�v�I��
	/// </summary>
	void WipeEnd()
	{
		//���C�v���J�n�ʒu�ɖ߂�
		m_wipeParam.wipeSize = 0.0f;
		//���C�v�����C�v�A�E�g�ɂ��Ă���
		//�������Ȃ��Ń��C�v���J�n�ʒu�ɖ߂��ƁA���Ԃ�^���ÂɂȂ�
		SetInOrOut(EN_OUT);
		//���C�v�t���O��܂�
		m_wipeFlag = false;
	}

	/// <summary>
	/// ���C�v���������ݒ肷��
	/// leftOrLight�Ɩ�������悤�ɐݒ肵�Ȃ��łˁB
	/// </summary>
	/// <param name="dir">���C�v�������</param>
	void SetWipeDirection(const Vector2& dir);

	/// <summary>
	/// �������烏�C�v���邩�A�E�����烏�C�v���邩�ݒ肷��
	/// WipeDirection�Ɩ�������悤�ɐݒ肵�Ȃ��łˁB
	/// </summary>
	/// <param name="leftOrLight">�E��������</param>
	void SetWipeLeftOrLight(const bool leftOrLight)
	{
		m_wipeParam.leftOtLight = leftOrLight;
	}

	/// <summary>
	/// �㑤���烏�C�v���邩�A�������烏�C�v���邩�ݒ肷��
	/// </summary>
	/// <param name="topOrDown">�㑤������</param>
	void SetWipeTopOrDown(const bool topOrDown)
	{
		m_wipeParam.topOrDown = topOrDown;
	}

	/// <summary>
	/// ���C�v�̎�ނ�ݒ肷��
	/// </summary>
	/// <param name="wipeType"></param>
	void SetWipeType(const int wipeType)
	{
		m_wipeParam.wipeType = wipeType;
	}


	/// <summary>
	/// ���C�v���鑬�x��ݒ肷��
	/// </summary>
	/// <param name="wipeSpeed">���C�v���鑬�x</param>
	void SetWipeSpeed(const float wipeSpeed)
	{
		m_wipeSpeed = wipeSpeed;
	}

	/// <summary>
	/// �ǂꂭ�炢���C�v���Ă��邩�𓾂�
	/// </summary>
	/// <returns>�ǂꂭ�炢���C�v���Ă��邩</returns>
	const float GetWipeSize() const
	{
		return m_wipeParam.wipeSize;
	}

	/// <summary>
	/// �e��ނ̃��C�v�̍ő�T�C�Y�𓾂�
	/// </summary>
	/// <param name="wipeType">���C�v�̎��</param>
	/// <returns>�e��ނ̃��C�v�̍ő�T�C�Y</returns>
	const float GetWipeSizeMax(const int wipeType) const;

	/// <summary>
	/// ���݂̃��C�v�̎�ނ𓾂�
	/// </summary>
	/// <returns>���݂̃��C�v�̎��</returns>
	const int GetWipeType() const
	{
		return m_wipeParam.wipeType;
	}

	/// <summary>
	/// ���C�v���Ō�܂Ń��C�v�������𓾂�
	/// </summary>
	/// <returns>�Ō�܂Ń��C�v�������H</returns>
	const bool IsWipeFinished() const;

	/**
	 * @brief �t�F�[�h�̃X�v���C�g�̃A���t�@�l��ݒ肷��
	 * @param [in] alphaValue �A���t�@�l
	*/
	void SetFadeSpriteAlphaValue(const float alphaValue)
	{
		m_fadeSprite.SetAlphaValue(alphaValue);
	}


private:	//private�ȃ����o�֐�

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̏�����
	/// </summary>
	void InitRenderTarget();

	/// <summary>
	/// �X�v���C�g�̏�����
	/// </summary>
	void InitSprite();

	/// <summary>
	/// �t�F�[�h�̏�����
	/// </summary>
	void InitFade();

	/// <summary>
	/// ���C�v�J�n
	/// </summary>
	void WipeStart()
	{
		//���C�v���J�n�ʒu�ɂ���
		m_wipeParam.wipeSize = 0.0f;
		//���C�v�t���O�𗧂Ă�
		m_wipeFlag = true;
	};

	/// <summary>
	/// ���C�v�C�������C�v�A�E�g��ݒ�
	/// </summary>
	/// <param name="inOrOut"></param>
	void SetInOrOut(const bool inOrOut)
	{
		m_wipeParam.inOrOut = inOrOut;
	};


private:	//�f�[�^�����o
	RenderTarget m_beforeWipingRenderTarget;	//���C�v�O�̃����_�����O�^�[�Q�b�g
	Sprite m_mainRenderTargetSprite;			//���C�������_�����O�^�[�Q�b�g�̃X�v���C�g
	Sprite m_afterWipingSprite;					//���C�v������̃X�v���C�g
	Sprite m_backSprite;						//���C�v�̌��ɉf���X�v���C�g
	Sprite m_fadeSprite;						//�t�F�[�h�C���A�A�E�g�Ŏg���X�v���C�g�����_���[
	HUDConstData::SWipeParam m_wipeParam;		//���C�v�̃p�����[�^
	bool m_wipeFlag = false;					//���C�v���邩�H
	float m_wipeSpeed = 0.0f;					//���C�v�̃X�s�[�h
	float m_deltaTime = 0.0f;					//�f���^�^�C��

};