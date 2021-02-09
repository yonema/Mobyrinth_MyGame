#pragma once
#include "stdafx.h"

struct SDirectionLight
{
	Vector4 dirLigColor;
	Vector3 dirLigDirection;
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