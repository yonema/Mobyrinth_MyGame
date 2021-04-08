#pragma once
#include "ShadowModel.h"

/// <summary>
/// モデル描画クラス
/// </summary>
class CModelRender : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	void Update() override final;					//アップデート関数
	void Render(RenderContext& rc) override final;	//描画用関数

public:		//ここのメンバ関数を主に使う。
	/// <summary>
	/// デフォルトの初期化処理関数
	/// 最初に呼んで！
	/// </summary>
	/// <param name="filePath">tkmのファイルパス</param>
	/// <param name="animationClips">アニメーションクリップ</param>
	/// <param name="numAnimationClips">アニメーションクリップの総数</param>
	/// <param name="modelUpAxis">モデルのUP軸</param>
	void Init(const char* filePath,
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
	}

	/// <summary>
	/// 回転を設定
	/// </summary>
	/// <param name="rot">回転</param>
	void SetRotation(const Quaternion& rot)
	{
		m_rotation = rot;
	}

	/// <summary>
	/// 拡大を設定
	/// </summary>
	/// <param name="scale">拡大</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	/// <summary>
	/// アニメーションを再生する
	/// </summary>
	/// <param name="animNo">登録してあるアニメーションクリップの番号</param>
	/// <param name="interpolateTime">補完時間（単位：秒）</param>
	void PlayAnimation(int animNo, float interpolateTime = 0.0f)
	{
		m_animationPtr->Play(animNo, interpolateTime);
	}

	void SetShadowReceiverFlag(const bool shadowReceiverFlag)
	{
		m_model.SetShadowReceiverFlag(shadowReceiverFlag);
	}
	void SetShadowCasterFlag(const bool shadowCasterFlag)
	{
		if (shadowCasterFlag && !m_shadowModel.IsValid())
			InitShadowModel();

		m_model.SetShadowCasterFlag(shadowCasterFlag);
	}

	//モデルの参照を返す
	Model& GetModel()
	{
		return m_model;
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
	void InitModel(const char* filePath, EnModelUpAxis modelUpAxis);

	/// <summary>
	/// アニメーションを初期化。
	/// </summary>
	/// <param name="animationClips">アニメーションクリップ</param>
	/// <param name="numAnimationClips">アニメーションクリップの総数</param>
	void InitAnimation(AnimationClip* animationClips, int numAnimationClips);

	void InitShadowModel();


private://データメンバ
	Model m_model;								//モデル表示処理
	std::unique_ptr<Skeleton> m_skeletonPtr;	//スケルトン。
	std::unique_ptr<Animation> m_animationPtr;	//アニメション再生処理。
	const char* m_tkmFilePath = nullptr;

	Vector3 m_position = g_vec3Zero;			//位置
	Quaternion m_rotation = g_quatIdentity;		//回転
	Vector3 m_scale = g_vec3One;				//拡大

	CShadowModel m_shadowModel;					//シャドウを描画する用のモデル


	bool m_isInited = false;					//初期化済み？
};

