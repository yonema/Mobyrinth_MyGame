#pragma once
#include "stdafx.h"
struct SLightParam {
	Vector3 eyePos;				//視線の位置。
	int numDirectionLight;		//ディレクションライトの数。
	Vector3 ambientLight;		//アンビエントライト。
	int numPointLight;			//ポイントライトの数。
	float specPow;				//スペキュラの絞り
};
struct SDirectionLight
{
	Vector4 dirLigColor = { 0.0f,0.0f,0.0f,0.0f };
	Vector3 dirLigDirection = { 0.0f,0.0f,0.0f };
	float pad = 0.0f;
	////ディレクションライト
	//Vector3 dirLigDirection;
	//float pad;
	//Vector4 dirLigColor;
	//Vector3 eyePos;			//カメラの位置。
	//float specPow;				//スペキュラの絞り。
	//Vector3 ambinetLight;	//環境光。	
};

//ポイントライトの構造体
struct SPointLight
{
	Vector3 ptPosition;		//位置。
	float ptRange;			//影響範囲。
	Vector4 ptColor;		//カラー。
};