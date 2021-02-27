#pragma once
class CModelRender : IGameObject
{
public:
	void Init(const char* filePath);
private:	//メンバ関数
	/// <summary>
	/// スケルトンを初期化。
	/// </summary>
	void InitSkeleton(const char* filePath);
	/// <summary>
	/// モデルを初期化。
	/// </summary>
	void InitModel(const char* filePath);
private:	//データメンバ
	Model m_model;	//モデル表示処理
	Skeleton m_skeleton;	//スケルトン。
};

