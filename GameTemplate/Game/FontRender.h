#pragma once

//フォントレンダラーの定数データ
namespace fontRendetConstData
{
	//デフォルトの座標
	const Vector2 POSITION_DEFAULT = { 0.0f,0.0f };
	//デフォルトのカラー
	const Vector4 COLOR_DEFAULT = g_VEC4_WHITE;
	//デフォルトのピボット
	const Vector2 PIVOT_DEFAULT = { 0.0f,0.0f };
	//テキストの最大のサイズ
	constexpr UINT SIZE_MAX_TEXT = 256;
}

/// <summary>
/// フォント描画クラス
/// </summary>
class CFontRender : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;							//スタート関数
	~CFontRender();										//デストラクタ
	void Render(RenderContext& rc)override final;		//描画用関数

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
		Vector2 position = fontRendetConstData::POSITION_DEFAULT,
		Vector4 color = fontRendetConstData::COLOR_DEFAULT,
		float rotation = 0.0f,
		float scale = 1.0f,
		Vector2 pivot = fontRendetConstData::PIVOT_DEFAULT
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
		return;
	}

	/// <summary>
	/// 場所を設定する。{0.0f,0.0f}が中央
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector2& pos)
	{
		m_position = pos;
		return;
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
		return;
	}

	/// <summary>
	/// 回転を設定する
	/// </summary>
	/// <param name="rotation">回転</param>
	void SetRotation(const float rotation)
	{
		m_rotation = rotation;
		return;
	}

	/// <summary>
	/// 拡大を設定する
	/// </summary>
	/// <param name="scale">拡大</param>
	void SetScale(const float scale)
	{
		m_scale = scale;
		return;
	}

	/// <summary>
	/// ピボット（基点）を設定する
	/// </summary>
	/// <param name="pivot">ピボット（基点）</param>
	void SetPivot(const Vector2& pivot)
	{
		m_pivot = pivot;
		return;
	}


	void SetShadowParam(const bool isDrawShadow, const float shadowOffset, const Vector4& shadowColor)
	{
		m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		return;
	}

private:	//データメンバ
	Font m_font;								//フォントクラス

	wchar_t m_text[fontRendetConstData::SIZE_MAX_TEXT];			//表示するテキスト
	Vector2 m_position = fontRendetConstData::POSITION_DEFAULT;	//表示する場所。{0.0f,0.0f}が中央
	Vector4 m_color = fontRendetConstData::COLOR_DEFAULT;		//カラー{1.0f,1.0f,1.0f,1.0f}が白色
	float m_rotation = 0.0f;									//回転
	float m_scale = 1.0f;										//拡大
	Vector2 m_pivot = fontRendetConstData::PIVOT_DEFAULT;		//ピボット（基点）

};

