#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "LightManager.h"
#include "LevelObjectManager.h"
#include "SoundEngine.h"
#include "OBBWorld.h"
#include "StopWatch.h"
#include "GameTime.h"
#include "effect/effect.h"
#include "TitleNameData.h"

/**
 * @brief �K�v�ȃC���X�^���X�B�𐶐�����
*/
void CreateInstances();

/**
 * @brief ���������C���X�^���X������j������
*/
void DeleteInstances();

/**
 * @brief �Q�[���S�̂�ʂ��Ďg���f�B���N�V�������C�g�𐶐�����
 * @param [out] directionLight �f�B���N�V�������C�g�̃|�C���^
*/
void CreateGameDirectionLigth(CDirectionLight* directionLight);

/**
 * @brief �A�b�v�f�[�g����
*/
void Updating();

/**
 * @brief �����_�����O����
*/
void Rendering(RenderContext& renderContext);


///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, titleNameData::TITLE_NAME);

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	//�K�v�ȃC���X�^���X�B�𐶐�����
	CreateInstances();

	//�X�g�b�v�E�H�b�`�𐶐�����
	CStopWatch stopWatch;

	//�Q�[���̃C���X�^���X�𐶐�����
	CGame* game = NewGO<CGame>(PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_GAME));

	//�Q�[���S�̂�ʂ��Ďg���f�B���N�V�������C�g
	CDirectionLight* gameDirectionLight = nullptr;
	//�Q�[���S�̂�ʂ��Ďg���f�B���N�V�������C�g�𐶐�����
	CreateGameDirectionLigth(gameDirectionLight);


	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////

	auto& renderContext = g_graphicsEngine->GetRenderContext();	//�����_�[�R���e�L�X�g

	//���C�������_�[�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();


	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�X�g�b�v�E�H�b�`�̌v���J�n
		stopWatch.Start();

		//�����_�����O�J�n�B
		g_engine->BeginFrame();

		//////////////////////////////////////////////////////////////
		///	�A�b�v�f�[�g�̏����J�n
		//////////////////////////////////////////////////////////////

		//�A�b�v�f�[�g����
		Updating();

		//////////////////////////////////////////////////////////////
		///	�A�b�v�f�[�g�̏����I��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		///	�����_�����O�̏����J�n
		//////////////////////////////////////////////////////////////

		//�����_�����O����
		Rendering(renderContext);

		//////////////////////////////////////////////////////////////
		///	�����_�����O�̏����I��
		//////////////////////////////////////////////////////////////

		//�����_�����O�I��
		g_engine->EndFrame();


		//FPS�Œ�̏���
		//�X�s�����b�N���s���B
		int restTime = 0;
		do {
			stopWatch.Stop();
			restTime = STOP_WATCH_CONST_DATA::MILLISECOND_FOR_LOCK_60FPS - 
				static_cast<int>(stopWatch.GetElapsedMillisecond());
		} while (restTime > 0);

		//�X�g�b�v�E�H�b�`�̌v���I��
		stopWatch.Stop();

		//�f���^�^�C�����X�g�b�v�E�H�b�`�̌v�����Ԃ���A�v�Z����
		GameTime().PushFrameDeltaTime((float)stopWatch.GetElapsed());
	}

	//�f�B���N�V�������C�g��j��
	DeleteGO(gameDirectionLight);

	//�Q�[���̃C���X�^���X��j��
	DeleteGO(game);

	//���������C���X�^���X������j������
	DeleteInstances();

	return 0;
}


