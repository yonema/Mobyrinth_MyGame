#pragma once
#include "LightBase.h"
#include "LightData.h"
#include "LightManager.h"

class CDirectionLight : public IGameObject
{
private:
	void Init();
public:
	CDirectionLight();
	~CDirectionLight();
	//���ۃN���X�̏������z�֐��̃I�[�o�[���C�h
	bool Start() override final;
	//void Update() override final;

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
	void SetRawData(SDirectionLight* lig)
	{
		m_light = lig;
	}
	void SetControlNumber(const int num)
	{
		m_controlNumber = num;
	}
	const int GetControlNumver()const 
	{
		return m_controlNumber;
	}

private:
	SDirectionLight* m_light = nullptr;	//�f�B���N�V�������C�g�̍\����
	int m_controlNumber;
	bool m_result;

	
};

