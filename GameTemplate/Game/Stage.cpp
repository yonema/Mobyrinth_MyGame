#include "stdafx.h"
#include "Stage.h"

/// <summary>
/// �������֐��ANewGO������ŕK�����łˁB
/// </summary>
/// <param name="levelFilePath">���x���̃t�@�C���p�X</param>
void CStage::Init(const char* levelFilePath, const int stageNum)
{
	sprintf(m_levelFilePath, levelFilePath);
	SetStageNum(stageNum);
}

//�X�^�[�g�֐�
bool CStage::StartSub()
{
	//���x���̃��[�h
	LoadLevel(m_levelFilePath);

	return true;
}

//�f�X�g���N�^
CStage::~CStage()
{

}
