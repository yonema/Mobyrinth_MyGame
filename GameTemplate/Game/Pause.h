#pragma once
#include "FontRender.h"
#include "SpriteRender.h"
#include "Level2D.h"
#include "SoundCue.h"

/// <summary>
/// �|�[�Y�N���X
/// </summary>
class CPause : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;		//�X�^�[�g�֐�
	~CPause();						//�f�X�g���N�^

	/// <summary>
	/// �|�[�Y���ł��Ă΂��A�b�v�f�[�g�֐�
	/// �|�[�Y���łȂ��Ă���ɌĂ΂��
	/// </summary>
	void UpdateWhenPaused()override final;

	/// <summary>
	/// �|�[�Y���ɂ����Ă΂��A�b�v�f�[�g�֐�
	/// </summary>
	void UpdateOnlyPaused()override final;

public:		//�����̃����o�֐�����Ɏg��
	/// <summary>
	/// ���g���C���I�����ꂽ���ǂ�����߂��֐�
	/// ���g���C���I�����ꂽ��true��߂��B
	/// </summary>
	/// <returns>���g���C���I�����ꂽ���H</returns>
	const bool GetRetryFlag()const
	{
		return m_retryFlag;
	}

	/// <summary>
	/// �I�����I�����ꂽ���ǂ�����߂��֐��B
	/// �I�����I�����ꂽ��true��߂��B
	/// </summary>
	/// <returns></returns>
	const bool GetQuitFlag()const
	{
		return m_quitFlag;
	}

	/// <summary>
	/// �|�[�Y�ł��邩�ł��Ȃ�����ݒ肷��֐�
	/// false��ݒ肵����|�[�Y�ł��Ȃ�
	/// </summary>
	/// <param name="b">�|�[�Y�ł��邩�H</param>
	void SetCanPause(const bool b)
	{
		m_canPause = b;
	}

private:	//private�ȃ����o�֐�
	/// <summary>
	/// �|�[�Y��Ԃɂ���֐�
	/// </summary>
	void ToPause();

	/// <summary>
	/// �|�[�Y��Ԃ����������֐�
	/// </summary>
	void UnPause();

	/// <summary>
	///	�|�[�Y���ɃJ�[�\���I��������֐�
	/// </summary>
	void Choose();

	/// <summary>
	/// �I�������J�[�\���Ō��肷��֐�
	/// </summary>
	void Decision();


private:	//�f�[�^�����o

	/// <summary>
	///	�t�H���g�Ɏg�������̎�ނ�
	/// �|�[�Y���̃X�e�[�g�i��ԁj
	/// </summary>
	enum ENPauseState
	{
		enContinue,			//������A��I���������
		enRetry,			//���g���C�A��I���������
		enQuit,				//�I���A��I���������
		enPauseNum		//�X�e�[�g�̐��B�t�H���g�݂̂Ŏg�p
	};


	//CFontRender* m_fontRender[enPauseStateNum] = { nullptr };	//�t�H���g�����_���[
	//CFontRender* m_arrowFR = nullptr;							//�t�H���g�����_���[

	int m_pauseState = enContinue;		//���݂̃|�[�Y���̃X�e�[�g�i��ԁj
	bool m_isPaused = false;			//�|�[�Y�����ǂ����H
	bool m_buttonFlag = true;			//�{�^������͂ł��邩�ǂ����H�i�A�����͖h�~�p�j
	bool m_retryFlag = false;			//���g���C���I������Ă��邩�H
	bool m_quitFlag = false;			//�I�����I������Ă��邩�H
	bool m_canPause = false;			//�|�[�Y�ł��邩�H
	bool m_flagOperation = true;		//�|�[�Y��ʂ̑��삪�\���H



	/*enum EnPauseList
	{
		enContinue,
		enRetry,
		enQuit,
		enPauseNum,
	};*/

	CLevel2D m_level2D;
	CSpriteRender* m_pause = nullptr;
	CSpriteRender* m_choices[enPauseNum] = { nullptr };
	CSpriteRender* m_cursor = nullptr;
	CSpriteRender* m_background = nullptr;

	CSoundCue* m_selectSE = nullptr;	//selectSE�̃T�E���h�L���[
	CSoundCue* m_buttonASE = nullptr;	//buttonASE�̃T�E���h�L���[
	CSoundCue* m_buttonBSE = nullptr;	//buttonBSE�̃T�E���h�L���[
};

