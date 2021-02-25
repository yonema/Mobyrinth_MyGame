#include "stdafx.h"
#include "TklFile.h"

void TklFile::Load(const char* filePath)
{
	auto fp = fopen(filePath, "rb");
	if (fp == nullptr) {
		return;
	}
	int version;
	//バージョンを取得
	fread(&version, sizeof(version), 1, fp);
	//骨の数を取得。
	fread(&m_numBone, sizeof(m_numBone), 1, fp);
	m_bones.resize(m_numBone);	//骨の数だけリサイズ

	char buff_c;	//いらないBYTE型変数を受け取る用
	int buff_i;		//いらないint型変数を受け取る用


	for (int i = 0; i < m_numBone; i++) {
		auto& bone = m_bones.at(i);
		size_t nameCount = 0;
		//骨の名前を取得。
		fread(&nameCount, 1, 1, fp);
		bone.name = std::make_unique<char[]>(nameCount + 1);
		fread(bone.name.get(), nameCount + 1, 1, fp);
		//親のIDを取得。
		fread(&bone.parentNo, sizeof(bone.parentNo), 1, fp);
		//バインドポーズを取得。
		fread(bone.bindPose, sizeof(bone.bindPose), 1, fp);
		//バインドポーズの逆数を取得。
		fread(bone.invBindPose, sizeof(bone.invBindPose), 1, fp);
		//ボーンの番号。
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


