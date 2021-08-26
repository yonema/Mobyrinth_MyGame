/*!
 *@brief	CGameObject�̃}�l�[�W��
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//�C���X�^���X�����łɍ���Ă���B
		std::abort();
	}
	m_instance = this;
}
GameObjectManager::~GameObjectManager()
{
	m_instance = nullptr;
}
void GameObjectManager::ExecuteUpdate()
{	
	//���S�t���O�����Ă���Q�[���I�u�W�F�N�g��j������B
	for (auto& goList : m_gameObjectListArray) {
		goList.remove_if([&](IGameObject* go) {	
			if (go->IsDead()) {
				delete go;
				return true;
			}
			return false;	 
		});
	}

	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->StartWrapper();
		}
	}

	//�|�[�Y�����ǂ����H
	if (!IGameObject::GetGamePaused())
	{
		//�|�[�Y���łȂ�
		//���ʂ�Update���Ă�
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->UpdateWrapper();
			}
		}
	}
	else
	{
		//�|�[�Y��
		//UpdateOnlyPaused���Ă�
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->UpdateOnlyPausedWrapper();
			}
		}
	}

	//�|�[�Y���ł��A�|�[�Y���łȂ��Ă�
	//UpdateWhenPaused���Ă�
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->AlwaysUpdateWrapper();
		}
	}

}
void GameObjectManager::ExecuteRender(RenderContext& rc)
{
	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc);
		}
	}

}

/**
 * @brief �t�H���[�h�����_�����O�����s
 * @param rc �����_�[�R���e�L�X�g
*/
void GameObjectManager::ExecuteForwardRender(RenderContext& rc)
{
	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ForwardRenderWrapper(rc);
		}
	}

}

void GameObjectManager::ExecuteFontRender(RenderContext& rc)
{
	//�����_���[��ύX����Ȃ炱�����������Ă����Ɨǂ��B
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->FontRenderWrapper(rc);
		}
	}
}
