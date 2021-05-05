#pragma once
#include "FontRender.h"
#include <locale>
#include "ObjectType.h"

/// <summary>
/// Tips�\���N���X
/// </summary>
class CTips
{
public:		//�����ŌĂ΂�郁���o�֐�
	CTips();	//�R���X�g���N�^
	~CTips();	//�f�X�g���N�^

public:		//�����o�֐�

	/// <summary>
	/// ���[�h�ς݂̃I�u�W�F�N�g�^�C�v�̃e�L�X�g���Z�b�g����
	/// </summary>
	/// <param name="textType">�I�u�W�F�N�g�^�C�v</param>
	void SetText(const int textType);

	/// <summary>
	/// �e�L�X�g�����ɂ��\�����Ȃ�
	/// </summary>
	void ClearText();

private:	//private�ȃ����o�֐�

	/// <summary>
	/// �e�L�X�g�̏�����
	/// </summary>
	void InitText();

	/// <summary>
	/// �e�L�X�g�����[�h����
	/// </summary>
	/// <param name="objectType">�I�u�W�F�N�g�^�C�v</param>
	/// <param name="fileName">�e�L�X�g�t�@�C���̖��O</param>
	void LoadText(const int objectType, const char* fileName);

private:	//static�ȃf�[�^�����o
	//�e�L�X�g�̍ő�T�C�Y
	static const int m_maxTextSize = 256;

private:	//�f�[�^�����o
	CFontRender* m_tipsFR = nullptr;				//�uTips�v�̃t�H���g�����_���[
	CFontRender* m_sentenceFR = nullptr;			//Tips�̕��͂�\������t�H���g�����_���[
	char m_text[enObjectTypeNum][m_maxTextSize];	//���[�h����Tips�̕���
};

