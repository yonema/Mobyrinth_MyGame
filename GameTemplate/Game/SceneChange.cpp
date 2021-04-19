#include "stdafx.h"
#include "SceneChange.h"

//�������֐�
void CSceneChange::Init()
{
	//�����_�����O�^�[�Q�b�g�̏�����
	InitRenderTarget();

	//�X�v���C�g�̏�����
	InitSprite();

	//�����̏�����
	srand((unsigned int)time(NULL));
}

/// <summary>
/// �����_�����O�^�[�Q�b�g�̏�����
/// </summary>
void CSceneChange::InitRenderTarget()
{
	//���C�������_�����O�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	m_renderTarget.Create(
		mainRenderTarget.GetWidth(),
		mainRenderTarget.GetHeight(),
		1,
		1,
		// �y���ځz�J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);
}

/// <summary>
/// �X�v���C�g�̏�����
/// </summary>
void CSceneChange::InitSprite()
{
	//���C�������_�����O�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	//�X�v���C�g�̏������f�[�^
	SpriteInitData spriteInitData;
	//�e�N�X�`�������C�������_�[�^�[�Q�b�g�̃e�N�X�`���ɐݒ�
	spriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//�V�F�[�_�[��ݒ�
	spriteInitData.m_fxFilePath = "Assets/shader/SceneChange.fx";
	//�e�N�X�`���̕��ƍ�����ݒ�
	spriteInitData.m_width = mainRenderTarget.GetWidth();
	spriteInitData.m_height = mainRenderTarget.GetHeight();
	//�萔�o�b�t�@��ݒ�
	spriteInitData.m_expandConstantBuffer = &m_wipeParam;
	spriteInitData.m_expandConstantBufferSize = sizeof(m_wipeParam);

	//�X�v���C�g�̏�����
	m_sprite.Init(spriteInitData);


	//�ŏI�I�ȃX�v���C�g�̏������f�[�^
	SpriteInitData finalSpriteInitData;
	//�e�N�X�`�������C�������_�[�^�[�Q�b�g�̃e�N�X�`���ɐݒ�
	finalSpriteInitData.m_textures[0] = &m_renderTarget.GetRenderTargetTexture();
	//�V�F�[�_�[��ݒ�
	finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	//�e�N�X�`���̕��ƍ�����ݒ�
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//�A���t�@�u�����h���[�h���Z�����ɐݒ�
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

	//�ŏI�I�ȃX�v���C�g�̏�����
	m_finalSprite.Init(finalSpriteInitData);


	//���ɉf���X�v���C�g�̏������f�[�^
	SpriteInitData backSpriteInitData;
	//�e�N�X�`���̃t�@�C���p�X��ݒ�
	backSpriteInitData.m_ddsFilePath[0] = "Assets/Image/backSprite2.DDS";
	//�V�F�[�_�[��ݒ�
	backSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	//�e�N�X�`���̕��ƍ�����ݒ�
	backSpriteInitData.m_width = 1280.0f;
	backSpriteInitData.m_height = 720.0f;

	//���ɉf���X�v���C�g��������
	m_backSprite.Init(backSpriteInitData);
	//���ɉf���X�v���C�g�̃A�b�v�f�[�g
	m_backSprite.Update(g_vec3Zero, g_quatIdentity, { -1.0f,1.0f,1.0f });
}

//�p�����[�^�̃A�b�v�f�[�g�֐�
void CSceneChange::UpdateParam()
{
	//���C�v�t���O�������Ă�����
	if (m_wipeFlag)
	{
		//���C�v��i�߂�
		m_wipeParam.wipeSize += m_wipeSpeed;
	}
}

