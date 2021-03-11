#pragma once
#include "Player_kari.h"

class ILevelObjectBase;

class CLevelObjectManager
{
private:
	CLevelObjectManager();
	~CLevelObjectManager();

private:
	static CLevelObjectManager* m_instance;
public:
	/// <summary>
	/// �V���O���g���p�^�[��
	/// �B��̃C���X�^���X�����֐�
	/// �ŏ��ɌĂ�łˁI
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new CLevelObjectManager;
	}

	/// <summary>
	/// �C���X�^���X�������֐�
	/// </summary>
	static void DeleteInstance()
	{
		delete m_instance;
	}

	/// <summary>
	/// �C���X�^���X�̎Q�Ƃ�߂��֐�
	/// </summary>
	/// <returns>�C���X�^���X�̎Q��</returns>
	static CLevelObjectManager* GetInstance()
	{
		return m_instance;
	}


public:
	/// <summary>
	/// �Q�[�����[�v�œǂ�łق���
	/// �A�b�v�f�[�g�֐��֐�
	/// </summary>
	void ExecuteUpdate();

	Player_kari* GetPlayer()const
	{
		return m_player;
	}

	/// <summary>
	/// �E�F�C�|�C���g�́u�ꏊ�v��������
	/// </summary>
	/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
	/// <param name="posMap">�ꏊ�̃}�b�v</param>
	void InitWayPointPos(const std::size_t vecSize, std::map<int, Vector3>& posMap);
	/// <summary>
	/// �E�F�C�|�C���g�́u��]�v��������
	/// </summary>
	/// <param name="vecSize">�E�F�C�|�C���g�̃T�C�Y</param>
	/// <param name="rotMap">��]�̃}�b�v</param>
	void InitWayPointRot(const std::size_t vecSize, std::map<int, Quaternion>& rotMap);

	std::vector<Vector3>* GetWayPointPos()
	{
		return &m_wayPointPos;
	}
	std::vector<Quaternion>* GetWayPointRot()
	{
		return &m_wayPointRot;
	}

	void AddObject(ILevelObjectBase* object);
	void RemoveObject(ILevelObjectBase* object);


	template<class T>
	void QueryLevelObjects(const int objectType, std::function<bool(T* lo)> func)
	{
		int a = 1;
		for (auto lo : m_levelObjects) {
				if (lo->GetObjectType() == objectType) {
					//�������B
					T* p = dynamic_cast<T*>(lo);
					if (func(p) == false) {
						//�N�G�����f�B
						return;
					}
				}
		}
	}
private:
	Player_kari* m_player = nullptr;
	std::vector<Vector3> m_wayPointPos;		//�E�F�C�|�C���g�́u�ꏊ�v�̃R���e�i
	std::vector<Quaternion> m_wayPointRot;	//�E�F�C�|�C���g�́u��]�v�̃R���e�i
	int m_vecSize = 0;		//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l

	std::vector<ILevelObjectBase*> m_levelObjects;

};


template<class T>
static inline void QueryLOs(const int objectType, std::function<bool(T* lo)> func)
{
	return CLevelObjectManager::GetInstance()->QueryLevelObjects(objectType, func);
}