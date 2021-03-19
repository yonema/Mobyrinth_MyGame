#include "stdafx.h"
#include "Title.h"


//�X�^�[�g�֐�
bool Title::Start()
{
	//�t�H���g�����_���[�̐����Ə�����
	m_titleFR = NewGO<CFontRender>(0);
	m_titleFR->Init(L"���r�����X",
		{ -200.0f,300.0f },
		{ 0.5f,0.5f,1.0f,1.0f },
		0.0f,
		3.0f
	);
	m_pushAButtonFR = NewGO<CFontRender>(0);
	m_pushAButtonFR->Init(L"A�{�^���������ĂˁB",
		{ -150.0f,0.0f },
		{ 1.0f,0.0f,0.0f,1.0f }
	);
	m_title = NewGO<CSpriteRender>(0);
	m_title->Init("Assets/sprite/Title.dds", 1400.0f, 600.0f);

	//�t�H���g�̔z�u
	const float leftSide = -100.0f;		//���[
	const float UpSide = 300.0f;		//��[
	const float DownSide = -300.0f;		//���[
	const float BetweenLine = (DownSide - UpSide) / enStageNum;	//�t�H���g�̔z�u�̕�

	//�t�H���g�����_���[�̐����Ə�����
	for (int i = 0; i < enStageNum; i++)
	{
		m_stageName[i] = NewGO<CFontRender>(0);
	}
	m_stageName[enStage_kari]->Init(L"stage_kari",
		{ leftSide ,UpSide + BetweenLine * enStage_kari }
	);
	m_stageName[enStageProto01]->Init(L"stage_proto01",
		{ leftSide ,UpSide + BetweenLine * enStageProto01 }
	);
	m_stageName[enStageProto02]->Init(L"stage_proto02",
		{ leftSide ,UpSide + BetweenLine * enStageProto02 }
	);

	//�ŏ��̓X�e�[�W���͕\�����Ȃ�����A���������Ĕ�\���ɂ���
	for (int i = 0; i < enStageNum; i++)
	{
		m_stageName[i]->Deactivate();
	}
	m_arrow = NewGO<CFontRender>(0);
	m_arrow->Init(L"->",
		{ leftSide - 50.0f,UpSide - 5.0f }
	);
	m_arrow->Deactivate();


	//�X�v���C�g�̃��x����������
	m_level2D.Init("Assets/level2D/test2d.casl", [&](Level2DObjectData& objdata)
		{
			//���O����v�Ńt�b�N����
			if (objdata.EqualObjectName("title"))
			{
				//�t�b�N������true��߂�
				return true;
			}
			//���̂܂ܕ\�����邩��false��߂�
			return false;
		});


	return true;
}

//�f�X�g���N�^
Title::~Title()
{
	DeleteGO(m_titleFR);
	DeleteGO(m_pushAButtonFR);
	for (int i = 0; i < enStageNum; i++)
	{
		DeleteGO(m_stageName[i]);
	}
	DeleteGO(m_arrow);

}

//�A�b�v�f�[�g�֐�
void Title::Update()
{
	//���݂̃X�e�[�W�̃X�e�[�g�i��ԁj�ŏ�����U�蕪����
	switch (m_stageState)
	{
	case enTitleScreen:
		//�^�C�g�����
		TitleScreen();
		break;
	case enStageSelect:
		//�X�e�[�W�Z���N�g
		StageSelect();
		break;
	default:
		break;
	}
}


//�^�C�g�����
void Title::TitleScreen()
{
	//�{�^���̓��͂𒲂ׂ�
	if (g_pad[0]->GetLStickYF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//�������͂��Ȃ����
		//�{�^�����������Ƃ��ł���悤�ɂ���i�A�����͖h�~�p�j
		m_buttonFlag = true;
	}
	else if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//A�{�^�������
		//�{�^�����������Ƃ��ł��Ȃ��悤�ɂ���i�A�����͖h�~�p�j
		m_buttonFlag = false;
		//�X�e�[�W�̃X�e�[�g�i��ԁj���X�e�[�W�Z���N�g�Ɉڍs����B
		m_stageState = enStageSelect;

		//�^�C�g����ʗp�̃t�H���g�����_���[�𖳌������Ĕ�\���ɂ���
		m_titleFR->Deactivate();
		m_pushAButtonFR->Deactivate();

		//�X�e�[�W�Z���N�g�p�̃t�H���g�����_���[��L�������ĕ\���ł���悤�ɂ���
		for (int i = 0; i < enStageNum; i++)
		{
			m_stageName[i]->Activate();
		}
		m_arrow->Activate();
	}

}

