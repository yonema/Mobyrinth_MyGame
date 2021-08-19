#include "stdafx.h"
#include "Sky.h"

//�R���X�g���N�^
CSky::CSky()
{

}

//�f�X�g���N�^
CSky::~CSky()
{

}

//�X�^�[�g�֐�
bool CSky::Start()
{
	//���f���̏������f�[�^
	ModelInitData initData;
	//tkm�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_tkmFilePath = MODEL_FILEPATH_SKY;
	//�V�F�[�_�[�t�@�C���̃t�@�C���p�X���w�肷��B
	initData.m_fxFilePath = FX_FILEPATH_SKY;

	//�X�J�C�L���[�u�e�N�X�`����������
	m_skyCubeTexture.InitFromDDSFile(TEXTURE_FILEPATH_SKY);

	//�X�J�C�L���[�u�e�N�X�`����SRV�ɓo�^
	initData.m_expandShaderResoruceView[0] = &m_skyCubeTexture;

	//���f��������������
	m_model.Init(initData);
	//���f�����X�V����
	m_model.UpdateWorldMatrix(m_position, g_QUAT_IDENTITY, m_scale);

	SetIsDefferdRender(false);

	return true;
}

//�A�b�v�f�[�g�֐�
void CSky::Update()
{
	//�_�[�e�B�t���O�������Ă�����
	if (m_isDirty)
	{
		//���f�����X�V
		m_model.UpdateWorldMatrix(m_position, g_QUAT_IDENTITY, m_scale);

		//�_�[�e�B�t���O��܂�
		m_isDirty = false;
	}

	return;
}

//�`��֐�
void CSky::ForwardRender(RenderContext& rc)
{
	//���f����`�悷��
	m_model.Draw(rc);
}