#pragma once
#include "Player.h"


//���x���I�u�W�F�N�g�̒��ۃN���X
class ILevelObjectBase;


/// <summary>
/// ���x���I�u�W�F�N�g�̃}�l�[�W���[
/// </summary>
class CLevelObjectManager
{
private:	//�����ŌĂ΂�郁���o�֐�
	CLevelObjectManager();		//�R���X�g���N�^��private�ɉB��
	~CLevelObjectManager();		//�f�X�g���N�^��private�ɉB��

private:	//static�ȃf�[�^�����o
	static CLevelObjectManager* m_instance;	//���g�̗B��̃C���X�^���X�����ϐ�

public:		//static�ȃ����o�֐�

	/// <summary>
	/// �V���O���g���p�^�[��
	/// �B��̃C���X�^���X�����֐�
	/// �ŏ��ɌĂ�łˁI
	/// </summary>
	static void CreateInstance()
	{
		//�B��̃C���X�^���X�𐶐�����
		m_instance = new CLevelObjectManager;
	}

	/// <summary>
	/// �C���X�^���X�������֐�
	/// </summary>
	static void DeleteInstance()
	{
		//�B��̃C���X�^���X��j������
		delete m_instance;
	}

	/// <summary>
	/// �B��C���X�^���X�̎Q�Ƃ�߂��֐�
	/// </summary>
	/// <returns>�B��C���X�^���X�̎Q��</returns>
	static CLevelObjectManager* GetInstance()
	{
		return m_instance;
	}

public:		//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// �v���C���[�̎Q�Ƃ𓾂�
	/// </summary>
	/// <returns>�v���C���[�̎Q��</returns>
	Player* GetPlayer()const
	{
		return m_player;
	}

	/// <summary>
	/// �v���C���[�̎Q�Ƃ�ݒ肷��
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void SetPlayer(Player* player)
	{
		m_player = player;
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

	/// <summary>
	/// �E�F�C�|�C���g�́u�ꏊ�v�̎Q�Ƃ𓾂�
	/// </summary>
	/// <returns>�E�F�C�|�C���g�́u�ꏊ�v�̎Q��</returns>
	std::vector<Vector3>* GetWayPointPos()
	{
		return &m_wayPointPos;
	}

	/// <summary>
	/// �E�F�C�|�C���g�́u��]�v�̎Q�Ƃ𓾂�
	/// </summary>
	/// <returns>�E�F�C�|�C���g�́u��]�v�̎Q��</returns>
	std::vector<Quaternion>* GetWayPointRot()
	{
		return &m_wayPointRot;
	}

	/// <summary>
	/// �E�F�C�|�C���g�̃x�N�^�[�̃T�C�Y�𓾂�
	/// </summary>
	/// <returns>�E�F�C�|�C���g�̃x�N�^�[�̃T�C�Y</returns>
	int GetVecSize()const
	{
		return m_vecSize;
	}

	const Vector3 CalcWayPointNextPos
	(const int rpIndex, const Vector3& pos, const float dist, const bool leftOrRight);

	/// <summary>
	/// ���g�iLevelObjectManager�j�ɃI�u�W�F�N�g��ǉ�����
	/// </summary>
	/// <param name="object">�ǉ�����I�u�W�F�N�g</param>
	void AddObject(ILevelObjectBase* object);

	/// <summary>
	/// ���g�iLevelObjectManager�j����I�u�W�F�N�g��j������
	/// </summary>
	/// <param name="object">�j������I�u�W�F�N�g</param>
	void RemoveObject(ILevelObjectBase* object);

	/// <summary>
	/// ���g�iLevelObjectManager�j�ɓo�^���Ă���I�u�W�F�N�g�̒�����
	/// �N�G���i�₢���킹�j������B
	/// �^�C�v�ꗗ��LevelObjectBase.h���Q��
	/// </summary>
	/// <typeparam name="T">�N�G������I�u�W�F�N�g�̌^</typeparam>
	/// <param name="objectType">�N�G������I�u�W�F�N�g�̃^�C�v</param>
	/// <param name="func">�N�G�����Ď��s�������֐�</param>
	template<class T>
	void QueryLevelObjects(const int objectType, std::function<bool(T* lo)> func)
	{
		//���g�iLevelObjectManager�j�ɓo�^���Ă���I�u�W�F�N�g�̒�����
		//������o���ăN�G�����Ă���
		for (auto lo : m_levelObjects) {
			//�w�肵���^�C�v�ƈ�v����I�u�W�F�N�g�𒲂ׂ�
				if (lo->GetObjectType() == objectType) {
					//�������B
					T* p = dynamic_cast<T*>(lo);
					//�֐������s����
					if (func(p) == false) {
						//�N�G�����f�B
						return;
					}
				}
		}
	}

	/// <summary>
	/// �I�u�W�F�N�g�����ׂČ�������
	/// �������g�ȊO�ŏd�Ȃ��Ă�����̂�T���B
	/// </summary>
	/// <param name="thisObject">�������g�̏��</param>
	/// <param name="objectType">�������g�̖��O</param>
	/// <returns></returns>
	bool QueryLevelAllObjects(ILevelObjectBase& thisObject, const int objectType);


	/// <summary>
	/// ���x���I�u�W�F�N�g�����ׂď�������
	/// </summary>
	void AllDeleteLOs();



	//�f�o�b�N�p
	//��ŏ���
	std::vector<ILevelObjectBase*> GetLevelObjects()
	{
		return m_levelObjects;
	}
private:
	Player* m_player = nullptr;				//�v���C���[�̃|�C���^
	std::vector<Vector3> m_wayPointPos;		//�E�F�C�|�C���g�́u�ꏊ�v�̃R���e�i
	std::vector<Quaternion> m_wayPointRot;	//�E�F�C�|�C���g�́u��]�v�̃R���e�i
	int m_vecSize = 0;						//�E�F�C�|�C���g�X�e�[�g�̍ő�̒l

	std::vector<ILevelObjectBase*> m_levelObjects;	//�C���X�^���X�������x���I�u�W�F�N�g�̔z��

};


/// <summary>
/// LevelObjectManager�ɓo�^���Ă���I�u�W�F�N�g�̒�����
/// �N�G���i�₢���킹�j������B
/// �^�C�v�ꗗ��LevelObjectBase.h���Q��
/// </summary>
/// <typeparam name="T">�N�G������I�u�W�F�N�g�̌^</typeparam>
/// <param name="objectType">�N�G������I�u�W�F�N�g�̃^�C�v</param>
/// <param name="func">�N�G�����Ď��s�������֐�</param>
template<class T>
static inline void QueryLOs(const int objectType, std::function<bool(T* lo)> func)
{
	//LevelObjectManager�̎Q�Ƃ������Ă��āA
	//�N�G���̃����o�֐������������Ă���B
	return CLevelObjectManager::GetInstance()->QueryLevelObjects(objectType, func);
}

static inline bool QueryAllLOs(ILevelObjectBase& thisObject, const int objectType)
{
	return CLevelObjectManager::GetInstance()->QueryLevelAllObjects(thisObject, objectType);
}