//�X�e�[�W�Z���N�g
void Title::StageSelect()
{
	//�{�^���̓��͂𒲂ׂ�
	if (g_pad[0]->GetLStickYF() == 0.0f && !g_pad[0]->IsPressAnyKey())
	{
		//�������͂��Ȃ����
		//�{�^�����������Ƃ��ł���悤�ɂ���i�A�����͖h�~�p�j
		m_buttonFlag = true;
	}
	if (g_pad[0]->GetLStickYF() < 0.0f && m_buttonFlag)
	{
		//�������
		//�{�^�����������Ƃ��ł��Ȃ��悤�ɂ���i�A�����͖h�~�p�j
		m_buttonFlag = false;

		//�X�e�[�W�Z���N�g�̃X�e�[�g�����Z����
		m_stageSelectState++;
		//�X�e�[�g���ő�̒l�ɂȂ�����A������傫���Ȃ�Ȃ��悤�ɂ���
		if (m_stageSelectState > enStageProto02)
		{
			m_stageSelectState = enStageProto02;
		}
	}
	else if (g_pad[0]->GetLStickYF() > 0.0f && m_buttonFlag)
	{
		//������
		//�{�^�����������Ƃ��ł��Ȃ��悤�ɂ���i�A�����͖h�~�p�j
		m_buttonFlag = false;

		//�X�e�[�W�Z���N�g�̃X�e�[�g�����Z����
		m_stageSelectState--;
		//�X�e�[�g���ŏ��̒l�ɂȂ�����A�����菬�����Ȃ�Ȃ��悤�ɂ���
		if (m_stageSelectState <= enStage_kari)
		{
			m_stageSelectState = enStage_kari;
		}
	}
	else if (g_pad[0]->IsTrigger(enButtonA) && m_buttonFlag)
	{
		//A�{�^�������
		//�{�^�����������Ƃ��ł��Ȃ��悤�ɂ���i�A�����͖h�~�p�j
		m_buttonFlag = false;

		////////////////////////////////////////////////////////////
		//�X�e�[�W��V�����쐬�����ꍇ�A������NewGO���s���B
		////////////////////////////////////////////////////////////
		//�X�e�[�W�Z���N�g�̃X�e�[�g�ɂ���Đ�������X�e�[�W��U�蕪����
		switch (m_stageSelectState)
		{
		case enStage_kari:
			NewGO<stage_kari>(0, "stage_kari");
			break;
		case enStageProto01:
			NewGO<stage_proto01>(0, "stage_proto01");
			break;
		case enStageProto02:
			NewGO<stage_proto02>(0, "stage_proto02");
			break;
		default:
			break;
		}
		//���g�̃I�u�W�F�N�g��j������
		Release();
	}
	else if (g_pad[0]->IsTrigger(enButtonB) && m_buttonFlag)
	{
		//B�{�^�������
		//�{�^�����������Ƃ��ł��Ȃ��悤�ɂ���i�A�����͖h�~�p�j
		m_buttonFlag = false;

		//�X�e�[�W�̃X�e�[�g���^�C�g����ʂɂ���
		m_stageState = enTitleScreen;
		//�^�C�g����ʗp�̃t�H���g�����_���[��L�������ĕ\���ł���悤�ɂ���
		m_titleFR->Activate();
		m_pushAButtonFR->Activate();
		//�X�e�[�W�Z���N�g�̃t�H���g�����_���[�𖳌������ĕ\���ł��Ȃ��悤�ɂ���
		for (int i = 0; i < enStageNum; i++)
		{
			m_stageName[i]->Deactivate();
		}
		m_arrow->Deactivate();
	}


	//�t�H���g�̔z�u
	const float leftSide = -100.0f;		//���[
	const float UpSide = 300.0f;		//��[
	const float DownSide = -300.0f;		//���[
	const float BetweenLine = (DownSide - UpSide) / enStageNum;	//�t�H���g�̔z�u�̕�
	//�J�[�\���p�̃t�H���g�����_���[�̏ꏊ��ݒ肷��
	m_arrow->SetPosition({ leftSide - 50.0f , UpSide + BetweenLine * m_stageSelectState - 5.0f });
}