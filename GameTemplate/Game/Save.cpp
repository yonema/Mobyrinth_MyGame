#include "stdafx.h"
#include "Save.h"

//コンストラクタ
CSave::CSave()
{
	//ロードする
	LoadData();
}

/// <summary>
/// データメンバのセーブデータをセーブする
/// </summary>
void CSave::SaveData()
{
	//ファイルをバイナリで上書きモードで開く
	auto fp = fopen(SAVE_DATA_FILEPATH, MODE_WRITE_BINARY);

	//セーブデータの一番高いクリアしたステージの番号をファイルに書き込む
	fwrite(
		&m_saveData.highestClearStageNum,
		sizeof(m_saveData.highestClearStageNum),
		1,
		fp
	);

	//ファイルを閉じる
	fclose(fp);

}

/// <summary>
/// セーブデータをデータメンバにロードする
/// </summary>
/// <returns>ロードできたか？</returns>
const bool CSave::LoadData()
{
	//ファイルをバイナリで読み込みモードで開く
	auto fp = fopen(SAVE_DATA_FILEPATH, MODE_READ_BINARY);

	//開けたか？
	if (!fp)
	{
		//開けていなかったら
		//エラーコード（負の数）を入れて
		m_saveData.highestClearStageNum = -1;
		//ロードできなかったからfalseを戻す
		return false;
	}
	//開けた

	//一番高いクリアしたステージの番号のバッファー
	int highestClearStageBuf;

	//一番高いクリアしたステージの番号をファイルから読み込む
	fread(&highestClearStageBuf, sizeof(highestClearStageBuf), 1, fp);

	//データメンバに代入する
	m_saveData.highestClearStageNum = highestClearStageBuf;

	//ファイルを閉じる
	fclose(fp);

	//ロードできたから
	//trueを戻す
	return true;
}