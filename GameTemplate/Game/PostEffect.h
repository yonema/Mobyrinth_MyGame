#pragma once
#include "Bloom.h"

/// <summary>
/// �|�X�g�G�t�F�N�g�N���X
/// </summary>
class CPostEffect
{
public:		//�����ŌĂ΂�郁���o�֐�
	CPostEffect();		//�R���X�g���N�^
	~CPostEffect();		//�f�X�g���N�^

public:		//�����o�֐�
	void Init();	//�������֐�
	void Draw(RenderContext& renderContext);	//�`�悷��֐�

private:	//�f�[�^�����o
	CBloom m_bloom;		//�u���[���N���X
};

