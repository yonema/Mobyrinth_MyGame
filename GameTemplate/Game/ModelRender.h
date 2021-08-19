#pragma once
#include "ModelRenderConstData.h"
#include "LightManager.h"
#include "ShadowModel.h"
#include "GameTime.h"

/// <summary>
/// モデル描画クラス
/// </summary>
class CModelRender : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	void Update() override final;					//アップデート関数
	void Render(RenderContext& rc) override final;	//描画用関数
	void ForwardRender(RenderContext& rc) override final;
	void AlwaysUpdate() override final;				//常に呼ばれるアップデート関数

public:		//メンバ関数
	/// <summary>
	/// デフォルトの初期化処理関数
	/// 最初に呼んで！
	/// </summary>
	/// <param name="filePath">tkmのファイルパス</param>
	/// <param name="animationClips">アニメーションクリップ</param>
	/// <param name="numAnimationClips">アニメーションクリップの総数</param>
	/// <param name="modelUpAxis">モデルのUP軸</param>
	void Init(const char* filePath,
		D3D12_CULL_MODE cullMode = D3D12_CULL_MODE_BACK,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0,
		EnModelUpAxis modelUpAxis = enModelUpAxisZ
		);
	
	/// <summary>
	/// カスタム用の初期化関数
	/// 最初に呼んで！
	/// デフォルトのModelInitDataとは違う内容で初期化するときに使う
	/// </summary>
	/// <param name="initData">初期化用のデータ</param>
	/// <param name="animationClips">アニメーションクリップ</param>
	/// <param name="numAnimationClips">アニメーションクリップの総数</param>
	void Init(ModelInitData initData,
		AnimationClip* animationClips = nullptr,
		int numAnimationClips = 0
	);

	/// <summary>
	/// 場所を設定
	/// </summary>
	/// <param name="pos">場所</param>
	void SetPosition(const Vector3& pos)
	{
		m_position = pos;
		return;
	}

	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
		return;
	}

	/// <summary>
	/// 拡大を設定
	/// </summary>
	/// <param name="scale">拡大</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
		return;
	}

	/// <summary>
	/// アニメーションを再生する
	/// </summary>
	/// <param name="animNo">登録してあるアニメーションクリップの番号</param>
	/// <param name="interpolateTime">補完時間（単位：秒）</param>
	void PlayAnimation(
		int animNo,
		float interpolateTime = modelRenderConstData::INTERPOLATE_TIME_DEFAULT
	)
	{
		m_animationPtr->Play(animNo, interpolateTime);
		return;
	}

	/// <summary>
	/// シャドウレシーバーフラグを設定
	/// </summary>
	/// <param name="shadowReceiverFlag">シャドウレシーバー？</param>
	void SetShadowReceiverFlag(const bool shadowReceiverFlag)
	{
		m_model.SetShadowReceiverFlag(shadowReceiverFlag);
		return;
	}

	/// <summary>
	/// シャドウキャスターフラグを設定
	/// </summary>
	/// <param name="shadowCasterFlag">シャドウキャスター？</param>
	void SetShadowCasterFlag(const bool shadowCasterFlag);

	/// <summary>
	/// 自己発光色を設定する
	/// </summary>
	/// <param name="color">自己発光色</param>
	void SetModelEmissionColor(const Vector4& color)
	{
		m_model.SetModelEmissionColor(color);
		return;
	}

	/// <summary>
	/// 乗算カラーを設定する
	/// </summary>
	/// <param name="color">乗算カラー</param>
	void SetMulColor(const Vector4& color)
	{
		m_model.SetMulColor(color);
		return;
	}

	/**
	 * @brief アルファ値を設定する
	 * @param [in] alphaValue アルファ値
	*/
	void SetAlphaValue(const float alphaValue)
	{
		m_model.SetAlphaValue(alphaValue);
		return;
	}

	/// <summary>
	/// 輪郭線を描画するか？を設定する
	/// </summary>
	/// <param name="drawOutLineFlag">輪郭線を描画するか？</param>
	void SetDrawOutLineFlag(const bool drawOutLineFlag)
	{
		m_drawOutLineFlag = drawOutLineFlag;
		m_model.SetOutLineFlag(drawOutLineFlag);
		return;
	}



	/// <summary>
	/// ステルスする？を設定する
	/// </summary>
	/// <param name="stealthFlag">ステルスする？</param>
	void SetStealthFlag(const bool stealthFlag)
	{
		m_model.SetStealthFlag(stealthFlag);
		return;
	}

	//モデルの参照を返す
	Model& GetModel()
	{
		return m_model;
	}

	/// <summary>
	/// アニメーションの再生中？
	/// </summary>
	/// <returns></returns>
	bool IsPlayingAnimation() const
	{
		return m_animationPtr->IsPlaying();
	}


private:	//privateなメンバ関数

	/// <summary>
	/// スケルトンを初期化。
	/// tkmファイルパスをtksファイルパスに変換する
	/// </summary>
	/// <param name="filePath">tkmファイルパス</param>
	/// <returns>初期化が成功したか？</returns>
	bool InitSkeleton(const char* filePath);

	/// <summary>
	/// モデルを初期化。
	/// </summary>
	/// <param name="filePath">tkmファイルパス</param>
	/// <param name="modelUpAxis">モデルのUP軸</param>
	void InitModel(const char* filePath, D3D12_CULL_MODE cullMode, EnModelUpAxis modelUpAxis);

	/// <summary>
	/// アニメーションを初期化。
	/// </summary>
	/// <param name="animationClips">アニメーションクリップ</param>
	/// <param name="numAnimationClips">アニメーションクリップの総数</param>
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips);

	/// <summary>
	/// シャドウ用のモデルを初期化
	/// </summary>
	void InitShadowModel();

	/// <summary>
	/// Zプリパス用のモデルを初期化
	/// </summary>
	void InitZPrepassModel();


private://データメンバ
	Model m_model;								//モデル表示処理
	std::unique_ptr<Skeleton> m_skeletonPtr;	//スケルトン。
	std::unique_ptr<Animation> m_animationPtr;	//アニメション再生処理。
	const char* m_tkmFilePath = nullptr;		//tkmファイルパス

	Vector3 m_position = g_VEC3_ZERO;			//位置
	Quaternion m_rotation = g_QUAT_IDENTITY;		//回転
	Vector3 m_scale = g_VEC3_ONE;				//拡大

	CShadowModel m_shadowModel;					//シャドウを描画する用のモデル
	Model m_zprepassModel;						// ZPrepassで描画されるモデル

	bool m_isInited = false;					//初期化済み？
	bool m_drawOutLineFlag = false;				//輪郭線を描画するか？
};