//�`��֐�
void CSceneChange::Draw(RenderContext& renderContext)
{
	//���C�������_�����O�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	//�����_�����O�^�[�Q�b�g����ʓ]���p�̃����_�����O�^�[�Q�b�g�ɐݒ�
	renderContext.WaitUntilToPossibleSetRenderTarget(m_renderTarget);

	// �����_�����O�^�[�Q�b�g��ݒ�
	renderContext.SetRenderTargetAndViewport(m_renderTarget);

	// �����_�����O�^�[�Q�b�g���N���A
	renderContext.ClearRenderTargetView(m_renderTarget);


	m_sprite.Draw(renderContext);


	// �����_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_renderTarget);



	//�����_�����O�^�[�Q�b�g�����C�������_�����O�^�[�Q�b�g�ɕύX����
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//���C�������_�����O�^�[�Q�b�g�����p�ł���܂ő҂�
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);

	//���C�v���Ă�����A���̃X�v���C�g��`�悷��
	if (m_wipeFlag)
		m_backSprite.Draw(renderContext);

	m_finalSprite.Draw(renderContext);

	//���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

/// <summary>
/// ���C�v���������ݒ肷��
/// leftOrLight�Ɩ�������悤�ɐݒ肵�Ȃ��łˁB
/// </summary>
/// <param name="dir">���C�v�������</param>
void CSceneChange::SetWipeDirection(const Vector2& dir)
{
	m_wipeParam.wipeDir = dir;
	//�X�v���C�g�͍��E���]�����Ă��邩��AX���𔽓]������B
	m_wipeParam.wipeDir.x *= -1.0f;
	//���K������
	m_wipeParam.wipeDir.Normalize();
}

/// <summary>
/// ���C�v�̎�ނ������_���ɂ��āA���C�v�A�E�g�J�n
/// </summary>
void CSceneChange::RandomWipeOut()
{
	//0�`4��5�̗����𓾂�
	int randNum = rand() % 5;

	//���C�v�̎�ނ�ݒ肷��
	SetWipeType(randNum);

	//���ʂ̃��C�v��������
	if (randNum == enWipe)
		//�������C�v������
		SetWipeSpeed(20.0f);
	//�~�`���C�v��������
	else if (randNum == enCircleWipe)
		//������Ƃ����������C�v������
		SetWipeSpeed(10.0f);
	//���̑���������
	else
		//�f�t�H���g�̃��C�v�X�s�[�h�̂���
		SetWipeSpeed(2.0f);

	//���C�v�A�E�g����
	WipeOut();
}

/// <summary>
/// �e��ނ̃��C�v�̍ő�T�C�Y�𓾂�
/// ���C�v�̎�ނ�SceneChange.h���Q��
/// </summary>
/// <param name="wipeType">���C�v�̎��</param>
/// <returns>�e��ނ̃��C�v�̍ő�T�C�Y</returns>
const float CSceneChange::GetWipeSizeMax(const int wipeType) const
{
	//���C�v�̍ő�̃T�C�Y
	float sizeMax = m_wipeMaxX;

	//�e��ނ��ƂɊ���U��
	switch (wipeType)
	{
	case enWipe:
		//���ʂ̃��C�v
		sizeMax = m_wipeMaxX;
		sizeMax += m_wipeMaxY * m_wipeParam.wipeDir.y;
		break;

	case enCircleWipe:
		//�~�`���C�v
		sizeMax = m_circleWipeMax;
		break;

	case enVerticalStripeWipe:
		//�c�ȃ��C�v
		sizeMax = m_verticalStripeWipeMax;
		break;

	case enHorizontalStripeWipe:
		//���ȃ��C�v
		sizeMax = m_horizontalStripeWipeMax;
		break;

	case enCheckerboardWipe:
		//�`�F�b�J�[�{�[�h���C�v
		sizeMax = m_checkerboardWipeMax;
		break;
	}

	//���C�v�̍ő�̃T�C�Y��߂�
	return sizeMax;
}

/// <summary>
/// ���C�v���Ō�܂Ń��C�v�������𓾂�
/// </summary>
/// <returns>�Ō�܂Ń��C�v�������H</returns>
const bool CSceneChange::IsWipeFinished() const
{
	//�Ō�܂Ń��C�v�������H
	bool isFinished = false;

	//���݂̃��C�v�̃T�C�Y���A
	//���݂̃��C�v�̎�ނ̍ő�̃T�C�Y���
	//�傫��������
	if (GetWipeSize() >= GetWipeSizeMax(GetWipeType()))
		//�Ō�܂Ń��C�v���Ă���
		isFinished = true;

	//�Ō�܂Ń��C�v�������߂�
	return isFinished;
}