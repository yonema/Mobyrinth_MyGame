#pragma once

/// <summary>
/// フォント描画クラス
/// </summary>
class CFontRender : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;							//スタート関数
	~CFontRender();										//デストラクタ
	void Render(RenderContext& rc)override final;		//描画用関数
	void PostRender(RenderContext& rc)override final;	//一番上に描画する関数

public:		//ここのメンバ関数を主に使う。

	/// <summary>
	/// 初期化関数。最初に呼んでね。
	/// </summary>
	/// <param name="text">表示するテキスト</param>
	/// <param name="position">座標。{0.0f,0.0f}が中央</param>
	/// <param name="color">カラー{1.0f,1.0f,1.0f,1.0f}が白色</param>
	/// <param name="rotation">回転</param>
	/// <param name="scale">拡大</param>
	/// <param name="pivot">ピボット（基点）</param>
	void Init
	(const wchar_t* text,
		Vector2 position = { 0.0f,0.0f },
		Vector4 color = { 1.0f,1.0f,1.0f,1.0f },
		float rotation = 0.0f,
		float scale = 1.0f,
		Vector2 pivot = { 0.0f,0.0f }
	);

	/// <summary>
	/// 表示するテキストを設定する
	/// </summary>
	/// <param name="text">テキスト</param>
	void SetText(const wchar_t* text);

	/// <summary>
	/// 表示するテキストをフォーマット指定して設定する。
	/// テンプレートでintとかフォーマットを指定して、
	/// textの中でも%dみたいに指定して、
	/// 両方で指定する。
	/// </summary>
	/// <typeparam name="T">フォーマット指定</typeparam>
	/// <param name="text">テキスト</param>
	/// <param name="var">表示する変数</param>
	template <typename T>
	void SetText(const wchar_t* text, const T var)
	{
		swprintf_s(m_text, text, var);
	}

	/// <summary>
	/// 場所を設定する。{0.0f,0.0f}が中央
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector2& pos)
	{
		m_position = pos;
	}

	/// <summary>
	/// 座標を取得する
	/// </summary>
	/// <returns>座標</returns>
	const Vector2& GetPosition() const
	{
		return m_position;
	}

	/// <summary>
	/// カラーを設定する。{1.0f,1.0f,1.0f,1.0f}が白色
	/// </summary>
	/// <param name="color">カラー</param>
	void SetColor(const Vector4& color)
	{
		m_color = color;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const float rotation)
	{
		m_rotation = rotation;
	}

	/// <summary>
	/// 拡大を設定する
	/// </summary>
	/// <param name="scale">拡大</param>
	void SetScale(const float scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// ピボット（基点）を設定する
	/// </summary>
	/// <param name="pivot">ピボット（基点）</param>
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
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


	void SetShadowParam(const bool isDrawShadow, const float shadowOffset, const Vector4& shadowColor)
	{
		m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
	}


private:
	static const int m_maxTextSize = 256;
private:	//データメンバ
	Font m_font;								//フォントクラス
	wchar_t m_text[m_maxTextSize];				//表示するテキスト
	Vector2 m_position = { 0.0f,0.0f };			//表示する場所。{0.0f,0.0f}が中央
	Vector4 m_color = { 1.0f,1.0f,1.0f,1.0f };	//カラー{1.0f,1.0f,1.0f,1.0f}が白色
	float m_rotation = 0.0f;					//回転
	float m_scale = 1.0f;						//拡大
	Vector2 m_pivot = { 0.0f,0.0f };			//ピボット（基点）

	bool m_postRenderFlag = false;				//PostRenderで描画するかどうか？
};

