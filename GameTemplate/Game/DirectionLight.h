#pragma once
#include "LightBase.h"
#include "LightData.h"
class CDirectionLight : public CLightBase
{
private:
	void Init();
public:
	CDirectionLight();
	~CDirectionLight();
	//���ۃN���X�̏������z�֐��̃I�[�o�[���C�h
	bool StartSub() override final;
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
	const Vector3& GetDirection() const
	{
		return m_light.dirLigDirection;
	}
	/// <summary>
	/// �f�B���N�V�������C�g�̐F�̐ݒ�
	/// </summary>
	/// <param name="color">���C�g�̐F</param>
	void SetColor(const Vector4& color);
	/// <summary>
	/// ���f�[�^���擾
	/// </summary>
	/// <returns>���f�[�^�̎Q��</returns>
	SDirectionLight& GetRawData()
	{
		return m_light;
	};

private:
	SDirectionLight m_light;	//�f�B���N�V�������C�g�̍\����
};

