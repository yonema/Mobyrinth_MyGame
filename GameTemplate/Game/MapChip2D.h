#pragma once
#include "CaslFile.h"
#include "SpriteRender.h"


/// <summary>
/// �}�b�v�`�b�v2D�B
/// </summary>
class MapChip2D
{
public:		//�����ŌĂ΂�郁���o�֐�
	~MapChip2D();

public:		//�����̃����o�֐�����Ɏg��

	/// <summary>
	/// �������B
	/// </summary>
	/// <param name="caslData">Casl�f�[�^�B</param>
	void Init(CaslData* caslData);



private:	//�f�[�^�����o
	CSpriteRender* m_spriteRender = nullptr;	//�X�v���C�g�B
};



