#include "stdafx.h"
#include "Stage.h"

/// <summary>
/// �������֐��ANewGO������ŕK�����łˁB
/// </summary>
/// <param name="levelFilePath">���x���̃t�@�C���p�X</param>
void CStage::Init(const char* levelFilePath)
{
	sprintf(m_levelFilePath, levelFilePath);
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
