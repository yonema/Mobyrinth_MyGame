#pragma once
class TklFile
{
public:
	/// <summary>
	/// �{�[���B
	/// </summary>
	struct SBone {
		//tkl�t�@�C���̃f�[�^
		std::unique_ptr<char[]> name;	//���̖��O�B
		int parentNo = -1;				//�e�̔ԍ��B
		float bindPose[4][3];			//�o�C���h�|�[�Y�B
		float invBindPose[4][3];		//�o�C���h�|�[�Y�̋t���B
		int no;							//�{�[���̔ԍ��B

	};
	/// <summary>
	/// TKl�t�@�C�������[�h����B
	/// </summary>
	/// <param name="filePath"></param>
	void Load(const char* filePath);

	/// <summary>
/// �{�[���ɑ΂��ăN�G�����s���B
/// </summary>
/// <param name="query">�N�G���֐�</param>
	void QueryBone(std::function<void(SBone& bone)> query)
	{
		for (auto& bone : m_bones) {
			query(bone);
		}
	}
private:
	int m_numBone = 0;			//���̐��B
	std::vector<SBone> m_bones;	//���̃��X�g�B
	using BonePtr = std::unique_ptr<SBone>;


};

