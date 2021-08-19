#pragma once

/**
 * @brief モデルレンダラーの定数データ
*/
namespace modelRenderConstData
{
	constexpr float ALPHA_VALUE_TRANSPARENT = 0.0f;					//透明のアルファ値
	constexpr float ALPHA_VALUE_OPACITY = 1.0f;						//不透明のアルファ値

	//アニメーションのデフォルトの保管時間
	constexpr float INTERPOLATE_TIME_DEFAULT = 0.2f;


	//tkmのファイル拡張子
	constexpr const char* const FILE_EXTENSION_TKM = ".tkm";
	//tksのファイル拡張子
	constexpr const char* const FILE_EXTENSION_TKS = ".tks";
	//拡張子の長さ
	constexpr size_t FILE_EXTENSION_LENGHT = 4;

	//ピクセルシェーダ―のデフォルトのエントリーポイント
	constexpr const char* const ENTRY_POINT_FUNC_PS_DEFAULT = "PSMain";
	//頂点シェーダーのデフォルトのエントリーポイント
	constexpr const char* const ENTRY_POINT_FUNC_VS_DEFAULT = "VSMain";
	//スキンありの頂点シェーダーのデフォルトのエントリーポイント
	constexpr const char* const ENTRY_POINT_FUNC_VS_SKIN_DEFAULT = "VSSkinMain";
	//シェーダーのデフォルトのファイルパス
	constexpr const char* const SHADER_FILEPATH_DEFAULT = "Assets/shader/defferdRenderModel.fx";

	//シェーダーのシャドウマップのファイルパス
	constexpr const char* const SHADER_FILTPATH_SHADOW_MAP = "Assets/shader/DrawShadowMap.fx";

	//シェーダーのZプリパスのファイルパス
	constexpr const char* const SHADER_GILEPATH_Z_PREPASS = "Assets/shader/ZPrepass.fx";
}