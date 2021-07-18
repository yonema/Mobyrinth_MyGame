#include "stdafx.h"
#include "HUD.h"
#include "GameTime.h"

//HUD�̒萔�f�[�^���g����悤�ɂ���
using namespace HUDConstData;

//�R���X�g���N�^
CHUD::CHUD()
	:m_wipeSpeed(SPEED_WIPE_DEFAULT)	//���C�v�̃X�s�[�h���f�t�H���g�̃X�s�[�h�ɂ���
{
	//�f���^�^�C�����Z�b�g���Ă���
	m_deltaTime = GameTime().GetFrameDeltaTime();

	return;
}

//�������֐�
void CHUD::Init()
{
	//�����_�����O�^�[�Q�b�g�̏�����
	InitRenderTarget();

	//�X�v���C�g�̏�����
	InitSprite();

	//�t�F�[�h�̏�����
	InitFade();

	//�����̏�����
	srand((unsigned int)time(NULL));

	return;
}

/// <summary>
/// �����_�����O�^�[�Q�b�g�̏�����
/// </summary>
void CHUD::InitRenderTarget()
{
	//���C�������_�����O�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();
	
	//���C�v�O�̃����_�����O�^�[�Q�b�g���쐬
	m_beforeWipingRenderTarget.Create(
		mainRenderTarget.GetWidth(),	//���C�������_�����O�^�[�Q�b�g�Ɠ����ɂ���
		mainRenderTarget.GetHeight(),	//���C�������_�����O�^�[�Q�b�g�Ɠ����ɂ���
		1,
		1,
		// �y���ځz�J���[�o�b�t�@�[�̃t�H�[�}�b�g��32bit���������_�ɂ��Ă���
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_D32_FLOAT
	);

	return;
}

/// <summary>
/// �X�v���C�g�̏�����
/// </summary>
void CHUD::InitSprite()
{
	//���C�������_�����O�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	//���C�������_�����O�^�[�Q�b�g�̃X�v���C�g�̏������f�[�^
	SpriteInitData mainRenderTargetSpriteInitData;
	//�e�N�X�`�������C�������_�[�^�[�Q�b�g�̃e�N�X�`���ɐݒ�
	mainRenderTargetSpriteInitData.m_textures[0] = &mainRenderTarget.GetRenderTargetTexture();
	//�V�F�[�_�[��ݒ�
	mainRenderTargetSpriteInitData.m_fxFilePath = SHADER_FILEPATH_WIPE;
	//�e�N�X�`���̕��ƍ�����ݒ�
	mainRenderTargetSpriteInitData.m_width = mainRenderTarget.GetWidth();
	mainRenderTargetSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//�萔�o�b�t�@��ݒ�
	mainRenderTargetSpriteInitData.m_expandConstantBuffer = &m_wipeParam;
	mainRenderTargetSpriteInitData.m_expandConstantBufferSize = sizeof(m_wipeParam);

	//���C�������_�����O�^�[�Q�b�g�̃X�v���C�g�̏�����
	m_mainRenderTargetSprite.Init(mainRenderTargetSpriteInitData);


	//���C�v������̃X�v���C�g�̏������f�[�^
	SpriteInitData afterWipingSpriteInitData;
	//�e�N�X�`�������C�������_�[�^�[�Q�b�g�̃e�N�X�`���ɐݒ�
	afterWipingSpriteInitData.m_textures[0] = &m_beforeWipingRenderTarget.GetRenderTargetTexture();
	//�V�F�[�_�[��ݒ�
	afterWipingSpriteInitData.m_fxFilePath = spriteRenderConstData::SHADER_FILEPATH_DEFAULT;
	//�e�N�X�`���̕��ƍ�����ݒ�
	afterWipingSpriteInitData.m_width = mainRenderTarget.GetWidth();
	afterWipingSpriteInitData.m_height = mainRenderTarget.GetHeight();
	//�A���t�@�u�����h���[�h�𓧉ߍ����ɐݒ�
	afterWipingSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

	//���C�v������̃X�v���C�g�̏�����
	m_afterWipingSprite.Init(afterWipingSpriteInitData);


	//���ɉf���X�v���C�g�̏������f�[�^
	SpriteInitData backSpriteInitData;
	//�e�N�X�`���̃t�@�C���p�X��ݒ�
	backSpriteInitData.m_ddsFilePath[0] = SPRITE_FILEPATH_BACK_SPRITE;
	//�V�F�[�_�[��ݒ�
	backSpriteInitData.m_fxFilePath = spriteRenderConstData::SHADER_FILEPATH_DEFAULT;
	//�e�N�X�`���̕��ƍ�����ݒ�
	backSpriteInitData.m_width = mainRenderTarget.GetWidth();;
	backSpriteInitData.m_height = mainRenderTarget.GetHeight();;

	//���ɉf���X�v���C�g��������
	m_backSprite.Init(backSpriteInitData);
	//���ɉf���X�v���C�g�̃A�b�v�f�[�g
	m_backSprite.Update(
		g_VEC3_ZERO, g_QUAT_IDENTITY,
		spriteRenderConstData::SPRITE_SCALE_DEFAULT
	);

	return;
}