/**
 * @brief �K�v�ȃC���X�^���X�B�𐶐�����
*/
void CreateInstances()
{
	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();

	//�e�Ǘ��N���X�̃C���X�^���X�𐶐�����B
	CLightManager::CreateInstance();		//���C�g�}�l�[�W���[
	CLevelObjectManager::CreateInstance();	//���x���I�u�W�F�N�g�}�l�[�W���[
	CSoundEngine::CreateInstance();			//�T�E���h�G���W��
	COBBWorld::CreateInstance();			//OBB���[���h

	//�G�t�F�N�g�G���W���̃C���X�^���X���쐬����B
	EffectEngine::CreateInstance();


	//�V���h�E�̐����Ə�����
	g_graphicsEngine->GetShadowMap().CreateShadowMap(
		shadowConstData::SHADOW_INIT_DIRECTION,
		shadowConstData::SHADOW_INIT_LENGTH
	);

	return;
}

/**
 * @brief ���������C���X�^���X������j������
*/
void DeleteInstances()
{
	//�G�t�F�N�g�G���W���̃C���X�^���X�̔j��
	EffectEngine::DeleteInstance();

	//�e�Ǘ��N���X�̃C���X�^���X��j������B
	COBBWorld::DeleteInstance();			//OBB���[���h
	CSoundEngine::DeleteInstance();			//�T�E���h�G���W��
	CLevelObjectManager::DeleteInstance();	//���x���I�u�W�F�N�g�}�l�[�W���[
	CLightManager::DeleteInstance();		//���C�g�}�l�[�W���[

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	PhysicsWorld::DeleteInstance();
	GameObjectManager::DeleteInstance();

	return;
}


/**
 * @brief �Q�[���S�̂�ʂ��Ďg���f�B���N�V�������C�g�𐶐�����
 * @param [out] directionLight �f�B���N�V�������C�g�̃|�C���^
*/
void CreateGameDirectionLigth(CDirectionLight* directionLight)
{
	directionLight = NewGO<CDirectionLight>(
		PRIORITY_FIRST, GetGameObjectName(EN_GO_TYPE_GAME_DIRECTION_LIGHT)
		);
	directionLight->SetDirection(
		directionLightConstData::DIRECTIONLIG_INIT_DIRECTION
	);
	directionLight->SetColor(directionLightConstData::DIRECTIONLIG_INIT_COLOR);

	return;
}

/**
 * @brief �A�b�v�f�[�g����
*/
void Updating()
{
	//�Q�[���I�u�W�F�N�g�̃A�b�v�f�[�g
	GameObjectManager::GetInstance()->ExecuteUpdate();
	//�G�t�F�N�g�G���W���̃A�b�v�f�[�g�B
	EffectEngine::GetInstance()->Update(GameTime().GetFrameDeltaTime());
	//���C�g�̃A�b�v�f�[�g
	CLightManager::GetInstance()->Update();
	//�T�E���h�G���W���̃A�b�v�f�[�g
	CSoundEngine::GetInstance()->Update();

	return;
}

/**
 * @brief �����_�����O����
*/
void Rendering(RenderContext& renderContext)
{
	//�V���h�E�C���O
	g_graphicsEngine->GetShadowMap().Draw(renderContext);

	// ZPrepass
	g_graphicsEngine->ZPrepass(renderContext);

	//���C�������_�����O�^�[�Q�b�g���Z�b�g����
	g_graphicsEngine->UseMainRenderTarget();

	//���C���̃����_�����O�����s
	GameObjectManager::GetInstance()->ExecuteRender(renderContext);

	//�G�t�F�N�g�̃h���[�B
	EffectEngine::GetInstance()->Draw();

#ifdef MY_DEBUG
	//FPS��`�悷��
	GameTime().DrawFPS(renderContext, (float)stopWatch.GetElapsed());
#endif

	//���C�������_�����O�^�[�Q�b�g�̏������ݏI���҂�
	g_graphicsEngine->WaitDrawingMainRenderTarget();

	//�|�X�g�G�t�F�N�g
	g_graphicsEngine->DrawPostEffect(renderContext);

	//HUD
	g_graphicsEngine->HUDRender();

	//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɕ`�悷��
	g_graphicsEngine->CopyToFrameBuffer();

	//�o�^����Ă���3D���f�����N���A
	g_graphicsEngine->ClearZPrepassModels();

	return;
}