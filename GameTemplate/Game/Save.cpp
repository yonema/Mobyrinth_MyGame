#include "stdafx.h"
#include "Save.h"

//�R���X�g���N�^
CSave::CSave()
{
	//���[�h����
	LoadData();
}

/// <summary>
/// �f�[�^�����o�̃Z�[�u�f�[�^���Z�[�u����
/// </summary>
void CSave::SaveData()
{
	//�t�@�C�����o�C�i���ŏ㏑�����[�h�ŊJ��
	auto fp = fopen(SAVE_DATA_FILEPATH, MODE_WRITE_BINARY);

	//�Z�[�u�f�[�^�̈�ԍ����N���A�����X�e�[�W�̔ԍ����t�@�C���ɏ�������
	fwrite(
		&m_saveData.highestClearStageNum,
		sizeof(m_saveData.highestClearStageNum),
		1,
		fp
	);

	//�t�@�C�������
	fclose(fp);

}

/// <summary>
/// �Z�[�u�f�[�^���f�[�^�����o�Ƀ��[�h����
/// </summary>
/// <returns>���[�h�ł������H</returns>
const bool CSave::LoadData()
{
	//�t�@�C�����o�C�i���œǂݍ��݃��[�h�ŊJ��
	auto fp = fopen(SAVE_DATA_FILEPATH, MODE_READ_BINARY);

	//�J�������H
	if (!fp)
	{
		//�J���Ă��Ȃ�������
		//�G���[�R�[�h�i���̐��j������
		m_saveData.highestClearStageNum = -1;
		//���[�h�ł��Ȃ���������false��߂�
		return false;
	}
	//�J����

	//��ԍ����N���A�����X�e�[�W�̔ԍ��̃o�b�t�@�[
	int highestClearStageBuf;

	//��ԍ����N���A�����X�e�[�W�̔ԍ����t�@�C������ǂݍ���
	fread(&highestClearStageBuf, sizeof(highestClearStageBuf), 1, fp);

	//�f�[�^�����o�ɑ������
	m_saveData.highestClearStageNum = highestClearStageBuf;

	//�t�@�C�������
	fclose(fp);

	//���[�h�ł�������
	//true��߂�
	return true;
}