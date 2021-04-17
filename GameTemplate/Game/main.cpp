#include "stdafx.h"
#include "system/system.h"
#include "Game.h"
#include "LightManager.h"
#include "LevelObjectManager.h"
#include "SoundEngine.h"
#include "OBBWorld.h"


///////////////////////////////////////////////////////////////////
// �E�B���h�E�v���O�����̃��C���֐��B
///////////////////////////////////////////////////////////////////
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//�Q�[���̏������B
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Game"));

	//////////////////////////////////////
	// �������珉�������s���R�[�h���L�q����B
	//////////////////////////////////////

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃C���X�^���X���쐬����B
	GameObjectManager::CreateInstance();
	PhysicsWorld::CreateInstance();
	
	//�e�Ǘ��N���X�̃C���X�^���X�𐶐�����B
	CLightManager::CreateInstance();		//���C�g�}�l�[�W���[
	CLevelObjectManager::CreateInstance();	//���x���I�u�W�F�N�g�}�l�[�W���[
	CSoundEngine::CreateInstance();			//�T�E���h�G���W��
	COBBWorld::CreateInstance();			//OBB���[���h

	//�Q�[���̃C���X�^���X�𐶐�����
	Game* game = NewGO<Game>(0, "Game");

	//�V���h�E����
	g_engine->CreateShadow({ 1.0f,-1.0f,-1.0f }, 1000.0f);
	//�f�B���N�V�������C�g�̐���
	CDirectionLight* gameDirectionLight = nullptr;
	gameDirectionLight = NewGO<CDirectionLight>(0, "GameDirectionLight");
	gameDirectionLight->SetDirection({ 1.0f,-1.0f,-1.0f });
	gameDirectionLight->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	//////////////////////////////////////
	// ���������s���R�[�h�������̂͂����܂ŁI�I�I
	//////////////////////////////////////

	auto& renderContext = g_graphicsEngine->GetRenderContext();	//�����_�[�R���e�L�X�g

	//���C�������_�[�^�[�Q�b�g
	RenderTarget& mainRenderTarget = g_graphicsEngine->GetMainRenderTarget();


	// ��������Q�[�����[�v�B
	while (DispatchWindowMessage())
	{
		//�����_�����O�J�n�B
		g_engine->BeginFrame();

		//////////////////////////////////////////////////////////////
		///	�A�b�v�f�[�g�̏����J�n
		//////////////////////////////////////////////////////////////

		//�Q�[���I�u�W�F�N�g�̃A�b�v�f�[�g
		GameObjectManager::GetInstance()->ExecuteUpdate();
		//���C�g�̃A�b�v�f�[�g
		CLightManager::GetInstance()->ExecuteUpdate();
		//�T�E���h�G���W���̃A�b�v�f�[�g
		CSoundEngine::GetInstance()->Update();

		//////////////////////////////////////////////////////////////
		///	�A�b�v�f�[�g�̏����I��
		//////////////////////////////////////////////////////////////


		//////////////////////////////////////////////////////////////
		///	�����_�����O�̏����J�n
		//////////////////////////////////////////////////////////////

		//�V���h�E�C���O
		g_engine->DrawShadow();

		//���C�������_�����O�^�[�Q�b�g���Z�b�g����
		g_engine->UseMainRenderTarget();

		//���C���̃����_�����O�����s
		GameObjectManager::GetInstance()->ExecuteRender(renderContext);

		//���C�������_�����O�^�[�Q�b�g�̏������ݏI���҂�
		g_engine->WaitDrawingMainRenderTarget();

		//�|�X�g�G�t�F�N�g
		g_engine->DrawPostEffect();

		//��ʓ]����2D���o
		g_engine->DrawSceneChange();

		//���C�������_�����O�^�[�Q�b�g�̊G���t���[���o�b�t�@�[�ɕ`�悷��
		g_engine->DrawFrameBuffer();

		//////////////////////////////////////////////////////////////
		///	�����_�����O�̏����I��
		//////////////////////////////////////////////////////////////

		//�����_�����O�I��
		g_engine->EndFrame();
	}

	//�f�B���N�V�������C�g��j��
	DeleteGO(gameDirectionLight);

	//�Q�[���̃C���X�^���X��j��
	DeleteGO(game);


	//�e�Ǘ��N���X�̃C���X�^���X��j������B
	COBBWorld::DeleteInstance();			//OBB���[���h
	CSoundEngine::DeleteInstance();			//�T�E���h�G���W��
	CLevelObjectManager::DeleteInstance();	//���x���I�u�W�F�N�g�}�l�[�W���[
	CLightManager::DeleteInstance();		//���C�g�}�l�[�W���[

	//�Q�[���I�u�W�F�N�g�}�l�[�W���[���폜�B
	PhysicsWorld::DeleteInstance();
	GameObjectManager::DeleteInstance();


	return 0;
}

