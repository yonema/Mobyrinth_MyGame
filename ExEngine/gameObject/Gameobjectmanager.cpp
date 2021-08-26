/*!
 *@brief	CGameObjectのマネージャ
 */
#include "stdafx.h"
#include "ExEngine.h"
#include "GameObjectManager.h"

GameObjectManager* GameObjectManager::m_instance = nullptr;

GameObjectManager::GameObjectManager()
{
	if (m_instance != nullptr) {
		//インスタンスがすでに作られている。
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
	//死亡フラグがついているゲームオブジェクトを破棄する。
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

	//ポーズ中かどうか？
	if (!IGameObject::GetGamePaused())
	{
		//ポーズ中でない
		//普通のUpdateを呼ぶ
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->UpdateWrapper();
			}
		}
	}
	else
	{
		//ポーズ中
		//UpdateOnlyPausedを呼ぶ
		for (auto& goList : m_gameObjectListArray) {
			for (auto& go : goList) {
				go->UpdateOnlyPausedWrapper();
			}
		}
	}

	//ポーズ中でも、ポーズ中でなくても
	//UpdateWhenPausedを呼ぶ
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->AlwaysUpdateWrapper();
		}
	}

}
void GameObjectManager::ExecuteRender(RenderContext& rc)
{
	//レンダラーを変更するならここを改造していくと良い。
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->RenderWrapper(rc);
		}
	}

}

/**
 * @brief フォワードレンダリングを実行
 * @param rc レンダーコンテキスト
*/
void GameObjectManager::ExecuteForwardRender(RenderContext& rc)
{
	//レンダラーを変更するならここを改造していくと良い。
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->ForwardRenderWrapper(rc);
		}
	}

}

void GameObjectManager::ExecuteFontRender(RenderContext& rc)
{
	//レンダラーを変更するならここを改造していくと良い。
	for (auto& goList : m_gameObjectListArray) {
		for (auto& go : goList) {
			go->FontRenderWrapper(rc);
		}
	}
}
