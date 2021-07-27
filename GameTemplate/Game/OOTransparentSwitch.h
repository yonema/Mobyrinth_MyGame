#pragma once
#include "ObstacleObject.h"
#include "FontRender.h"
#include "SpriteRender.h"
#include "GameCamera.h"
#include "UFO.h"
#include "SoundCue.h"
#include "effect/Effect.h"
#include "Pause.h"

/**
 * @brief 透明アイテム切り替えスイッチの定数データ
*/
namespace transparentSwitchConstData
{
	/**
	 * @brief タイマーのフォントの通常カラーを取得する
	 * @return タイマーのフォントの通常カラー
	*/
	static inline const Vector4& GetTimerFontNormalColor()
	{
		//通常カラー
		//信号の青の色から取ってきた。
		static Vector3 normalColor = { 71.0f,225.0f,241.0f };
		//明るすぎるため、正規化する
		normalColor.Normalize();
		//今度は暗すぎるため明るくする
		normalColor.Scale(1.5f);

		return { normalColor.x,normalColor.y,normalColor.z,1.0f };
	}

	//タイマーのフォントの点滅カラー
	const Vector4 FONT_COLOR_BLINK_TIMER = { 1.0f,0.0f,0.0f,1.0f };

	//実体化できるデフォルトのタイム
	constexpr float TIME_ENTITY_DEFAULT = 10.0f;

	//ウェイポイントからのZ方向への奥行
	constexpr float LENGTH_Z_FROM_WAYPOINT = 100.0f;

	/*
	* サウンド関連の定数データ
	*/

	//スイッチを押したときのサウンドのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_PRESS_SWITCH = L"Assets/sound/buttonpush.wav";
	//スイッチを押したときのサウンドの音量
	constexpr float SOUND_VOLUME_PRESS_SWITCH = 0.5f;

	//透明アイテムが実体化した時のサウンドのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_ENTITY = L"Assets/sound/activation.wav";
	//透明アイテムが実体化した時のサウンドの音量
	constexpr float SOUND_VOLUME_ENTITY = 0.5f;

	//実体化していた透明アイテムが透明になる時のサウンドのファイルパス
	constexpr const wchar_t* const SOUND_FILEPATH_TRANSPARENT = L"Assets/sound/invalidation.wav";
	//実体化していた透明アイテムが透明になる時のサウンドの音量
	constexpr float SOUND_VOLUME_TRANSPARENT = 0.5f;

	//タイマーのサウンドのファイルパス
	constexpr const wchar_t* const SOUND_FILTPATH_TIMER = L"Assets/sound/timer.wav";
	//タイマーのサウンドの音量
	constexpr float SOUND_VOLUME_TIMER = 1.0f;

	//警告タイマーのサウンドのファイルパス
	constexpr const wchar_t* const SOUND_FILTPATH_TIMER_WARNING = L"Assets/sound/timer_warning.wav";
	//警告タイマーのサウンドの音量
	constexpr float SOUND_VOLUME_TIMER_WARNING = 1.0f;

	/*
	* 透明、実体化を切り替え中の定数データ
	*/

	constexpr float TIME_WAIT_START = 0.5f;								//最初の少し待つ時間
	constexpr float TIME_FADE_OUT_START = TIME_WAIT_START + 0.5f;		//最初のフェードアウト時間
	constexpr float TIME_FADE_WAIT_START = TIME_FADE_OUT_START + 0.5f;	//最初のフェード中間時間
	constexpr float TIME_FADE_IN_START = TIME_FADE_WAIT_START + 0.5f;	//最初のフェードイン時間
	constexpr float TIME_SWITCHING = TIME_FADE_IN_START + 2.0f;			//切り替え中時間
	constexpr float TIME_SWITCHING_HALF = 
		TIME_FADE_IN_START + (TIME_SWITCHING - TIME_FADE_IN_START) / 2.0f;//切り替え中時間の半分の時間
	constexpr float TIME_FADE_OUT_END = TIME_SWITCHING + 0.5f;			//終わりのフェードアウト時間
	constexpr float TIME_FADE_WAIT_END = TIME_FADE_OUT_END + 0.5f;		//終わりのフェード中間時間
	constexpr float TIME_FADE_IN_END = TIME_FADE_WAIT_END + 0.5f;			//終わりのフェードイン時間

	//ステージ全体を見渡すカメラの座標
	const Vector3 POSITION_VIEW_STAGE_CAMERA = { 0.0f,0.0f,3500.0f };


	//タイムのフォントが点滅し始めるタイム
	constexpr float TIME_START_BLINK = 3.1f;
	//タイムのフォントが点滅する間隔
	constexpr float TIME_BLINK = 0.1f;

	//フォントの座標の高さ
	constexpr float LENGHT_UP_POSITION_FONT_TIMER = 300.0f;
	//タイマーのフォントのオフセット。（フォントはピボットが効いてないため）
	const Vector2 OFFSET_FONT_TIMER = { -40.0f,40.0f };

