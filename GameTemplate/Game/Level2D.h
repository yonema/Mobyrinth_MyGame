#pragma once
#include "MapChip2D.h"


/// <summary>
/// 2D�̃��x���f�[�^�B
/// </summary>
struct Level2DObjectData {
	Vector3 position = Vector3::Zero;			//���W�B
	Quaternion rotation = Quaternion::Identity;	//��]�B
	Vector3 scale = Vector3::One;				//�g�嗦�B
	int width = 0;								//�����B
	int height = 0;								//�c���B
	const char* name;							//���O�B
	const char* ddsFilePath;					//.dds�t�@�C���̃t�@�C���p�X�B


	/// <summary>
	/// �����œn�����I�u�W�F�N�g���̃I�u�W�F�N�g�𒲂ׂ�B
	/// </summary>
	/// <param name="objName">���ׂ閼�O�B</param>
	/// <returns>���O�������ꍇ��true��Ԃ��܂��B</returns>
	bool EqualObjectName(const char* objName) const
	{
		return strcmp(objName, name) == 0;
	}


	/// <summary>
	/// ���O���O����v���邩���ׂ�B
	/// </summary>
	/// <param name="n">���ׂ閼�O�B</param>
	/// <returns>���O�������ꍇ��true��Ԃ��܂��B</returns>
	bool ForwardMatchName(const char* n) const
	{
		auto len = strlen(n);
		auto namelen = strlen(name);
		if (len > namelen) {
			//���O�������B�s��v�B
			return false;
		}
		return strncmp(n, name, len) == 0;
	}
};



/// <summary>
/// ���x��2D
/// </summary>
class CLevel2D {
private:
	//�}�b�v�`�b�v�̃��j�[�N�|�C���^
	using MapChip2DPtr = std::unique_ptr<MapChip2D>;

public:		//�����o�֐�

	/// <summary>
	/// ���x�����������B
	/// </summary>
	/// <param name="filePath">casl�t�@�C���̃t�@�C���p�X�B</param>
	/// <param name="hookFunc">�I�u�W�F�N�g���쐬���鎞�̏������t�b�N���邽�߂̊֐��I�u�W�F�N�g�B</param>
	void Init(const char* filePath, std::function<bool(Level2DObjectData& objData)> hookFunc);



private:	//private�ȃ����o�֐�

	/// <summary>
	/// �}�b�v�`�b�v2D��ǉ�����B
	/// </summary>
	/// <param name="caslData">Casl�f�[�^</param>
	void AddMapChip2D(SCaslData* caslData);

private:	//�f�[�^�����o
	std::vector<MapChip2DPtr> m_mapChip2DPtrList;		//�}�b�v�`�b�v2D�B
};

