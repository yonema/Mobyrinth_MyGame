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
	virtual void UpdateSub() {};
	virtual ~CReversibleObject();
	void Reverse();
	void SetFrontOrBack(const bool frontOrBack);
	virtual void SetFrontOrBackSub() {};
	const bool GetFrontOrBack()
	{
		return m_frontOrBack;
	}
	void SetBothModelActiveFlag(const bool activeFlag);

public:
	enum EnFrontAndBack
	{
		enFront,
		enBack,
		enFrontAndBackNum,
	};
private:
	bool m_frontOrBack = enFront;
	bool m_bothModelactiveFlag = true;
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

	Quaternion m_throwRot = g_quatIdentity;
};

