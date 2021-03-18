#include "stdafx.h"
#include "ModelRender.h"
#include "LightManager.h"


/// <summary>
/// デフォルトの初期化処理関数
/// 最初に呼んで！
/// </summary>
/// <param name="filePath">tkmのファイルパス</param>
/// <param name="animationClips">アニメーションクリップ</param>
/// <param name="numAnimationClips">アニメーションクリップの総数</param>
/// <param name="modelUpAxis">モデルのUP軸</param>
void CModelRender::Init(
	const char* filePath,
	AnimationClip* animationClips,
	int numAnimationClips,
	EnModelUpAxis modelUpAxis)
{
	//スケルトンのデータの読み込み。
	InitSkeleton(filePath);
	//モデルを初期化
	InitModel(filePath, modelUpAxis);
	//アニメーションを初期化
	InitAnimation(animationClips, numAnimationClips);

	//初期化完了
	m_isInited = true;
}


/// <summary>
/// カスタム用の初期化関数
/// 最初に呼んで！
/// デフォルトのModelInitDataとは違う内容で初期化するときに使う
/// </summary>
/// <param name="initData">初期化用のデータ</param>
/// <param name="animationClips">アニメーションクリップ</param>
/// <param name="numAnimationClips">アニメーションクリップの総数</param>
void CModelRender::Init(
	ModelInitData initData,
	AnimationClip* animationClips ,
	int numAnimationClips)
{
	//スケルトンのデータの読み込み。
	InitSkeleton(initData.m_tkmFilePath);
	//モデルを初期化
	m_model.Init(initData);
	//アニメーションを初期化
	InitAnimation(animationClips, numAnimationClips);

	//初期化完了
	m_isInited = true;
}


/// <summary>
/// スケルトンを初期化。
/// tkmファイルパスをtksファイルパスに変換する
/// </summary>
/// <param name="filePath">tkmファイルパス</param>
/// <returns>初期化が成功したか？</returns>
bool InitSkeleton(const char* filePath);
bool CModelRender::InitSkeleton(const char* filePath)
{
	//tkmファイルをtksファイルに変換する
	std::string skeletonFilePath = filePath;
	int pos = (int)skeletonFilePath.find(".tkm");
	skeletonFilePath.replace(pos, 4, ".tks");

	//スケルトンのリソースの確保
	m_skeletonPtr.reset(new Skeleton);
	//スケルトンのデータを読み込み。
	bool isInited = m_skeletonPtr->Init(skeletonFilePath.c_str());

	//初期化に成功したか？
	if (isInited)
		return true;	//成功したらtrueを返す
	else
	{
		//失敗したらスケルトンのリソースを開放して
		//falseを返す
		m_skeletonPtr.reset();
		return false;
	}
}


/// <summary>
/// モデルを初期化。
/// </summary>
/// <param name="filePath">tkmファイルパス</param>
/// <param name="modelUpAxis">モデルのUP軸</param>
void CModelRender::InitModel(const char* filePath, EnModelUpAxis modelUpAxis)
{
	//初期化データを作成する
	ModelInitData initData;
	//tkmファイルのファイルパスを指定する。
	initData.m_tkmFilePath = filePath;
	//シェーダーファイルのファイルパスを指定する。
	initData.m_fxFilePath = "Assets/shader/myModel.fx";
	//スケルトンを指定する。
	if (m_skeletonPtr)	//スケルトンが初期化されていたら
		initData.m_skeleton = m_skeletonPtr.get();
	//モデルの上方向を指定する。
	initData.m_modelUpAxis = modelUpAxis;

	//シェーダに渡すコンスタントバッファの設定

	//ライト共通のデータの登録
	initData.m_expandConstantBuffer
		= CLightManager::GetInstance()->GetLightParam();
	initData.m_expandConstantBufferSize
		= sizeof(*CLightManager::GetInstance()->GetLightParam());

	//ディレクションライト達のデータの登録
	initData.m_expandConstantBuffer2
		= CLightManager::GetInstance()->GetDirectionLigData();
	initData.m_expandConstantBufferSize2
		= sizeof(*CLightManager::GetInstance()->GetDirectionLigData())
		* CLightManager::GetMax_DirectionLight();


	//作成した初期化データをもとにモデルを初期化する、
	m_model.Init(initData);
}


/// <summary>
/// アニメーションを初期化。
/// </summary>
/// <param name="animationClips">アニメーションクリップ</param>
/// <param name="numAnimationClips">アニメーションクリップの総数</param>
void CModelRender::InitAnimation(AnimationClip* animationClips, int numAnimationClips)
{
	//アニメーションクリップが登録されているか
	if (animationClips == nullptr)
		return;	//されていない場合は何もしない

	//アニメーションのリソースの確保
	m_animationPtr.reset(new Animation);
	//アニメーションを初期化。
	m_animationPtr->Init(
		*m_skeletonPtr,			//アニメーションを流し込むスケルトン。
		animationClips,		//アニメーションクリップ。
		numAnimationClips	//アニメーションの数。
	);
}


//アップデート関数
void CModelRender::Update()
{
	//初期化されているか？
	if (!m_isInited)
		return;	//されていない場合は何もしない


	//スケルトンを更新。
	if (m_skeletonPtr)	//スケルトンが初期化されていたら
		m_skeletonPtr->Update(m_model.GetWorldMatrix());
	//アニメーションを進める。
	if (m_animationPtr)	//アニメーションが初期化されていたら
		m_animationPtr->Progress(1.0f / 60.0f);
	//モデルの座標更新
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);

}


//描画用関数
void CModelRender::Render(RenderContext& rc)
{
	//初期化されているか？
	if (!m_isInited)
		return;	//されていない場合は何もしない

	//モデルを描画
	m_model.Draw(rc);
}