#pragma once

 


class Light
{
private:
	static Light* m_instance;
	Light();
	~Light();
	void Init();
	void Release();
public:
	static void CreateInstance()
	{
		m_instance = new Light;
	}
	static Light* GetInstance()
	{
		return m_instance;
	}
	static void DeleteInstance()
	{
		delete m_instance;
	}

	//�f�B���N�V�������C�g�̍\����
	struct SDirectionLight
	{
		//�f�B���N�V�������C�g
		Vector3 dirLigDirection;
		float pad;
		Vector3 dirLigColor;
		float pad2;
		Vector3 eyePos;			//�J�����̈ʒu�B
		float specPow;				//�X�y�L�����̍i��B
		Vector3 ambinetLight;	//�����B	
	};
	SDirectionLight m_directionLight;
	//�|�C���g���C�g�̍\����
	struct SPointLight
	{
		Vector3 ptPosition;		//�ʒu�B
		float ptRange;			//�e���͈́B
		Vector3 ptColor;		//�J���[�B
	};
	SPointLight m_pointLight;
};

