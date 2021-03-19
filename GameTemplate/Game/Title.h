#pragma once
#include "FontRender.h"
#include "SpriteRender.h"
#include "Level2D.h"

//�X�e�[�W
#include "stage_kari.h"
#include "stage_proto01.h"
#include "stage_proto02.h"

/// <summary>
/// �^�C�g���N���X
/// </summary>
class Title : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	bool Start()override final;		//�X�^�[�g�֐�
	~Title();						//�f�X�g���N�^
	void Update()override final;	//�A�b�v�f�[�g�֐�

private:	//private�ȃ����o�֐�

	void TitleScreen();	//�^�C�g�����
	void StageSelect();	//�X�e�[�W�Z���N�g
	void Release()		//���g�̃I�u�W�F�N�g��j������֐�
	{
		DeleteGO(this);
	}

private:	//�f�[�^�����o

	/// <summary>
	/// �X�e�[�W�̃X�e�[�g�i��ԁj
	/// </summary>
	enum EnState
	{
		enTitleScreen,	//�^�C�g����ʂ̏��
		enStageSelect,	//�X�e�[�W�Z���N�g�̏��
	};
	int m_stageState = enTitleScreen;	//���݂̃X�e�[�W�̃X�e�[�g�i��ԁj

	/// <summary>
	/// �X�e�[�W�̔ԍ�
	/// �V�����X�e�[�W��������ꍇ�A�����ɔԍ���ǉ�
	/// </summary>
	enum EnStageList
	{
		enStage_kari,
		enStageProto01,
		enStageProto02,
		enStageNum,
	};

	int m_stageSelectState = enStage_kari;	//���݂̃X�e�[�W�Z���N�g�̃X�e�[�g

	CFontRender* m_titleFR = nullptr;					//�t�H���g�����_�[
	CFontRender* m_pushAButtonFR = nullptr;				//�t�H���g�����_�[
	CFontRender* m_stageName[enStageNum] = { nullptr };	//�t�H���g�����_�[
	CFontRender* m_arrow = nullptr;						//�t�H���g�����_�[
	bool m_buttonFlag = true;							//�{�^�����������Ƃ��ł��邩�H
														//�i�A�����͖h�~�p�j
	CLevel2D m_level2D;									//�X�v���C�g�p�̃��x���N���X
	std::list<CSpriteRender*> m_spriteRenders;
	CSpriteRender* m_title = nullptr;
	
};

