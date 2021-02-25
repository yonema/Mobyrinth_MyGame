#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"


CMapChip::CMapChip(const LevelObjectData& objData/*, CMapChipRender* mapChipRender*/)
{
	char objName[256];
	wcstombs(objName, objData.name, 256);

	//ファイルパスを作成。
	char filePath[256];
	sprintf(filePath, "Assets/modelData/%s.tkm", objName);
	//m_mapChipRender = mapChipRender;
	//m_physicsStaticObject.CreateMesh(
	//	objData.position,
	//	objData.rotation,
	//	objData.scale,
	//	mapChipRender->GetSkinModelRender()
	//);

	ModelInitData initData;
	initData.m_tkmFilePath = filePath;
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/model.fx";
	//頂点シェーダーのエントリーポイント
	initData.m_vsEntryPointFunc = "VSMain";

	m_model.Init(initData);
	m_model.UpdateWorldMatrix(objData.position, objData.rotation, objData.scale);
	m_physicsStaticObject.CreateFromModel(m_model, m_model.GetWorldMatrix());
}