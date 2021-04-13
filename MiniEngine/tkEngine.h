#pragma once

#include "HID/GamePad.h"
#include "TResourceBank.h"
#include "tkFile/TkmFile.h"
#include "Shader.h"


class GraphicsEngine;

class TkEngine {
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~TkEngine();
	/// <summary>
	/// �t���[���̊J�n���ɌĂ΂�鏈���B
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// �t���[���̏I�����ɌĂ΂�鏈���B
	/// </summary>
	void EndFrame();
	/// <summary>
	/// �Q�[���G���W���̏������B
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
	/// <summary>
	/// tkm�t�@�C�����o���N����擾�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <returns>tkm�t�@�C���Bnullptr���Ԃ��Ă�����o���N�ɓo�^����Ă��Ȃ��B</returns>
	TkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return m_tkmFileBank.Get(filePath);
	}
	/// <summary>
	/// tkm�t�@�C�����o���N�ɓo�^�B
	/// </summary>
	/// <param name="filePath">tkm�t�@�C���p�X</param>
	/// <param name="tkmFile"></param>
	void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
	{
		m_tkmFileBank.Regist(filePath, tkmFile);
	}
	/// <summary>
	/// �V�F�[�_�[�t�@�C���o���N����V�F�[�_�[���擾�B
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return m_shaderBank.Get(programName.c_str());
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="shader"></param>
	void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		m_shaderBank.Regist(programName.c_str(), shader);
	}
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
	TResourceBank<TkmFile> m_tkmFileBank;			//tkm�t�@�C���o���N�B
	TResourceBank<Shader> m_shaderBank;				//�V�F�[�_�[�o���N
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B

	//�ǉ�
public:		//�����o�֐�
	/// <summary>
	/// �V���h�E��`�悷��
	/// </summary>
	void DrawShadow();

	/// <summary>
	/// ���C�������_�[�^�[�Q�b�g���g�p�ł���悤�ɂ���
	/// </summary>
	void UseMainRenderTarget();

	/// <summary>
	/// �|�X�g�G�t�F�N�g��`�悷��
	/// </summary>
	void DrawPostEffect();

	/// <summary>
	/// ���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɕ`�悷��
	/// </summary>
	void DrawFrameBuffer();

	/// <summary>
	/// �e�𐶐����郉�C�g�𐶐�����
	/// </summary>
	/// <param name="direction">�e����郉�C�g�̕���</param>
	/// <param name="length">���C�g���ǂꂭ�炢����Ă��邩</param>
	/// <param name="target">���C�g���Ƃ炷�ڕW</param>
	void CreateShadow
	(const Vector3& direction, const float length = 300.0f, const Vector3& target = g_vec3Zero);

	/// <summary>
	/// �e�𐶐����郉�C�g�̃p�����[�^�[�ݒ肷��
	/// </summary>
	/// <param name="direction">�e����郉�C�g�̕���</param>
	/// <param name="length">���C�g���ǂꂭ�炢����Ă��邩</param>
	/// <param name="target">���C�g���Ƃ炷�ڕW</param>
	void SetShadowPararm
	(const Vector3& direction, const float length = 300.0f, const Vector3& target = g_vec3Zero);
};

extern TkEngine* g_engine;	//TK�G���W���B