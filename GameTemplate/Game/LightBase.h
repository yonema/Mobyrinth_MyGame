#pragma once
class CLightBase : public IGameObject
{
public:
	CLightBase();
	virtual ~CLightBase();
private:
	/// <summary>
	/// いらないかもしれないけど、抽象クラスにしたいからとりあえず作った
	/// 純粋仮想関数。return tureさせるのをわすれないでね(falseでも可)
	/// </summary>
	/// <returns>bool値返しといてね</returns>
	virtual bool StartSub() = 0 { return true; };
	virtual void OnDestroySub() {};
};

