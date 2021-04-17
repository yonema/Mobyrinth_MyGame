#include "stdafx.h"
#include "SceneChange.h"


void CSceneChange::Init()
{
	//�����_�����O�^�[�Q�b�g�̏�����
	InitRenderTarget();

	InitSprite();

	srand((unsigned int)time(NULL));
}

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

void CSceneChange::InitSprite()
{
	//���C�������_�����O�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

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


	SpriteInitData finalSpriteInitData;
	//�e�N�X�`�������C�������_�[�^�[�Q�b�g�̃e�N�X�`���ɐݒ�
	finalSpriteInitData.m_textures[0] = &m_renderTarget.GetRenderTargetTexture();
	//�V�F�[�_�[��ݒ�
	finalSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	//�e�N�X�`���̕��ƍ�����ݒ�
	finalSpriteInitData.m_width = mainRenderTarget.GetWidth();
	finalSpriteInitData.m_height = mainRenderTarget.GetHeight();
	finalSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;
	m_finalSprite.Init(finalSpriteInitData);


	SpriteInitData backSpriteInitData;
	backSpriteInitData.m_ddsFilePath[0] = "Assets/Image/backSprite.DDS";
	backSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
	backSpriteInitData.m_width = 1280.0f;
	backSpriteInitData.m_height = 720.0f;

	m_backSprite.Init(backSpriteInitData);
	m_backSprite.Update(g_vec3Zero, g_quatIdentity, { -1.0f,1.0f,1.0f });
}

void CSceneChange::UpdateParam()
{
	if (m_wipeFlag)
	{
		m_wipeParam.wipeSize += m_wipeSpeed;
	}
}

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
	if (m_wipeFlag)
		m_backSprite.Draw(renderContext);
	m_finalSprite.Draw(renderContext);

	//���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);
}

void CSceneChange::SetWipeDirection(const Vector2& dir)
{
	m_wipeParam.wipeDir = dir;
	//�X�v���C�g�͍��E���]�����Ă��邩��AX���𔽓]������B
	m_wipeParam.wipeDir.x *= -1.0f;
	//���K������
	m_wipeParam.wipeDir.Normalize();
}

void CSceneChange::RandomWipeStart()
{
	int randNum = rand() % 5;
	SetWipeType(randNum);
	if (randNum == enWipe)
		SetWipeSpeed(20.0f);
	else if (randNum == enCircleWipe)
		SetWipeSpeed(10.0f);
	else
		SetWipeSpeed(2.0f);

	WipeOut();
}

const float CSceneChange::GetWipeSizeMax(const int wipeType) const
{
	float sizeMax = m_wipeMaxX;

	switch (wipeType)
	{
	case enWipe:
		sizeMax = m_wipeMaxX;
		sizeMax += m_wipeMaxY * m_wipeParam.wipeDir.y;
		break;
	case enCircleWipe:
		sizeMax = m_circleWipeMax;
		break;
	case enVerticalStripeWipe:
		sizeMax = m_verticalStripeWipeMax;
		break;
	case enHorizontalStripeWipe:
		sizeMax = m_horizontalStripeWipeMax;
		break;
	case enCheckerboardWipe:
		sizeMax = m_checkerboardWipeMax;
		break;
	}

	return sizeMax;
}


const bool CSceneChange::IsWipeFinished() const
{
	bool isFinished = false;

	if (GetWipeSize() >= GetWipeSizeMax(GetWipeType()))
		isFinished = true;

	return isFinished;
}