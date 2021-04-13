#pragma once

#include "HID/GamePad.h"
#include "TResourceBank.h"
#include "tkFile/TkmFile.h"
#include "Shader.h"


class GraphicsEngine;

class TkEngine {
public:
	/// <summary>
	/// デストラクタ。
	/// </summary>
	~TkEngine();
	/// <summary>
	/// フレームの開始時に呼ばれる処理。
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// フレームの終了時に呼ばれる処理。
	/// </summary>
	void EndFrame();
	/// <summary>
	/// ゲームエンジンの初期化。
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
	/// <summary>
	/// tkmファイルをバンクから取得。
	/// </summary>
	/// <param name="filePath">ファイルパス</param>
	/// <returns>tkmファイル。nullptrが返ってきたらバンクに登録されていない。</returns>
	TkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return m_tkmFileBank.Get(filePath);
	}
	/// <summary>
	/// tkmファイルをバンクに登録。
	/// </summary>
	/// <param name="filePath">tkmファイルパス</param>
	/// <param name="tkmFile"></param>
	void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
	{
		m_tkmFileBank.Regist(filePath, tkmFile);
	}
	/// <summary>
	/// シェーダーファイルバンクからシェーダーを取得。
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return m_shaderBank.Get(programName.c_str());
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="shader"></param>
	void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		m_shaderBank.Regist(programName.c_str(), shader);
	}
private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//グラフィックエンジン。
	TResourceBank<TkmFile> m_tkmFileBank;			//tkmファイルバンク。
	TResourceBank<Shader> m_shaderBank;				//シェーダーバンク
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//ゲームパッド。

	//追加
public:		//メンバ関数
	/// <summary>
	/// シャドウを描画する
	/// </summary>
	void DrawShadow();

	/// <summary>
	/// メインレンダーターゲットを使用できるようにする
	/// </summary>
	void UseMainRenderTarget();

	/// <summary>
	/// ポストエフェクトを描画する
	/// </summary>
	void DrawPostEffect();

	/// <summary>
	/// メインレンダリングターゲットの絵をフレームバッファーに描画する
	/// </summary>
	void DrawFrameBuffer();

	/// <summary>
	/// 影を生成するライトを生成する
	/// </summary>
	/// <param name="direction">影を作るライトの方向</param>
	/// <param name="length">ライトがどれくらい離れているか</param>
	/// <param name="target">ライトが照らす目標</param>
	void CreateShadow
	(const Vector3& direction, const float length = 300.0f, const Vector3& target = g_vec3Zero);

	/// <summary>
	/// 影を生成するライトのパラメーター設定する
	/// </summary>
	/// <param name="direction">影を作るライトの方向</param>
	/// <param name="length">ライトがどれくらい離れているか</param>
	/// <param name="target">ライトが照らす目標</param>
	void SetShadowPararm
	(const Vector3& direction, const float length = 300.0f, const Vector3& target = g_vec3Zero);
};

extern TkEngine* g_engine;	//TKエンジン。