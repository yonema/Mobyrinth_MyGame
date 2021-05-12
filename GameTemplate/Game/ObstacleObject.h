#pragma once
#include "LevelObjectBase.h"
#include "ModelRender.h"
#include "OBBWorld.h"


/// <summary>
/// オブステークルオブジェクト（障害オブジェクト）（読み方あってる？）
/// 障害オブジェクトを作るときはこのクラスを継承してね
/// </summary>
class CObstacleObject : public ILevelObjectBase
{
public:		//自動で呼ばれるメンバ関数
	bool PureVirtualStart()override final;		//スタート関数
	virtual ~CObstacleObject();					//デストラクタ
	void PureVirtualUpdate()override final;		//アップデート関数

public:		//オーバーライドしてほしいメンバ関数
	virtual bool StartSub() { return true; };	//スタート関数
	virtual void UpdateSub() {};				//アップデート関数

protected:	//ここのメンバ関数を主に使う

	/// <summary>
	/// 初期化関数
	/// 最初に呼んでね。trueを戻してね。
	/// モデルのファイルパスとオブジェクトのタイプを
	/// 設定する
	/// タイプ一覧はObjectType.hを参照
	/// </summary>
	/// <param name="filePath">モデルのtkmファイルパス</param>
	/// <param name="objectType">タイプ</param>
	/// <returns>trueを戻してね</returns>
	bool Init(const char* filePath, int objectType);

	/// <summary>
	/// 自己発光色を設定する
	/// </summary>
	/// <param name="color">自己発光色</param>
	void SetEmissionColor(const Vector4& color)
	{
		m_modelRender->SetEmissionColor(color);
	}

	/// <summary>
	/// ゲームオブジェクトをアクティブにする
	/// </summary>
	void Activate()
	{
		m_modelRender->Activate();
	}

	/// <summary>
	/// ゲームオブジェクトを非アクティブにする
	/// </summary>
	void Deactivate()
	{
		m_modelRender->Deactivate();
	}

private:	//データメンバ
	CModelRender* m_modelRender = nullptr;	//モデルレンダラー
};

