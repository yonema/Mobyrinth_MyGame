#pragma once
#include "LightData.h"


class CDirectionLight;

class CLightManager
{
private://private�ȃ����o�֐�
	CLightManager();	//�R���X�g���N�^��
	~CLightManager();	//�f�X�g���N�^��private�ɉB��
	/// <summary>
	/// ����������
	/// </summary>
	void Init();

private://static�ȃf�[�^�����o
	static CLightManager* m_instance;			//�B��̃C���X�^���X�B�V���O���g���p�^�[��
	static const int Max_DirectionLight = 4;	//�f�B���N�V�������C�g�̍ő吔
	static const int Max_PointLight = 4;		//�|�C���g���C�g�̍ő吔
	static const int Max_SpotLight = 4;			//�X�|�b�g���C�g�̍ő吔

public://staic�ȃ����o�֐�
	/// <summary>
	/// �V���O���g���p�^�[��
	/// �B��̃C���X�^���X�����֐�
	/// �ŏ��ɌĂ�łˁI
	/// </summary>
	static void CreateInstance()
	{
		m_instance = new CLightManager;
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
	static CLightManager* GetInstance()
	{
		return m_instance;
	}

	/// <summary>
	/// �f�B���N�V�������C�g�̍ő吔��߂��֐�
	/// </summary>
	/// <returns>�f�B���N�V�������C�g�̍ő吔</returns>
	static const int GetMax_DirectionLight()
	{
		return Max_DirectionLight;
	}

public://publid�ȃ����o�֐�

	/// <summary>
	/// �Q�[�����[�v�œǂ�łق���
	/// �A�b�v�f�[�g�֐��֐�
	/// </summary>
	void ExecuteUpdate();

	/// <summary>
	/// ���C�g��ǉ�����֐�
	/// </summary>
	/// <param name="light">�ǉ����郉�C�g</param>
	/// <returns>�ǉ��ł�����true��߂�</returns>
	bool AddLight(CDirectionLight* light);

	/// <summary>
	/// ���C�g����������֐�
	/// </summary>
	/// <param name="light">�������郉�C�g</param>
	void RemoveLight(CDirectionLight* light);

	/// <summary>
	/// ���C�g�̋��ʃp�����[�^�̎Q�Ƃ�߂��֐�
	/// </summary>
	/// <returns>���C�g�̋��ʃp�����[�^�̎Q��</returns>
	SLightParam* const GetLightParam() const
	{
		return &m_lightParam;
	}

	/// <summary>
	/// �f�B���N�V�������C�g�B�̐擪�A�h���X��߂��֐�
	/// </summary>
	/// <returns>�f�B���N�V�������C�g�B�̐擪�A�h���X</returns>
	SDirectionLight* const GetDirectionLigData() //const	//�Ȃ񂩃_���݂���
	{
		return m_directionLightsData;
	}

	int GetShadowNum()const
	{
		return m_lightParam.numShadow;
	}

	void AddShadowNum()
	{
		m_lightParam.numShadow++;
	}

	void RemoveShadowNum()
	{
		m_lightParam.numShadow--;
	}

private://�f�[�^�����o

	static SLightParam m_lightParam;	//���C�g�̋��ʃp�����[�^

	//���ꂼ��̃f�B���N�V�������C�g�̃f�[�^
	SDirectionLight m_directionLightsData[Max_DirectionLight];

	//���ꂼ��̃f�B���N�V�������C�g�̎Q��
	CDirectionLight* m_directionLights[Max_DirectionLight] = { nullptr };

	//���݂̃��C�g�̐�
	int m_currentNum = 0;

};