/// <summary>
/// �t�F�[�h�̏�����
/// </summary>
void CHUD::InitFade()
{
	//�t�F�[�h�p�̃X�v���C�g�̏������f�[�^
	SpriteInitData fadeSpriteInitData;
	fadeSpriteInitData.m_ddsFilePath[0] = SPRITE_FILEPATH_FADE;	//dds�̃t�@�C�X�p�X
	fadeSpriteInitData.m_width = g_FRAME_BUFFER_W;				//����
	fadeSpriteInitData.m_height = g_FRAME_BUFFER_H;				//�c��
	fadeSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Trans;	//���ߍ������[�h
	//�V�F�[�_�[�̃t�@�C���p�X
	fadeSpriteInitData.m_fxFilePath = spriteRenderConstData::SHADER_FILEPATH_DEFAULT;

	//�t�F�[�h�p�̃X�v���C�g�̏�����
	m_fadeSprite.Init(fadeSpriteInitData);

	//�����ɂ��Ă���
	m_fadeSprite.SetAlphaValue(spriteRenderConstData::ALPHA_VALUE_TRANSPARENT);

	return;
}

//�p�����[�^�̃A�b�v�f�[�g�֐�
void CHUD::UpdateParam()
{
	//���C�v�t���O�������Ă�����
	if (m_wipeFlag)
	{
		//���C�v�C�����́A���O�Ƀ��[�h�������ăf���^�^�C���������������Ȃ邩��A
		//���C�v�A�E�g���̃f���^�^�C�����g���B
		if (m_wipeParam.inOrOut == EN_OUT)
			m_deltaTime = GameTime().GetFrameDeltaTime();

		//���C�v��i�߂�	//�f���^�^�C�����|����
		m_wipeParam.wipeSize += m_wipeSpeed * m_deltaTime;
	}

	return;
}



//�`��֐�
void CHUD::Draw(RenderContext& renderContext)
{
	//���C�������_�����O�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();

	//���C�v�O�̃����_�����O�^�[�Q�b�g�����p�\�ɂȂ�܂ő҂�
	renderContext.WaitUntilToPossibleSetRenderTarget(m_beforeWipingRenderTarget);
	// �����_�����O�^�[�Q�b�g��ݒ�
	renderContext.SetRenderTargetAndViewport(m_beforeWipingRenderTarget);
	// �����_�����O�^�[�Q�b�g���N���A
	renderContext.ClearRenderTargetView(m_beforeWipingRenderTarget);

	//���C�������_�����O�^�[�Q�b�g�̃X�v���C�g��`������
	m_mainRenderTargetSprite.Draw(renderContext);

	// �����_�����O�^�[�Q�b�g�ւ̕`�����ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(m_beforeWipingRenderTarget);



	//�����_�����O�^�[�Q�b�g�����C�������_�����O�^�[�Q�b�g�ɕύX����
	renderContext.WaitUntilToPossibleSetRenderTarget(mainRenderTarget);
	//���C�������_�����O�^�[�Q�b�g�����p�ł���܂ő҂�
	renderContext.SetRenderTargetAndViewport(mainRenderTarget);

	//���C�v���Ă�����A���̃X�v���C�g��`�悷��
	if (m_wipeFlag)
		m_backSprite.Draw(renderContext);

	//���C�v��̃X�v���C�g�̕`�悷��
	m_afterWipingSprite.Draw(renderContext);


	//�t�F�[�h�̃X�v���C�g��`�悷��
	m_fadeSprite.Draw(renderContext);

	//���C�������_�����O�^�[�Q�b�g�ւ̏������ݏI���҂�
	renderContext.WaitUntilFinishDrawingToRenderTarget(mainRenderTarget);

	return;
}

