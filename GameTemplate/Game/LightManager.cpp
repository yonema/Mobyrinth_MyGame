#include "stdafx.h"
#include "LightManager.h"
#include "LightBase.h"
#include "DirectionLight.h"

CLightManager* CLightManager::m_instance = nullptr;

CLightManager::CLightManager()
{
	if (m_instance != nullptr) {
		//ƒCƒ“ƒXƒ^ƒ“ƒX‚ª‚·‚Å‚Éì‚ç‚ê‚Ä‚¢‚éB
		std::abort();
	}
	//Init();
	m_instance = this;
}

CLightManager::~CLightManager()
{
	//Release()
	m_instance = nullptr;
}

//void CLightManager::Init()
//{
//
//}
//void CLightManager::Release()
//{
//
//}
void CLightManager::AddLight(CLightBase* light)
{
	//if (m_pointLights.size() >= MAX_POINT_LIGHT) {
	//	TK_WARNING("m_pointLights.size() is full!!");
	//	return;
	//}
	if (m_directionLights.size() >= Max_DirectionLight) {
		//TK_WARNING("m_directionLights.size() is full!!");
		return;
	}

	//ƒ‰ƒCƒg‚ÌU‚è•ª‚¯B
	const std::type_info& typeInfo = typeid(*light);
	//if (typeInfo == typeid(CPointLight)) {
	//	//“o˜^Ï‚İ‚©’²‚×‚éB
	//	auto findIt = std::find(m_pointLights.begin(), m_pointLights.end(), light);
	//	if (findIt == m_pointLights.end()) {
	//		//V‹K“o˜^B
	//		m_pointLights.push_back(reinterpret_cast<CPointLight*>(light));
	//	}
	//	else {
	//		//Šù‚É“o˜^‚³‚ê‚Ä‚é‚æB
	//		TK_LOG("This light was already added");
	//		return;
	//	}
	//}
	/*else*/ if (typeInfo == typeid(CDirectionLight)) {
		//“o˜^Ï‚İ‚©’²‚×‚éB
		auto findIt = std::find(m_directionLights.begin(), m_directionLights.end(), light);
		if (findIt == m_directionLights.end()) {
			//V‹K“o˜^B
			m_directionLights.push_back(reinterpret_cast<CDirectionLight*>(light));
		}
		else {
			//Šù‚É“o˜^‚³‚ê‚Ä‚é‚æB
			//TK_LOG("This light was already added");
			return;
		}
	}
}


void CLightManager::RemoveLight(const CLightBase* light)
{
	const std::type_info& typeInfo = typeid(*light);
/*	if (typeInfo == typeid(CPointLight)) {
		m_pointLights.erase(
			std::remove(m_pointLights.begin(), m_pointLights.end(), light),
			m_pointLights.end()
		);
	}
	else*/ if (typeInfo == typeid(CDirectionLight)) {
		m_directionLights.erase(
			std::remove(m_directionLights.begin(), m_directionLights.end(), light),
			m_directionLights.end()
		);
	}
}