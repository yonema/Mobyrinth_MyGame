#pragma once
#include "GameCameraConstData.h"
#include "SpriteRender.h"
#include "Player.h"
#include "StartDirecting.h"
#include "SpringCamera.h"

/**
 * @brief ゲーム内カメラクラス
*/
class CGameCamera : public IGameObject
{
public:		//自動で呼ばれるメンバ関数
	bool Start()override final;		//スタート関数
	void Update()override final;	//アップデート関数

private:	//privateメンバ関数

	/**
	 * @brief ゲーム中のカメラ
	*/
	void InGameCamera();


public:		//メンバ関数

	/// <summary>
	/// カメラの注視点を設定する
	/// </summary>
	/// <param name="target"></param>
	void SetTarget(const Vector3& target)
	{
		m_springCamera.SetTarget(target);
	}

	/// <summary>
	/// カメラの視点を設定する
	/// </summary>
	/// <param name="pos"></param>
	void SetPosition(const Vector3& pos)
	{
		m_springCamera.SetPosition(pos);
	}

	/// <summary>
	/// カメラの視点を取得する
	/// </summary>
	/// <returns>カメラの視点</returns>
	const Vector3& GetPosition() const
	{
		return m_springCamera.GetPosition();
	}

	/// <summary>
	/// タイトルモードにする
	/// </summary>
	void TitleMode()
	{
		m_titleMode = true;
	}

	/// <summary>
	/// プレイヤーを見るか？を設定する
	/// </summary>
	/// <param name="lookPlayerFlag">プレイヤーを見るか？</param>
	void SetLookPlayerFlag(const bool lookPlayerFlag)
	{
		m_lookPlayerFlag = lookPlayerFlag;
	}

	/// <summary>
	/// リフレッシュ
	/// この関数を呼び出すと、バネの移動がリフレッシュされて、Update関数を呼び出すとカメラの位置が
	/// 即座に目標視点と目標注視点に移動します。シーンが切り替わってカメラを一気に切り替えたいときなどに
	/// 使用してください。
	/// </summary>
	void Refresh()
	{
		m_springCamera.Refresh();
	}

	/// <summary>
	/// ワイプが終わったかを設定
	/// </summary>
	/// <param name="wipeEndFlag">ワイプが終わった？</param>
	void SetWipeEndFlag(const bool wipeEndFlag)
	{
		m_startDirecting.SetWipeEndFlag(wipeEndFlag);
	}


private:	//データメンバ

	bool m_lookPlayerFlag = true;				//プレイヤーを見るか？
	bool m_titleMode = false;					//タイトル画面か？

	CSpringCamera m_springCamera;				//ばねカメラ
	CStartDirecting m_startDirecting;			//開始演出クラス
	Player* m_player = nullptr;					//プレイヤーのポインタ

};

