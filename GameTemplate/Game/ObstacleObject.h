#pragma once
#include "LevelObjectBase.h"
#include "ModelRender.h"

class CObstacleObject : public ILevelObjectBase
{
protected:
	bool Init(const char* filePath, int objectType);
public:
	bool PureVirtualStart()override final;
	virtual bool StartSub() { return true; };
	void Update()override final;
	virtual ~CObstacleObject();
private:
	CModelRender* m_modelRender = nullptr;
};

