#pragma once
class CModelRender : IGameObject
{
public:
	void Init(const char* filePath);
private:	//�����o�֐�
	/// <summary>
	/// �X�P���g�����������B
	/// </summary>
	void InitSkeleton(const char* filePath);
	/// <summary>
	/// ���f�����������B
	/// </summary>
	void InitModel(const char* filePath);
private:	//�f�[�^�����o
	Model m_model;	//���f���\������
	Skeleton m_skeleton;	//�X�P���g���B
};

