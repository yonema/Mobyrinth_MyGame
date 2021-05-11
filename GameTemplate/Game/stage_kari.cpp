#include "stdafx.h"
#include "stage_kari.h"

bool stage_kari::StartSub()
{
	//サンドボックスステージ
	//LoadLevel("Assets/level/stage_kari02.tkl");

	//本番用ステージ確認用
	//どれか一個のLoadLevelのコメントを消して確認してね。
	//その他のLoadLevelは全部コメントアウトしてね。

	//制作者Y
	LoadLevel("Assets/level/Y_easy.tkl");
	//LoadLevel("Assets/level/Y_normal.tkl");
	//LoadLevel("Assets/level/Y_hard.tkl");

	//製作者O
	//LoadLevel("Assets/level/O_easy.tkl");
	//LoadLevel("Assets/level/O_normal.tkl");
	//LoadLevel("Assets/level/O_hard.tkl");	

	//製作者T
	//LoadLevel("Assets/level/T_easy.tkl");
	//LoadLevel("Assets/level/T_normal.tkl");
	//LoadLevel("Assets/level/T_hard.tkl");


	
	return true;
}
stage_kari::~stage_kari()
{

}


