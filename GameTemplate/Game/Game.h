#pragma once
#include "Title.h"
#include "Tips.h"

/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{

	/// <summary>
	/// ゲームクラス
	/// </summary>
	class CGame : public IGameObject
	{
	public:		//自動で呼ばれるメンバ関数
		bool Start()override final;							//スタート関数
		~CGame();											//デストラクタ

		//デバック用
		//後で消す
#ifdef MY_DEBUG
	public:
		//デバック用
		//後で消す
		void Update()override final;						//アップデート関数


	private:	//データメンバ
		std::vector<ILevelObjectBase*> m_lb;	//ILevelObjectBaseの情報を受け取る。
#endif

	};
}
