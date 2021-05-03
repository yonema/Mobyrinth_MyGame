#pragma once
#include "Title.h"
#include "Tips.h"

/// <summary>
/// ゲームクラス
/// </summary>
class Game : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;							//スタート関数
	~Game();											//デストラクタ

	//デバック用
	//後で消す
	//void PostRender(RenderContext& rc)override final;	//一番上に描画する関数
	void Update()override final;						//アップデート関数


private:	//データメンバ

	//デバック用
	//後で消す
	std::vector<ILevelObjectBase*> m_lb;	//ILevelObjectBaseの情報を受け取る。

};

