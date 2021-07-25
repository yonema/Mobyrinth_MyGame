#pragma once

/**
 * @brief 反転オブジェクトの定数データ
*/
namespace reversibleObjectConstData
{
	//ウェイポイントからのZ方向への奥行
	constexpr float LENGHT_Z_FROMT_WEYPOINT = 100.0f;

	//オブジェクトが持ち上げられた時の高さ
	constexpr float HEIGHT_HELD_UP = 330.0f;

	/**
	 * @brief オブジェクトの現在のステート（状態）
	 * これでアップデート中の処理を割り振る
	*/
	enum EN_OBJECT_STATE
	{
		EN_CHECK_HELD_PLAYER,	//プレイヤーに持たれるかどうか調べる
		EN_LIFTED_PLAYER,		//プレイヤーに持ち上げられ中の状態
		EN_HELD_PLAYER,			//プレイヤーに持たれている状態
		EN_THROWN_DOWN,			//持っているオブジェクトを下に投げる関数
		EN_CANCEL,				//持っているオブジェクトをその場に置く
		EN_REPELLED,			//横に弾かれる
		EN_QUERY,				//クエリしてほしいタイミング

		EN_OVERLAP,				//障害オブジェクトに重ねっているかの確認
		//EN_OVERLAP_THROWN_DOWN, //オブジェクトが戻ってくる処理（縦）
	};

	/*
	* サウンド関連の定数
	*/

	//性質反転する時のSEのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_SE_CHANGE = L"Assets/sound/change.wav";
	//性質反転するときのSEの音量
	constexpr float SOUND_VOLUME_SE_CHANGE = 0.5f;

	//性質反転する時のSEのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_SE_THROW = L"Assets/sound/throw.wav";
	//性質反転するときのSEの音量
	constexpr float SOUND_VOLUME_SE_THROW = 0.5f;

	//キャパシティオーバー時のSEのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_SE_CAPACITY_OVER = 
		L"Assets/sound/capacity_over.wav";
	//キャパシティオーバー時のSEの音量
	constexpr float SOUND_VOLUME_SE_CAPACITY_OVER = 0.5f;


	/*
	* エフェクト関連の定数
	*/
	
	//全反転のエフェクトのファイルパス
	constexpr const char16_t* const EFFECT_FILEPATH_REVERSE_ALL = u"Assets/effect/reverseall2.efk";
	//全反転のエフェクトの拡大率
	constexpr float EFFECT_SCALE_REVERSE_ALL = 50.0f;
	//全反転のエフェクトの座標の高さ
	constexpr float EFFECT_POSITION_HEIGHT_REVERSE_ALL = 100.0f;


	//反転オブジェクトのOBBの初期サイズ
	const Vector3 OBB_SIZE_DEFAULT_REVERSIBLE_OBJECT = { 200.0f,200.0f,600.0f };


	/*
	* プレイヤーに持ち上がられ中関連の定数
	*/
	//プレイヤーに持ち上げられ中のタイム
	constexpr float TIME_LIFTED_PLAYER = 0.45f;
	//プレイヤーに持ち上げられ中の半分のタイム
	constexpr float TIME_LIFTED_PLAYER_HALF = TIME_LIFTED_PLAYER / 2.0f;
	//プレイヤーに持ち上げられ中の水平方向に加える座標
	constexpr float ADD_VECTOR_SCALE_HORIZONTAL_LIFTED_PLAYER = 150.0f;
	//プレイヤーに持ち上げられ中の、動く前に待つタイム
	constexpr float TIME_LIFTED_PLAYER_WAIT_BEFORE_MOVING = 0.1f;


	/*
	* 投げられ中関連の定数
	*/

	/**
	 * @brief 下に投げられるか、上に投げられるか
	*/
	enum EN_DOWN_OR_UP_THROWN
	{
		EN_START_THROWN,	//最初に投げられる
		EN_DOWN_THROWN,		//下に投げられる
		EN_UP_THROWN,		//上に投げられる
	};
	//下に投げられ中のタイム
	constexpr float TIME_THROWN_DOWN = 0.5f;
	//下に投げられ中の半分のタイム
	constexpr float TIME_THROWN_DOWN_HALF = TIME_THROWN_DOWN / 2.0f;
	//下に投げるベクトルの大きさ
	constexpr float ADD_VECTOR_SCALE_HORIZONTAL_THORWN_DOWN = HEIGHT_HELD_UP * 2.0f * 1.2f;
	//弾かれて帰ってくるときのベクトルの大きさ
	constexpr float ADD_VECTOR_SCALE_HORIZONTAL_THORWN_DOWN_OVERLAP = HEIGHT_HELD_UP * 1.0f * 1.4f;
	//下に投げるときの奥行のベクトルの大きさ
	constexpr float ADD_VECTOR_SCALE_DEPTH_THROWN_DOWN = 60000.0f;
	//下に投げるときの奥行のベクトルの大きさに累乗する数
	constexpr float POW_NUM_DEPTH_SCALE_THROWN_DOWN = 2.0f;

	/*
	* キャンセル時関連の定数
	*/

	//キャンセル時の座標の高さ
	constexpr float POSITION_HEIGHT_CANCEL = 20.0f;

	/*
	* 弾かれるとき関連の定数
	*/

	//弾かれるときの時間
	constexpr float TIME_REPELLED = 1.0f;
	//弾かれるときの水平方向に加えるベクトルの大きさ
	constexpr float ADD_VECTOR_SCALE_HORIZONTAL_REPELLED = 500.0f;
	//弾かれるときの垂直方向に加えるベクトルの大きさ
	constexpr float ADD_VECTOR_SCALE_VERTICAL_REPELLED = 2000.0f;
}