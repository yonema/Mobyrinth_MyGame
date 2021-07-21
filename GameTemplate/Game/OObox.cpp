#include "stdafx.h"
#include "OObox.h"

//ObstacleObjectのモデルのファイルパスとOBBのサイズの定数データを使用可能にする
using namespace OOsFilepathAndObbSizeConstData;

//スタート関数
bool OObox::StartSub()
{
	//初期化用関数
	Init(MODEL_FILEPATH_BOX, EN_OO_TYPE_BOX);

	//OBBのサイズを設定
	SetOBBDirectionLength(OBB_SIZE_BOX);

	//OBBのタグを設定
	SetOBBTag(COBB::EN_OO_TYPE_BOX);

	return true;
}