#pragma once
class CLightBase : public IGameObject
{
public:
	CLightBase();
	virtual ~CLightBase();
private:
	/// <summary>
	/// ����Ȃ���������Ȃ����ǁA���ۃN���X�ɂ���������Ƃ肠���������
	/// �������z�֐��Breturn ture������̂��킷��Ȃ��ł�(false�ł���)
	/// </summary>
	/// <returns>bool�l�Ԃ��Ƃ��Ă�</returns>
	virtual bool StartSub() = 0 { return true; };
	virtual void OnDestroySub() {};
};

