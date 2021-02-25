#pragma once
#include "LightData.h"
#include "DirectionLight.h"

class CDirectionLight;

class CLightManager
{
private:
	static CLightManager* m_instance;	//�V���O���g���p�^�[��
	CLightManager();	//�R���X�g���N�^��
	~CLightManager();	//�f�X�g���N�^��private�ɉB��
	void Init();
	//void Release();

	static const int Max_DirectionLight = 4;	//�f�B���N�V�������C�g�̍ő吔
	static const int Max_PointLight = 4;		//�|�C���g���C�g�̍ő吔
	static const int Max_SpotLight = 4;			//�X�|�b�g���C�g�̍ő吔

public:
	static void CreateInstance()
	{
		m_instance = new CLightManager;
	}
	static void DeleteInstance()
	{
		delete m_instance;
	}
	static CLightManager* GetInstance()
	{
		return m_instance;
	}
	void ExecuteUpdate();
	//���C�g��ǉ�
	bool AddLight(CDirectionLight* light);
	//���C�g������
	void RemoveLight(CDirectionLight* light);

	/// <summary>
	/// �A���r�G���g���C�g���擾
	/// </summary>
	/// <returns>�J���[</returns>
	const Vector3& GetAmbientLight() const
	{
		return m_lightParam.ambientLight;
	}
	/// <summary>
	/// �A���r�G���g���C�g��ݒ�
	/// </summary>
	/// <param name="ambient">�J���[</param>
	void SetAmbientLight(const Vector3& ambient)
	{
		m_lightParam.ambientLight = ambient;
	}

	//std::list<CDirectionLight*>* GetDirectionLights()
	//{
	//	return &m_directionLights;
	//}
	SLightParam* GetLightParam()
	{
		return &m_lightParam;
	}
	const int GetNumDirectionLight() const
	{
		return m_lightParam.numDirectionLight;
	}
	const int GetNumPointLight() const
	{
		return m_lightParam.numPointLight;
	}
	const float GetSpecPow() const
	{
		return m_lightParam.specPow;
	}
	SDirectionLight* GetDirectionLigData()
	{
		return m_directionLightsData;
	}
private:

	static SLightParam m_lightParam;
	//std::list<SDirectionLight> m_lights;	//�f�B���N�V�������C�g�̍\����
	//std::list<CDirectionLight*> m_directionLights;	//�f�B���N�V�������C�g�̃��X�g

	//���ꂼ��̃f�B���N�V�������C�g�̃f�[�^
	SDirectionLight m_directionLightsData[Max_DirectionLight];


	//���ꂼ��̃f�B���N�V�������C�g�̎Q��
	CDirectionLight* m_directionLights[Max_DirectionLight] = { nullptr };

	int m_currentNum = 0;

};

