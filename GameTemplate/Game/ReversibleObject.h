#pragma once
#include "LevelObjectBase.h"
class CReversibleObject : public ILevelObjectBase
{
protected:
	bool Init
	(const char* filePath_front, const int type_front,
		const char* filePath_back, const int type_back);
private:
	void CheckPlayer();
	void HeldPlayer();
	void Thrown();
	void Query();
	virtual void QuerySub() {};
public:
	bool PureVirtualStart()override final;
	virtual bool StartSub() { return true; };
	void Update()override final;
	virtual ~CReversibleObject();
	void SetFrontOrBack(const bool frontOrBack);

public:
	enum EnFrontAndBack
	{
		enFront,
		enBack,
		enFrontAndBackNum,
	};
private:
	bool m_frontOrBack = enFront;
	CModelRender* m_modelRender[enFrontAndBackNum] = { nullptr };
	int m_reversibleType[enFrontAndBackNum] = { enEnpty };
	enum EnObjectState
	{
		enCheckPlayer,
		enHeldPlayer,
		enThrown,
		enQuery,
	};
	int m_objectState = enCheckPlayer;
	int m_throwCounter = 0;
};