/// <summary>
/// ���C�v���������ݒ肷��
/// leftOrLight�Ɩ�������悤�ɐݒ肵�Ȃ��łˁB
/// </summary>
/// <param name="dir">���C�v�������</param>
void CHUD::SetWipeDirection(const Vector2& dir)
{
	//���C�v�̕�����ݒ肷��
	m_wipeParam.wipeDir = dir;
	//�X�v���C�g�͍��E���]�����Ă��邩��AX���𔽓]������B
	m_wipeParam.wipeDir.x *= -1.0f;
	//���K������
	m_wipeParam.wipeDir.Normalize();

	return;
}

/// <summary>
/// ���C�v�̎�ނ������_���ɂ��āA���C�v�A�E�g�J�n
/// </summary>
void CHUD::RandomWipeOut()
{
	//0�`4��5�̗����𓾂�
	int randNum = rand() % 5;
	randNum = 0;
	//���C�v�̎�ނ�ݒ肷��
	SetWipeType(randNum);

	//���ʂ̃��C�v��������
	if (randNum == EN_WIPE)
		//�������C�v������
		SetWipeSpeed(SPEED_WIPE);
	//�~�`���C�v��������
	else if (randNum == EN_CIRCLE_WIPE)
		//������Ƃ����������C�v������
		SetWipeSpeed(SPEED_WIPE_CIRLE);
	//���̑���������
	else
		//�f�t�H���g�̃��C�v�X�s�[�h�̂���
		SetWipeSpeed(SPEED_WIPE_DEFAULT);

	//���C�v�A�E�g����
	WipeOut();

	return;
}

/// <summary>
/// �e��ނ̃��C�v�̍ő�T�C�Y�𓾂�
/// ���C�v�̎�ނ�SceneChange.h���Q��
/// </summary>
/// <param name="wipeType">���C�v�̎��</param>
/// <returns>�e��ނ̃��C�v�̍ő�T�C�Y</returns>
const float CHUD::GetWipeSizeMax(const int wipeType) const
{
	//���C�v�̍ő�̃T�C�Y
	float sizeMax = SIZE_MAX_X_WIPE;

	//�e��ނ��ƂɊ���U��
	switch (wipeType)
	{
	case EN_WIPE:
		//���ʂ̃��C�v
		sizeMax = SIZE_MAX_X_WIPE;
		sizeMax += SIZE_MAX_Y_WIPE * m_wipeParam.wipeDir.y;
		break;

	case EN_CIRCLE_WIPE:
		//�~�`���C�v
		sizeMax = SIZE_MAX_WIPE_CIRCLE;
		break;

	case EN_VERTICAL_STRIPE_WIPE:
		//�c�ȃ��C�v
		sizeMax = SIZE_MAX_WIPE_STRIPE_VERTICAL;
		break;

	case EN_HORIZONTAL_STRIPE_WIPE:
		//���ȃ��C�v
		sizeMax = SIZE_MAX_WIPE_STRIPE_HORIZONTAL;
		break;

	case EN_CHECKERBOARD_WIPE:
		//�`�F�b�J�[�{�[�h���C�v
		sizeMax = SIZE_MAX_WIPE_CHECKERBOARD;
		break;
	}

	//���C�v�̍ő�̃T�C�Y��߂�
	return sizeMax;
}

/// <summary>
/// ���C�v���Ō�܂Ń��C�v�������𓾂�
/// </summary>
/// <returns>�Ō�܂Ń��C�v�������H</returns>
const bool CHUD::IsWipeFinished() const
{
	//�Ō�܂Ń��C�v�������H
	bool isFinished = !m_wipeFlag;

	//���݂̃��C�v�̃T�C�Y���A
	//���݂̃��C�v�̎�ނ̍ő�̃T�C�Y���
	//�傫��������
	if (GetWipeSize() >= GetWipeSizeMax(GetWipeType()))
		//�Ō�܂Ń��C�v���Ă���
		isFinished = true;

	//�Ō�܂Ń��C�v�������߂�
	return isFinished;
}
