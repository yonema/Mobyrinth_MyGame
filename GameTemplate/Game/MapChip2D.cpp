#include "stdafx.h"
#include "MapChip2D.h"

/// <summary>
/// �������B
/// </summary>
/// <param name="caslData">Casl�f�[�^�B</param>
void MapChip2D::Init(CaslData* caslData)
{
	//�X�v���C�g�����_���[�̐���
	m_spriteRender = NewGO<CSpriteRender>(1);
	//�X�v���C�g�����_���[�̏�����
	m_spriteRender->Init(caslData->ddsFilePath.get(), caslData->width, caslData->height);

	//���W���X�v���C�g�����_���[�ɐݒ�
	Vector3 position = { 0.0f,0.0f,0.0f };
	position.x = caslData->position.x;
	position.y = caslData->position.y;
	m_spriteRender->SetPosition(position);

	//�g����X�v���C�g�����_���[�ɐݒ�
	Vector3 scale = { 1.0f,1.0f,1.0f };
	scale.x = caslData->scale.x;
	scale.y = caslData->scale.y;
	m_spriteRender->SetScale(scale);

}


MapChip2D::~MapChip2D()
{
	if (m_spriteRender)
		DeleteGO(m_spriteRender);
}