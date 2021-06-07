#pragma once

/// <summary>
/// スプライト描画クラス
/// </summary>
class CSpriteRender : public IGameObject
{
public:		//自動でよばれるメンバ関数
	bool Start()override final;								//スタート関数
	~CSpriteRender();										//デストラクタ
	void Update()override final;							//アップデート関数
	void Render(RenderContext& rc)override final;			//描画用関数
	void PostRender(RenderContext& rc)override final;		//一番上に描画する関数

public:		//ここのメンバ関数を主に使う

	/// <summary>
	/// 自動で呼ばれるアップデートではなく、
	/// 手動でスプライトの更新をしたいときに呼ぶ関数
	/// </summary>
	void UpdateSprite()
	{
		//スプライトのアップデート
		m_sprite.Update(m_position, m_rotation, m_scale, m_pivot);
	}

	/// <summary>
	/// 初期化用関数
	/// 最初に呼んでね。
	/// </summary>
	/// <param name="texFilePath">ddsファイルパス</param>
	/// <param name="w">スプライトの横幅</param>
	/// <param name="h">スプライトの縦幅</param>
	/// <param name="pivot">ピボット（基点）</param>
	/// <param name="alphaBlendMode">アルファブレンディングモード</param>
	void Init(const char* texFilePath, float w, float h, const Vector2& pivot = { 0.5f,0.5f },
		AlphaBlendMode alphaBlendMode = AlphaBlendMode_None);

	/// <summary>
	/// 場所を設定する。{0.0f,0.0f}が中央
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// 拡大を設定する
	/// </summary>
	/// <param name="scale">拡大</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		//座標系の関係で左右反転するから
		//xの拡大を反転させる
		m_scale.x *= -1.0f;
	}

	/// <summary>
	/// ピボット（基点）を設定する
	/// {0.0f,0.0f}が左下
	/// {0.5f,0.5f}が中央
	/// {1.0f,1.0f}が右上
	/// </summary>
	/// <param name="pivot">ピボット（基点）</param>
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
	}

	/// <summary>
	/// スプライトに色を乗算する関数。
	/// アルファ値を掛けて透過するときは、Initするときに
	/// アルファブレンディングモードを透過合成モードにしておいてね。
	/// </summary>
	/// <param name="mulColor">乗算する色</param>
	void SetMulColor(const Vector4& mulColor)
	{
		m_sprite.SetMulColor(mulColor);
	}

	/// <summary>
	/// PostRenderで描画するかどうか設定する関数
	/// 引数にtrueを入れるとPostRenderで描画されるようになり、
	/// 一番上に描画されるようになる。
	/// </summary>
	/// <param name="flag">PostRenderで描画するか？</param>
	void SetPostRenderFlag(const bool flag)
	{
		m_postRenderFlag = flag;
	}

	/// <summary>
	/// m_positionの値を返す。
	/// </summary>
	/// <returns>m_position</returns>
	Vector3 GetPosition()
	{
		return m_position;
	}

	float GetMulColorW()
	{
		return m_sprite.GetMulColorW();
	}

private:	//データメンバ
	Sprite m_sprite;							//スプライトクラス
	SpriteInitData m_initData;					//スプライトの初期化データ。構造体。
	
	Vector3 m_position = { 100.0f,0.0f,0.0f };	//場所
	Quaternion m_rotation = g_quatIdentity;		//回転
	//座標系の関係で左右反転するから
	//xの拡大を反転させる
	Vector3 m_scale = { -1.0f,1.0f,1.0f };		//拡大
	Vector2 m_pivot = { 0.5f,0.5f };			//ピボット（基点）
	
	bool m_postRenderFlag = false;				//PostRenderで描画するかどうか？
};

