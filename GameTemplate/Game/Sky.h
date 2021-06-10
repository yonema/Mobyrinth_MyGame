#pragma once

/**
 * @brief 空クラスの定数データ
*/
namespace
{
	//空のモデルのファイルパス
	const char* const MODEL_FILEPATH_SKY = "Assets/modelData/preset/sky.tkm";
	//空のシェーダーファイルパス
	const char* const FX_FILEPATH_SKY = "Assets/shader/SkyCubeMap.fx";
	//空のテクスチャのファイルパス
	const wchar_t* const TEXTURE_FILEPATH_SKY = L"Assets/modelData/preset/sky.dds";
}

/**
 * @brief 空クラス
*/
class CSky : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	CSky();		//コンストラクタ
	~CSky();	//デストラクタ
	bool Start()override final;		//スタート関数
	void Update()override final;	//アップデート関数
	void Render(RenderContext& rc)override final;	//描画関数

public:		//メンバ関数

	/**
	 * @brief 座標を設定
	 * @param [in] pos 座標
	*/
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		//値が変わったから、更新が必要
		m_isDirty = true;
	}

	/**
	 * @brief 拡大率を設定（Vector3）
	 * @param [in] scale 拡大率
	*/
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		//値が変わったから、更新が必要
		m_isDirty = true;
	}

	/**
	 * @brief 拡大率を設定（float）
	 * @param [in] scale 拡大率
	*/
	void SetScale(const float scale)
	{
		//拡大率を初期化してから
		m_scale = g_VEC3_ONE;
		//拡大
		m_scale.Scale(scale);
		//値が変わったから、更新が必要
		m_isDirty = true;
	}

	/**
	 * @brief 自己発光カラーを設定
	 * @param [in] color 自己発光から―
	*/
	void SetEmissionColor(const Vector4 color)
	{
		m_model.SetEmissionColor(color);
		//値が変わったから、更新が
		m_isDirty = true;
	}

private:	//データメンバ
	Model m_model;							//モデルクラス
	Texture m_skyCubeTexture;				//スカイキューブテクスチャ
	Vector3 m_position = g_VEC3_ZERO;		//座標
	Vector3 m_scale = g_VEC3_ONE;			//拡大率
	Vector4 m_selfLuminous = g_VEC4_BLACK;	//自己発光色
	bool m_isDirty = false;					//ダーティフラグ
};

