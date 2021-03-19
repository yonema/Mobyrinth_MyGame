#include "stdafx.h"
#include "MapChip.h"
#include "Level.h"


CMapChip::CMapChip(const LevelObjectData& objData)
{
	//wcharをcharに変換
	char objName[256];
	wcstombs(objName, objData.name, 256);

	//ファイルパスを作成。
	char filePath[256];
	sprintf(filePath, "Assets/modelData/%s.tkm", objName);

	//モデルレンダラーを初期化
	m_modelRender = NewGO<CModelRender>(0);
	m_modelRender->Init(filePath);
	m_modelRender->SetPosition(objData.position);
	m_modelRender->SetRotation(objData.rotation);
	m_modelRender->SetScale(objData.scale);

	//静的物理オブジェクトの初期化
	m_physicsStaticObject.CreateFromModel
	(m_modelRender->GetModel(), m_modelRender->GetModel().GetWorldMatrix());
}

CMapChip::~CMapChip()
{
	if (m_modelRender)
		DeleteGO(m_modelRender);
}