	//タイマーの調整値
	//そのままタイマーを入れると0.9秒～0.0秒の間ずっと0が表示されてしまうので
	//1.0f加算して、0もちょっとは表示したいから0.1f減算する
	constexpr float TIME_ADJUST = 1.0f - 0.1f;
	//タイマーのフォントのテキストのサイズ
	constexpr UINT TEXT_SIZE_FONT_TIMER = 32;
	//タイマーのフォントのテキスト
	constexpr const wchar_t* const TEXT_FONT_TIMER = L"%d";
}

/**
 * @brief 障害オブジェクト
 * 透明アイテム切り替えオブジェクト
*/
class OOTransparentSwitch : public CObstacleObject
{
public:		//自動で呼ばれるメンバ関数
	bool StartSub();			//スタート関数
	void UpdateSub();			//アップデート関数
	OOTransparentSwitch();		//コンストラクタ
	~OOTransparentSwitch();		//デストラクタ

public:		//メンバ関数

	/// <summary>
	/// リセットタイマーが作動したときの値を変更
	/// </summary>
	/// <param name="i">リセットタイマーが作動したときの値</param>
	void SetEntityMaxTime(const float entityMaxTime)
	{
		m_entityMaxTime = entityMaxTime;
	}

	/// <summary>
	/// オブジェクトを半透明にする
	/// </summary>
	void ChangeTransparent();

	/// <summary>
	/// オブジェクトを実体にする
	/// </summary>
	void ChangeEntity();

	/// <summary>
	/// スイッチのタイマーがカウントするか？を設定する
	/// </summary>
	/// <param name="timerCountFlag">タイマーがカウントするか？</param>
	void SetTimerCountFlag(const bool timerCountFlag)
	{
		m_timerCountFlag = timerCountFlag;
	}


private:	//privateなメンバ関数

	/**
	 * @brief モデルの初期化処理
	*/
	void InitModel();

	/**
	 * @brief サウンドの初期化処理
	*/
	void InitSound();

	/**
	 * @brief 必要なオブジェクトを探す処理
	*/
	void FindObjects();

	/// <summary>
	/// タイマーのフォントの更新
	/// </summary>
	void UpdateTimerFR();

	/**
	 * @brief タイマーのフォントのパラメーターを設定する
	 * @param [in] levelObject タイマーを設定するオブジェクトの参照
	*/
	void SetTimerFRParam
	(ILevelObjectBase* levelObject);

	/**
	 * @brief 透明、実体化を切り替え演出処理
	*/
	void SwitchingProduction();

	/**
	 * @brief フェードアウトする処理
	 * @param [in] fadeRate フェードアウト率。0.0f:透明。1.0f:真っ暗。
	*/
	void FadeOut(const float fadeRate);

	/**
	 * @brief フェードインする処理
	 * @param [in] fadeRate フェードイン率。0.0f:真っ暗。1.0f:透明。
	*/
	void FadeIn(const float fadeRate);

	/**
	 * @brief 透明、実体化の切り替え時の処理
	*/
	void Switching();

	/**
	 * @brief 透明、実体化の切り替え処理の最後の処理
	*/
	void SwitchingEnd();

	/**
	 * @brief カメラをステージ全体を見渡す位置に移動させる処理
	*/
	void CameraSetStageView();

	/**
	 * @brief カメラがプレイヤーを見るようにする
	*/
	void CameraSetLookPlayer();

	/**
	 * @brief タイマーのフォントの点滅処理
	*/
	void TimerFontBlink();

private: //メンバ変数

	CModelRender* m_pressedSwitchMR = nullptr;	//ボタンが押されたとき用のモデルレンダラー
	Vector4 m_fontColor = g_VEC4_WHITE;			//フォントのカラー

	/*
	* タイマー関連
	*/

	float m_entityTimer = 0.0f;					//実体化中のタイマー
	float m_entityMaxTime = 0.0f;				//実体化中の最大タイム
	float m_blinkTimer = 0.0f;					//点滅タイマー。最大値で初期化する
	float m_switchingTimer = 0.0f;				//スイッチの切り替え中のタイマー

	/*
	* フラグ関連
	*/

	bool m_flagSwitchOn = false;				//スイッチがON状態か？
	bool m_isTransparentFlag = false;			//透明状態か？
	bool m_switchingFlag = false;				//スイッチの切り替え中か？
	bool m_timerCountFlag = true;				//スイッチのタイマーがカウントするか？

	/*
	* 別の場所で生成されたオブジェクトを探す
	*/

	CGameCamera* m_gameCamera = nullptr;		//カメラのポインタ
	CUFO* m_ufo = nullptr;						//UFOのポインタ
	CPause* m_pause = nullptr;					//ポーズクラスのポインタ

	/*
	* サウンド関連
	*/

	CSoundCue* m_pressSwitchSC = nullptr;		//スイッチを押したときのサウンド
	CSoundCue* m_entitySC = nullptr;			//透明アイテムが実体化した時のサウンド
	CSoundCue* m_transparentSC = nullptr;		//実体化していた透明アイテムが透明になるときのサウンド
	CSoundCue* m_timerSC = nullptr;				//タイマーのサウンド
	CSoundCue* m_timerWarningSC = nullptr;		//警告タイマーのサウンド
};

