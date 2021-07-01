#pragma once
#include <time.h>
//#include "GameTime.h"

/// <summary>
/// ��ʓ]���N���X
/// </summary>
class CSceneChange
{
public:
	~CSceneChange();				//�f�X�g���N�^

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
		SetInOrOut(enIn);
		//���C�v�J�n
		WipeStart();
	}

	/// <summary>
	/// ���C�v�A�E�g���J�n����
	/// </summary>
	void WipeOut()
	{
		//���C�v�A�E�g�ɐݒ肵��
		SetInOrOut(enOut);
		//���C�v�J�n
		WipeStart();
	}

	/// <summary>
	/// ���C�v�̎�ނ������_���ɂ��āA���C�v�A�E�g�J�n
	/// </summary>
	void RandomWipeOut();

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
	/// ���C�v�I��
	/// </summary>
	void WipeEnd()
	{
		//���C�v���J�n�ʒu�ɖ߂�
		m_wipeParam.wipeSize = 0.0f;
		//���C�v�����C�v�A�E�g�ɂ��Ă���
		//�������Ȃ��Ń��C�v���J�n�ʒu�ɖ߂��ƁA���Ԃ�^���ÂɂȂ�
		SetInOrOut(enOut);
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
	/// CSceneChange::enLeft��CSceneChange::enRight�������ɂ���
	/// </summary>
	/// <param name="leftOrLight">�E��������</param>
	void SetWipeLeftOrLight(const bool leftOrLight)
	{
		m_wipeParam.leftOtLight = leftOrLight;
	}

	/// <summary>
	/// �㑤���烏�C�v���邩�A�������烏�C�v���邩�ݒ肷��
	/// CSceneChange::enTop��CSceneChange::enDown�������ɂ���
	/// </summary>
	/// <param name="topOrDown">�㑤������</param>
	void SetWipeTopOrDown(const bool topOrDown)
	{
		m_wipeParam.topOrDown = topOrDown;
	}

	/// <summary>
	/// ���C�v�̎�ނ�ݒ肷��
	/// ���C�v�̎�ނ�SceneChange.h���Q��
	/// </summary>
	/// <param name="wipeType"></param>
	void SetWipeType(const int wipeType)
	{
		m_wipeParam.wipeType = wipeType;
	}

	/// <summary>
	/// ���C�v�C�������C�v�A�E�g��ݒ�
	/// CSceneChange::enIn��CSceneChange::enOut�������ɂ���
	/// </summary>
	/// <param name="inOrOut"></param>
	void SetInOrOut(const bool inOrOut)
	{
		m_wipeParam.inOrOut = inOrOut;
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
	/// ���C�v�̎�ނ�SceneChange.h���Q��
	/// </summary>
	/// <param name="wipeType">���C�v�̎��</param>
	/// <returns>�e��ނ̃��C�v�̍ő�T�C�Y</returns>
	const float GetWipeSizeMax(const int wipeType) const;

	/// <summary>
	/// ���݂̃��C�v�̎�ނ𓾂�
	/// ���C�v�̎�ނ�SceneChange.h���Q��
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

public:		//�񋓌^

	/// <summary>
	/// �����E��
	/// </summary>
	enum EnLeftOrLight
	{
		enLeft,		//��
		enRight,	//�E
	};

	/// <summary>
	/// �ォ����
	/// </summary>
	enum EnTopOrDown
	{
		enTop,		//��
		enDown,		//��
	};

	/// <summary>
	/// ���C�v�C�������C�v�A�E�g��
	/// </summary>
	enum EnInOrOut
	{
		enIn,		//���C�v�C��
		enOut,		//���C�v�A�E�g
	};

	/// <summary>
	/// ���C�v�̎��
	/// </summary>
	enum EnWipeType
	{
		enWipe,					//���ʂ̃��C�v
		enCircleWipe,			//�~�`���C�v
		enVerticalStripeWipe,	//�c�ȃ��C�v
		enHorizontalStripeWipe,	//���ȃ��C�v
		enCheckerboardWipe,		//�`�F�b�J�[�{�[�h���C�v	//�����܂ł����������Ă��Ȃ�
		enMonochrome,			//���m�N�����H
		enSepia,				//�Z�s�A�����H
		enNega,					//�l�K�|�W���]
		enNois,					//�m�C�Y���H
	};

private:	//�\����

	/// <summary>
	/// ���C�v�̃p�����[�^�\����
	/// ������ύX������ASceneChange.fx�̒萔�o�b�t�@���ύX���邱��
	/// </summary>
	struct WipeParam
	{
		Vector2 wipeDir = { -1.0f,1.0f };
		float wipeSize = 0.0f;
		int leftOtLight = enLeft;
		int topOrDown = enTop;
		int wipeType = enWipe;
		int inOrOut = enOut;
	};
private:	//�萔
	//�e���C�v�̍ő像�C�v�T�C�Y
	//������ύX������ASceneChange.fx�̒萔���ύX���邱��
	const float m_wipeMaxX = 1280.0f;				//���ʂ̃��C�v��X���̍ő�T�C�Y
	const float m_wipeMaxY = 720.f;					//���ʂ̃��C�v��Y���̍ő�T�C�Y
	const float m_circleWipeMax = 1280.0f / 1.5;	//�~�`���C�v�̍ő�T�C�Y
	const float m_verticalStripeWipeMax = 64.0f;	//�c�ȃ��C�v�̍ő�T�C�Y
	const float m_horizontalStripeWipeMax = 64.0f;	//���ȃ��C�v�̍ő�T�C�Y
	const float m_checkerboardWipeMax = 128.0f;		//�`�F�b�J�[�{�[�h���C�v�̍ő�T�C�Y

private:	//�f�[�^�����o
	RenderTarget m_renderTarget;	//�����_�����O�^�[�Q�b�g
	Sprite m_sprite;				//�X�v���C�g
	Sprite m_finalSprite;			//�ŏI�I�ȃX�v���C�g
	Sprite m_backSprite;			//���ɉf���X�v���C�g
	WipeParam m_wipeParam;			//���C�v�̃p�����[�^
	bool m_wipeFlag = false;		//���C�v���邩�H
	float m_wipeSpeed = 120.0f;		//���C�v�̃X�s�[�h
	float m_deltaTime = 0.0f;		//�f���^�^�C��
	Sprite m_fadeSprite;			//�t�F�[�h�C���A�A�E�g�Ŏg���X�v���C�g�����_���[
};