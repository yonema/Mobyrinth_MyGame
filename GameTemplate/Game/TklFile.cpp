#include "stdafx.h"
#include "TklFile.h"

void TklFile::Load(const char* filePath)
{
	auto fp = fopen(filePath, "rb");
	if (fp == nullptr) {
		return;
	}
	int version;
	//�o�[�W�������擾
	fread(&version, sizeof(version), 1, fp);
	//���̐����擾�B
	fread(&m_numBone, sizeof(m_numBone), 1, fp);
	m_bones.resize(m_numBone);	//���̐��������T�C�Y

	char buff_c;	//����Ȃ�BYTE�^�ϐ����󂯎��p
	int buff_i;		//����Ȃ�int�^�ϐ����󂯎��p


	for (int i = 0; i < m_numBone; i++) {
		auto& bone = m_bones.at(i);
		size_t nameCount = 0;
		//���̖��O���擾�B
		fread(&nameCount, 1, 1, fp);
		bone.name = std::make_unique<char[]>(nameCount + 1);
		fread(bone.name.get(), nameCount + 1, 1, fp);
		//�e��ID���擾�B
		fread(&bone.parentNo, sizeof(bone.parentNo), 1, fp);
		//�o�C���h�|�[�Y���擾�B
		fread(bone.bindPose, sizeof(bone.bindPose), 1, fp);
		//�o�C���h�|�[�Y�̋t�����擾�B
		fread(bone.invBindPose, sizeof(bone.invBindPose), 1, fp);
		//�{�[���̔ԍ��B
		bone.no = i;


		//ShadowCasterFlag
		fread(&buff_c, sizeof(buff_c), 1, fp);
		//ShadowReceiverFlag
		fread(&buff_c, sizeof(buff_c), 1, fp);

		//numIntDataNum
		fread(&buff_i, sizeof(buff_i), 1, fp);
		for (int i = 0; i < buff_i; i++)
		{
			//intValue
			int b;
			fread(&b, sizeof(b), 1, fp);
		}
		//numFloatDataNum
		fread(&buff_i, sizeof(buff_i), 1, fp);
		for (int i = 0; i < buff_i; i++)
		{
			//floatValue
			float b;
			fread(&b, sizeof(b), 1, fp);
		}
		//numStringDataNum
		fread(&buff_i, sizeof(buff_i), 1, fp);
		for (int i = 0; i < buff_i; i++)
		{
			//stringSize
			int b;
			fread(&b, sizeof(b), 1, fp);
			//string
			char* BYTE = new char[b + 1];
			fread(&BYTE, sizeof(char), b + 1, fp);
		}
		//numVector3DataNum
		fread(&buff_i, sizeof(buff_i), 1, fp);
		for (int i = 0; i < buff_i; i++)
		{
			//Vector3Value
			Vector3 b;
			fread(&b, sizeof(b), 1, fp);
		}

	}

	fclose(fp);
}


