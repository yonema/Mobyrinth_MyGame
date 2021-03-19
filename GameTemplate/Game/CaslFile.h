#pragma once

/// <summary>
/// Casl�t�@�C���̃f�[�^�̍\����
/// </summary>
struct CaslData
{
	std::unique_ptr<char[]> name;			//���O
	std::unique_ptr<char[]> fileName;	//�t�@�C���p�X
	std::unique_ptr<char[]> ddsFileName;		//dds�t�@�C��
	std::unique_ptr<char[]> ddsFilePath;	//dds�t�@�C���p�X
	Vector2 position = g_vec2Zero;			//���W
	int width = 0;							//����
	int height = 0;							//�c��
	int numberLayer = 0;					//���C���[�D��x
	Vector2 scale = { 1.0f,1.0f };			//�傫��
};




/// <summary>
/// casl�t�@�C���������N���X
/// </summary>
class CaslFile
{
private:
	//CaslData�̃��j�[�N�|�C���^
	using CaslDataPtr = std::unique_ptr<CaslData>;
public:		//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// casl�t�@�C����ǂݍ���
	/// </summary>
	/// <param name="filePath">casl�t�@�C���p�X</param>
	void Load(const char* filePath);

	/// <summary>
	/// Casl�f�[�^�̐����擾
	/// </summary>
	/// <returns></returns>
	const int GetNumCaslData() const
	{
		return m_caslDataList.size();
	}

	/// <summary>
	/// Casl�f�[�^���擾
	/// </summary>
	/// <param name="number">�i���o�[</param>
	/// <returns>Casl�f�[�^�̃|�C���^</returns>
	CaslData* GetCaslData(int number) const
	{
		return m_caslDataList[number].get();
	}

private:	//private�ȃ����o�֐�

	/// <summary>
	/// 1���������ǂݍ��ށB�u , �v��u \n �v��ǂݍ��ނ̂Ɏg��
	/// </summary>
	/// <param name="file">�t�@�C��</param>
	void ReadOnlyOneCharacter(FILE* file)const;

	/// <summary>
	/// int�^��ǂݍ���
	/// </summary>
	/// <param name="file">�t�@�C��</param>
	/// <returns>int�^�̒l</returns>
	int ReadInteger(FILE* file)const;

	/// <summary>
	/// float�^��ǂݍ���
	/// </summary>
	/// <param name="file">�t�@�C��</param>
	/// <returns>float�^�̒l</returns>
	float ReadDecimal(FILE* file)const;

private:	//�f�[�^�����o
	std::vector<CaslDataPtr> m_caslDataList;	//CaslData�̃R���e�i
};

