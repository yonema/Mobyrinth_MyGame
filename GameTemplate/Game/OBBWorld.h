#pragma once
#include "OBB.h"


/// <summary>
/// OBBWorld
/// �Փˉ���������OBB��o�^����ꏊ
/// </summary>
class COBBWorld
{
private:	//�����ŌĂ΂�郁���o�֐�
	COBBWorld() {};			//�R���X�g���N�^����private�ɉB��
	~COBBWorld() {};		//�f�X�g���N�^��private�ɉB��

private:	//static�ȃf�[�^�����o
	static COBBWorld* m_instance;	//���g�̗B��̃C���X�^���X�����ϐ�

public:		//static�ȃ����o�֐�

	/// <summary>
	/// �V���O���g���p�^�[��
	/// �B��̃C���X�^���X�����֐�
	/// �ŏ��ɌĂ�łˁI
	/// </summary>
	static void CreateInstance()
	{
		//�B��̃C���X�^���X�𐶐�����
		m_instance = new COBBWorld;
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
	static COBBWorld* GetInstance()
	{
		return m_instance;
	}

public:		//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// OBB��OBBWorld�ɓo�^����֐�
	/// </summary>
	/// <param name="obb">�o�^����OBB</param>
	void AddOBB(COBB* obb);

	/// <summary>
	/// OBBWorld�ɓo�^���Ă���OBB����������֐�
	/// </summary>
	/// <param name="obb">�o�^����OBB</param>
	void RemoveOBB(COBB* obb);

	/// <summary>
	/// �Ƃ肠������������ǁA�g��Ȃ��\��B
	/// �e�X�g�����ĂȂ�
	/// �ꉞ�AOBB�ƃ��C�̌����_�����߂�֐�
	/// </summary>
	/// <param name="start">���C�̎n�_</param>
	/// <param name="end">���C�̏I�_</param>
	/// <param name="pos">���������ۂ̌����_�̍��W</param>
	/// <returns>�����������H</returns>
	bool InIntersectLine(const Vector3& start, const Vector3& end, Vector3* pos = nullptr);
	
	/// <summary>
	/// OBBWorld�ɓo�^���Ă��邷�ׂĂ�OBB�𒲂ׂāA
	/// ��������OBB�ƏՓ˂��Ă����ԋ߂�OBB����
	/// �߂��B
	/// �ǂ�OBB�Ƃ��Փ˂��Ă��Ȃ�������Anullptr��
	/// �߂��ė���B
	/// </summary>
	/// <param name="myOBB">���g��OBB</param>
	/// <returns>�Փ˂��Ă���OBB���Anullptr</returns>
	COBB* HitAllOBB(const COBB& myOBB);

private:	//�f�[�^�����o
	std::vector<COBB*> m_worldOBBs;		//OBBWorld�ɓo�^����Ă���OBB�B

};
