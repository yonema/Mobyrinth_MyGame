#pragma once
#include "stdafx.h"

//ライトの共通パラメータの構造体
struct SLightParam {
	Vector3 eyePos;				//視線の位置。
	int numDirectionLight;		//ディレクションライトの数。
	Vector3 ambientLight;		//アンビエントライト。
	int numPointLight;			//ポイントライトの数。
	float specPow;				//スペキュラの絞り
	int numShadow;	//
};

//ディレクションライトのパラメータの構造体
struct SDirectionLight
{
	Vector4 dirLigColor = { 0.0f,0.0f,0.0f,0.0f };	//ライトのカラー
	Vector3 dirLigDirection = { 0.0f,0.0f,0.0f };	//ライトの方向
	float pad = 0.0f;								//パッド。念のため
};

//ポイントライトのパラメータの構造体
struct SPointLight
{
	Vector3 ptPosition;		//位置。
	float ptRange;			//影響範囲。
	Vector4 ptColor;		//カラー。
};

const int g_max_shadowMap = 5;

//シャドウマップ用のパラメータ構造体
struct ShadowParam
{
	Matrix mLVP;		//ライトビュープロジェクション
	Vector3 lightPos;	//ライトの位置
};