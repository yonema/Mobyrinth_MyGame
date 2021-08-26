#pragma once
extern const UINT g_FRAME_BUFFER_W;	//フレームバッファの幅
extern const UINT g_FRAME_BUFFER_H;	//フレームバッファの高さ
/**
 * @brief メビリンス
*/
namespace nsMobyrinth
{
	/**
	 * @brief グラフィック
	*/
	namespace nsGraphic
	{
		/**
		 * @brief HUD
		*/
		namespace nsHUD
		{

			/**
			 * @brief HUDクラスの定数データ
			*/
			namespace HUDConstData
			{


				/**
				 * @brief ワイプの種類、ここを変更したらWipe.fxも変更すること。
				*/
				enum EN_WIPE_TYPE
				{
					EN_WIPE,					//普通のワイプ
					EN_CIRCLE_WIPE,				//円形ワイプ
					EN_VERTICAL_STRIPE_WIPE,	//縦縞ワイプ
					EN_HORIZONTAL_STRIPE_WIPE,	//横縞ワイプ
					EN_CHECKERBOARD_WIPE,		//チェッカーボードワイプ	//ここまでしか実装していない
					EN_WIPE_NUM,				//ワイプの種類の数

					EN_MONOCHROME,				//モノクロ加工
					EN_SEPIA,					//セピア調加工
					EN_NEGA,					//ネガポジ反転
					EN_NOIS,					//ノイズ加工
				};

				//デフォルトのワイプの方向
				const Vector2 DIR_WIPE_DEFAULT = { -1.0f,1.0f };


				//普通のワイプのX軸の最大サイズ、ここを変更したらWipe.fxも変更すること。
				const float SIZE_MAX_X_WIPE = static_cast<float>(g_FRAME_BUFFER_W);
				//普通のワイプのY軸の最大サイズ、ここを変更したらWipe.fxも変更すること。
				const float SIZE_MAX_Y_WIPE = static_cast<float>(g_FRAME_BUFFER_H);
				//円形ワイプの最大サイズ、ここを変更したらWipe.fxも変更すること。
				const float SIZE_MAX_WIPE_CIRCLE = static_cast<float>(g_FRAME_BUFFER_W) / 1.5f;
				//縦縞ワイプの最大サイズ、ここを変更したらWipe.fxも変更すること。
				constexpr float SIZE_MAX_WIPE_STRIPE_VERTICAL = 64.0f;
				//横縞ワイプの最大サイズ、ここを変更したらWipe.fxも変更すること。
				constexpr float SIZE_MAX_WIPE_STRIPE_HORIZONTAL = 64.0f;
				//チェッカーボードワイプの最大サイズ、ここを変更したらWipe.fxも変更すること。
				constexpr float SIZE_MAX_WIPE_CHECKERBOARD = 128.0f;

				//デフォルトのワイプのスピード
				constexpr float SPEED_WIPE_DEFAULT = 120.0f;
				//普通のワイプのスピード
				constexpr float SPEED_WIPE = 1200.0f;
				//円形ワイプのスピード
				constexpr float SPEED_WIPE_CIRLE = 600.0f;

				//ワイプ用のシェーダーのファイルパス
				constexpr const char* const SHADER_FILEPATH_WIPE = "Assets/shader/Wipe.fx";
				//ワイプの後ろに映すスプライトのファイルパス
				constexpr const char* const SPRITE_FILEPATH_BACK_SPRITE = "Assets/Image/backSprite2.DDS";
				//フェードイン、アウトで使うスプライトのファイルパス
				constexpr const char* const SPRITE_FILEPATH_FADE = "Assets/Image/black.DDS";


				/// <summary>
				/// ワイプのパラメータ構造体、ここを変更したらWipe.fxも変更すること。
				/// </summary>
				struct SWipeParam
				{
					Vector2 wipeDir = DIR_WIPE_DEFAULT;	//ワイプの方向
					float wipeSize = 0.0f;				//ワイプした量
					int leftOtLight = nsCommonData::EN_LEFT;			//右と左どちらからワイプするか？
					int topOrDown = nsCommonData::EN_TOP;				//上と下どちらからワイプするか？
					int wipeType = EN_WIPE;				//ワイプの種類
					int inOrOut = nsCommonData::EN_OUT;				//インとアウトどちらにワイプするか？
				};
			}

		}
	}
}