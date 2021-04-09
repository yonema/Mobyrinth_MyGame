#pragma once
#include "LightData.h"
#include "LightManager.h"

/// <summary>
/// �f�B���N�V�������C�g�N���X
/// </summary>
class CDirectionLight : public IGameObject
{
public:		//�����ŌĂ΂�郁���o�֐�
	CDirectionLight();		//�R���X�g���N�^
	~CDirectionLight();		//�f�X�g���N�^

public:		//�����o�֐�

	/// <summary>
	/// �f�B���N�V�������C�g�̕�����ݒ�
	/// </summary>
	/// <param name="direction">���C�g�̕���</param>
	void SetDirection(const Vector3& direction);

	/// <summary>
	/// �f�B���N�V�������C�g�̕������擾
	/// </summary>
	/// <returns>���C�g�̕���</returns>
	//const Vector3& GetDirection() const
	//{
	//	return m_light.dirLigDirection;
	//}

	/// <summary>
	/// �f�B���N�V�������C�g�̐F�̐ݒ�
	/// </summary>
	/// <param name="color">���C�g�̐F</param>
	void SetColor(const Vector4& color);

	/// <summary>
	/// ���f�[�^���擾
	/// </summary>
	/// <returns>���f�[�^�̎Q��</returns>
	SDirectionLight* GetRawData()
	{
		return m_light;
	};

	/// <summary>
	/// ���f�[�^��ݒ�
	/// </summary>
	/// <param name="lig">���f�[�^</param>
	void SetRawData(SDirectionLight* lig)
	{
		m_light = lig;
	}

	/// <summary>
	/// ����i���o�[��ݒ�
	/// </summary>
	/// <param name="num">����i���o�[</param>
	void SetControlNumber(const int num)
	{
		m_controlNumber = num;
	}

	/// <summary>
	/// ����i���o�[���擾
	/// </summary>
	/// <returns>����i���o�[</returns>
	const int GetControlNumver()const 
	{
		return m_controlNumber;
	}

private:
	SDirectionLight* m_light = nullptr;	//�f�B���N�V�������C�g�̍\����
	int m_controlNumber;				//����i���o�[
	bool m_result;						//���C�g�}�l�[�W���[�ɓo�^���Ă��邩�H

	
};

