#include "stdafx.h"
#include "OOpadlock.h"

//ObstacleObjectのモデルのファイルパスとOBBのサイズの定数データを使用可能にする
using namespace OOsFilepathAndObbSizeConstData;

//スタート関数
bool OOpadlock::StartSub()
{
	//初期化用関数
	Init(MODEL_FILEPATH_PADLOCK, EN_OO_TYPE_BIG_PADLOCK);

	//OBBのサイズを設定
	SetOBBDirectionLength(SIZE_OBB_PADLOCK);

	//OBBのタグを設定
	SetOBBTag(COBB::EN_RO_TYPE_PADLOCK);

	return true;
}