#pragma once
#include "SpriteRenderConstData.h"


/// <summary>
/// スプライト描画クラス
/// </summary>
class CSpriteRender : public IGameObject
{
public:		//自動でよばれるメンバ関数
	bool Start()override final;								//スタート関数
	~CSpriteRender();										//デストラクタ
	void Update()override final;							//アップデート関数
	void ForwardRender(RenderContext& rc)override final;			//描画用関数

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

	/**
	 * @brief 初期化用関数。幅の引数が<float>バージョン。最初に呼んでね。
	 * @attention 幅はUINTにキャストされるので、小数点以下のデータは失われる。
	 * @param [in] texFilePath ddsファイルパス
	 * @param [in] w スプライトの横幅
	 * @param [in] h スプライトの縦幅
	 * @param [in] pivot ピボット（基点）
	 * @param [in] alphaBlendMode アルファブレンディングモード
	*/
	void Init(
		const char* texFilePath,
		const float w,
		const float h,
		const Vector2& pivot = spriteRenderConstData::SPRITE_PIVOT_DEFALUT,
		AlphaBlendMode alphaBlendMode = AlphaBlendMode_None
	);

	/**
	 * @brief 初期化用関数。幅の引数が<int>バージョン。最初に呼んでね。
	 * @param [in] texFilePath ddsファイルパス
	 * @param [in] w スプライトの横幅
	 * @param [in] h スプライトの縦幅
	 * @param [in] pivot ピボット（基点）
	 * @param [in] alphaBlendMode アルファブレンディングモード
	*/
	void Init(
		const char* texFilePath,
		const int w,
		const int h,
		const Vector2& pivot = spriteRenderConstData::SPRITE_PIVOT_DEFALUT,
		AlphaBlendMode alphaBlendMode = AlphaBlendMode_None
	);


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
		m_scale.x *= spriteRenderConstData::FLIP_HORIZONTAL;
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

	/**
	 * @brief スプライトのアルファ値を設定する
	 * @param [in] alphaValue アルファ値
	*/
	void SetAlphaValue(const float alphaValue)
	{
		m_sprite.SetAlphaValue(alphaValue);
	}

	/// <summary>
	/// m_positionの値を返す。
	/// </summary>
	/// <returns>m_position</returns>
	const Vector3& GetPosition() const
	{
		return m_position;
	}

	/**
	 * @brief 乗算カラーを得る
	 * @return 乗算カラー
	*/
	const Vector4& GetMulColor() const
	{
		return m_sprite.GetMulColor();
	}

private:	//privateなメンバ関数

	/**
	 * @brief 初期化用関数のメインコア。Init関数の中で呼ばれる。
	 * @param [in] texFilePath ddsファイルパス
	 * @param [in] w スプライトの横幅
	 * @param [in] h スプライトの縦幅
	 * @param [in] pivot ピボット（基点）
	 * @param [in] alphaBlendMode アルファブレンディングモード
	*/
	void InitMainCore(
		const char* texFilePath,
		const UINT w,
		const UINT h,
		const Vector2& pivot,
		AlphaBlendMode alphaBlendMode
	);

private:	//データメンバ
	Sprite m_sprite;							//スプライトクラス
	SpriteInitData m_initData;					//スプライトの初期化データ。構造体。
	
	Vector3 m_position = g_VEC3_ZERO;	//場所
	Quaternion m_rotation = g_QUAT_IDENTITY;		//回転
	//座標系の関係で左右反転するから
	//xの拡大を反転させる
	Vector3 m_scale = spriteRenderConstData::SPRITE_SCALE_DEFAULT;	//拡大
	Vector2 m_pivot = spriteRenderConstData::SPRITE_PIVOT_DEFALUT;	//ピボット（基点）
	
};

