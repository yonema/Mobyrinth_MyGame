#pragma once
#include "LightData.h"
#include "DirectionLight.h"


class CLightManager
{
private:
	static CLightManager* m_instance;	//�V���O���g���p�^�[��
	CLightManager();	//�R���X�g���N�^��
	~CLightManager();	//�f�X�g���N�^��private�ɉB��
	//void Init();
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
	//���C�g��ǉ�
	void AddLight(CLightBase* light);
	//���C�g������
	void RemoveLight(const CLightBase* light);

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

private:
	struct SLightParam {
		Vector3 eyePos;				//�����̈ʒu�B
		int numDirectionLight;		//�f�B���N�V�������C�g�̐��B
		Vector3 ambientLight;		//�A���r�G���g���C�g�B
		int numPointLight;			//�|�C���g���C�g�̐��B
		float specPow;				//�X�y�L�����̍i��
	};
	SLightParam m_lightParam;
	std::list<CDirectionLight*> m_directionLights;	//�f�B���N�V�������C�g�̃��X�g


};

