#pragma once

/// <summary>
/// シャドウ用モデルクラス
/// </summary>
class CShadowModel : private Noncopyable
{
public:		//自動で呼ばれるメンバ関数
	CShadowModel();		//コンストラクタ
	~CShadowModel();	//デストラクタ

public:		//メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// モデルの初期化関数
	/// </summary>
	/// <param name="initData">モデルの初期化データ</param>
	void InitModel(ModelInitData& initData);

	/// <summary>
	/// モデルのパラメータ更新
	/// </summary>
	/// <param name="pos">場所</param>
	/// <param name="rot">回転</param>
	/// <param name="scale">拡大</param>
	void UpdateModel(const Vector3& pos, const Quaternion& rot, const Vector3& scale);

	/// <summary>
	/// シャドウ用のモデルの登録を消去する
	/// </summary>
	void RemoveShadowModel();

	/// <summary>
	/// 有効かどうか？
	/// </summary>
	/// <returns>有効か？</returns>
	const bool IsValid() const
	{
		return m_isValid;
	}

private:	//データメンバ
	std::unique_ptr<Model> m_model = nullptr;	//モデル
	bool m_isValid = false;						//有効か？
};

