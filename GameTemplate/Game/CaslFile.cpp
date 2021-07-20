#include "stdafx.h"
#include "CaslFile.h"


/// <summary>
/// casl�t�@�C����ǂݍ���
/// </summary>
/// <param name="filePath">casl�t�@�C���p�X</param>
void CaslFile::Load(const char* filePath)
{
	//casl�t�@�C�����o�C�i���ǂݍ��݃��[�h�ŊJ��
	FILE* fp = fopen(filePath, "rb");

	//�t�@�C�����ǂݍ��߂Ȃ�������
	if (fp == nullptr)
	{
		//�G���[���b�Z�[�W���o��
		MessageBoxA(nullptr, "casl�t�@�C���̃I�[�v���Ɏ��s���܂����B", "�G���[", MB_OK);
		return;
	}


	std::string ddsFolderPath = filePath;
	//�Ō�Ɍ���� "/" �����������A���̈ʒu��߂�
	int pos = (int)ddsFolderPath.rfind("/");
	//0�Ԗڂ���Apos + 1 �v�f�̕������߂�
	ddsFolderPath = ddsFolderPath.substr(0, pos + 1);
	int ddsFolderPathCount = ddsFolderPath.length();

	//�摜�̐����擾�B
	int numLevel = ReadInteger(fp);
	for (int i = 0; i < numLevel; i++)
	{

		auto caslData = std::make_unique<SCaslData>();
		//std::unique_ptr<Level2DObject> levelObject;
		//���x���t�@�C���ɕۑ����Ă���̂́B
		//�ȉ��S��char�^�ŕۑ����Ă���A����(,)�ŋ�؂肪���Ă���B
		//���O�̒���(int)�A���O(char)�B
		//�t�@�C���p�X�̒���(int)�A�t�@�C���p�X(char)�B
		//���Wx,y(float)�B
		//�摜�̑傫��x,y(int)�B
		//���C���[�D��xx,y(int)�B
		//�X�P�[���{��x,y(float)�B
		//.dds�t�@�C���̖��O�̒���(int)�Adds�t�@�C���̖��O�B

		//�摜�̖��O�̒������擾�B
		int nameCount = ReadInteger(fp);
		//�摜�̖��O���擾�B
		caslData.get()->name = std::make_unique<char[]>(nameCount + 1);
		fread(caslData.get()->name.get(), nameCount, 1, fp);
		ReadOnlyOneCharacter(fp);

		//�t�@�C���p�X�̖��O�̒������擾�B
		int fileNameCount = ReadInteger(fp);

		//�t�@�C���p�X���擾�B
		caslData.get()->fileName = std::make_unique<char[]>(fileNameCount + 1);
		fread(caslData.get()->fileName.get(), fileNameCount, 1, fp);
		ReadOnlyOneCharacter(fp);

		//���W���擾�B
		caslData.get()->position.x = ReadDecimal(fp);
		caslData.get()->position.y = ReadDecimal(fp);

		//���ƍ������擾�B
		caslData.get()->width = ReadInteger(fp);
		caslData.get()->height = ReadInteger(fp);

		//���C���[�D��x���擾�B
		caslData.get()->numberLayer = ReadInteger(fp);

		//�傫���̔{�����擾�B
		caslData.get()->scale.x = ReadDecimal(fp);
		caslData.get()->scale.y = ReadDecimal(fp);

		//dds�t�@�C���p�X�̖��O�̒������擾�B
		int ddsFileNameCount = ReadInteger(fp);
		//dds�t�@�C���p�X���擾�B
		caslData.get()->ddsFileName = std::make_unique<char[]>(ddsFileNameCount + 1);
		fread(caslData.get()->ddsFileName.get(), ddsFileNameCount, 1, fp);

		//.dds�t�@�C���p�X��ݒ肷��B
		caslData.get()->ddsFilePath = std::make_unique<char[]>(ddsFolderPathCount + ddsFileNameCount + 1);
		//�t�H���_�p�X��.dds�t�@�C���̖��O��A������B
		std::string ddsFilePath = ddsFolderPath + caslData.get()->ddsFileName.get();
		std::char_traits<char>::copy(caslData.get()->ddsFilePath.get(), ddsFilePath.c_str(), ddsFilePath.size() + 1);

		//ReadOnlyOneCharacter(fp);
		//���s�R�[�h��ǂݔ�΂��B
		char dummy[256];
		fgets(dummy, 256, fp);

		m_caslDataList.push_back(std::move(caslData));
	}
	//fopen������fclose�ŕ���B
	fclose(fp);
}

/// <summary>
/// 1���������ǂݍ��ށB�u , �v��u \n �v��ǂݍ��ނ̂Ɏg��
/// </summary>
/// <param name="file">�t�@�C��</param>
void CaslFile::ReadOnlyOneCharacter(FILE* file) const
{
	char a;
	fread(&a, 1, 1, file);
}


/// <summary>
/// int�^��ǂݍ���
/// </summary>
/// <param name="file">�t�@�C��</param>
/// <returns>int�^�̒l</returns>
int CaslFile::ReadInteger(FILE* file) const
{
	std::string number;
	while (true)
	{
		char c;
		fread(&c, 1, 1, file);
		if (c != ',' && c != '\n')
		{
			number += c;
		}
		else {
			break;
		}
	}
	//string�^��int�^�ɕϊ�����B
	return atoi(number.c_str());
}


/// <summary>
/// float�^��ǂݍ���
/// </summary>
/// <param name="file">�t�@�C��</param>
/// <returns>float�^�̒l</returns>
float CaslFile::ReadDecimal(FILE* file) const
{
	std::string number;
	while (true)
	{
		char c;
		fread(&c, 1, 1, file);
		if (c != ',' && c != '\n')
		{
			number += c;
		}
		else {
			break;
		}
	}
	//string�^��float�^�ɕϊ�����B
	return atof(number.c_str());
}