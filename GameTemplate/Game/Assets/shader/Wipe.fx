/*!
 * @brief リニアワイプ
 */



 ///////////////////////////////////////////////////
 // 定数
 ///////////////////////////////////////////////////


//画面の大きさ
static const int g_width = 1280;    //画面の横幅
static const int g_height = 720;    //画面の縦幅

//ワイプがどっち側からワイプされるか
static const int g_left = 0;        //左側から
static const int g_right = 1;       //右側から
static const int g_top = 0;         //上側から
static const int g_down = 1;        //下側から

//どの種類のワイプを行うか
static const int g_wipe = 0;                    //普通のワイプ
static const int g_circleWipe = 1;              //円形ワイプ
static const int g_verticalStripeWipe = 2;      //縦縞ワイプ
static const int g_horizontalStripeWipe = 3;    //横縞ワイプ
static const int g_checkerboardWipe = 4;        //チェッカーボードワイプ
static const int g_monochrome = 5;              //モノクロ加工
static const int g_sepia = 6;                   //セピア調加工
static const int g_nega = 7;                    //ネガポジ反転
static const int g_noise = 8;                   //ノイズ加工

//各ワイプの最大ワイプサイズ
//ここを変更したら、SceneChange.hの定数も変更すること
static const float g_wipeMax = g_width;                 //普通のワイプのX軸の最大サイズ
static const float g_circleWipeMax = g_width / 1.5;     //普通のワイプのY軸の最大サイズ
static const float g_verticalStripeWipeMax = 64.0f;     //円形ワイプの最大サイズ
static const float g_horizontalStripeWipeMax = 64.0f;   //縦縞ワイプの最大サイズ
static const float g_checkerboardWipeMax = 128.0f;      //横縞ワイプの最大サイズ
                                                        //チェッカーボードワイプの最大サイズ
//ワイプイン、ワイプアウト
static const int g_in = 0;          //ワイプイン
static const int g_out = 1;         //ワイプアウト


///////////////////////////////////////////////////
// 構造体
///////////////////////////////////////////////////
struct VSInput
{
    float4 pos : POSITION;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float2 uv  : TEXCOORD0;
};


////////////////////////////////////////////////
// 定数バッファ。
////////////////////////////////////////////////

//　スプライトの基本の定数バッファ
cbuffer cb : register(b0)
{
    float4x4 mvp;       // MVP行列
    float4 mulColor;    // 乗算カラー
};


// ワイプパラメータにアクセスするための定数バッファ
//ここを変更したら、SceneChange.hの構造体も変更すること
cbuffer WipeCB : register (b1)
{
    float2 wipeDirection;   //ワイプする方向
    float wipeSize;         //ワイプのサイズ
    int leftOrLight;        //左側か右側か
    int topOrDown;          //上側か下側か
    int wipeType;           //ワイプの種類
    int inOrOut;            //ワイプインかワイプアウトか
}


////////////////////////////////////////////////
// グローバル変数。
////////////////////////////////////////////////
Texture2D<float4> colorTexture : register(t0);  // カラーテクスチャ
sampler Sampler : register(s0);                 //サンプラー


///////////////////////////////////////////
//関数宣言
///////////////////////////////////////////

//ワイプインの時のワイプサイズを計算
float WipeInMode(float wipeOutSize, float maxWipeSize);

//普通のワイプ
float4 Wipe(float2 Inpos, float4 color);

//円形ワイプ
float4 CircleWipe(float2 Inpos, float4 color);

//縦縞ワイプ
float4 VerticalStripeWipe(float2 Inpos, float4 color);

//横縞ワイプ
float4 HorizontalStripeWipe(float2 Inpos, float4 color);

//チェッカーボードワイプ
float4 CheckerboardWipe(float2 Inpos, float4 color);


////////////////////////////////////////////////
// 関数定義。
////////////////////////////////////////////////


//頂点シェーダ―
PSInput VSMain(VSInput In)
{
    PSInput psIn;
    psIn.pos = mul(mvp, In.pos);
    psIn.uv = In.uv;
    return psIn;
}

//ピクセルシェーダ―
float4 PSMain(PSInput In) : SV_Target0
{
    //サンプリングする
    float4 color = colorTexture.Sample(Sampler, In.uv);

    //各種ワイプごとに割り振る
    switch (wipeType)
    {
    case g_wipe:
        //普通のワイプ
        color = Wipe(In.pos.xy, color);
        break;

    case g_circleWipe:
        //円形ワイプ
        color = CircleWipe(In.pos.xy, color);
        break;

    case g_verticalStripeWipe:
        //縦縞ワイプ
        color = VerticalStripeWipe(In.pos.xy, color);
        break;

    case g_horizontalStripeWipe:
        //横縞ワイプ
        color = HorizontalStripeWipe(In.pos.xy, color);
        break;

    case g_checkerboardWipe:
        //チェッカーボードワイプ
        color = CheckerboardWipe(In.pos.xy, color);
        break;

    case g_monochrome:
        //モノクロ加工
        break;

    case g_sepia:
        //セピア調加工
        break;

    case g_nega:
        //ネガポジ反転
        break;

    case g_noise:
        //ノイズ加工
        break;
    }
   
    
    
    return color;
}

//普通のワイプ
float4 Wipe(float2 Inpos, float4 color)
{
    //最終的なUV
    float2 finalUV = Inpos;
    //最終的なワイプの大きさ
    float finalWipeSize = wipeSize;
    //最終的な上側か下側
    int finalTopOrDown = topOrDown;

    //左側からワイプさせたいときは
    if (leftOrLight == g_left)
    {
        //UVのX軸を反転させる
        finalUV.x = Inpos.x - g_width;
    }
    //右側からワイプさせたいときは、そのまま。


    //ワイプインの場合はワイプサイズを反転させる
    if (inOrOut == g_in)
    {
        //ワイプサイズを最大サイズを使って反転
        finalWipeSize = WipeInMode(wipeSize, g_wipeMax);
        //上側と下側も反転させる
        finalTopOrDown = !finalTopOrDown;
    }

    //下側からワイプさせたいときは、
    if (finalTopOrDown == g_down)
    {
        //UVのY軸を反転させる
        finalUV.y = Inpos.y - g_height;
    }

    //ワイプする単位方向ベクトルに、UV（原点から現在のピクセルへのベクトル）
    //を射影する。
    float t = dot(wipeDirection, finalUV);

    


    //射影されたベクトルがワイプサイズ以下なら表示しない。
    //clip(t - finalWipeSize);
    //clipだと真っ暗になって、後ろのスプライトが見えないから
    //アルファ値0.0fのカラーにする
    if ((t - finalWipeSize) < 0)
        color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return color;
}

//円形ワイプ
float4 CircleWipe(float2 Inpos, float4 color)
{
    //最終的なワイプの大きさ
    float finalWipeSize = wipeSize;

    // 画面の中央から現在のピクセルに向かって伸びるベクトルを計算する。
    float2 posFromCenter = Inpos.xy - float2(g_width / 2, g_height / 2);

    //ワイプインの場合はワイプサイズを反転させる
    if (inOrOut == g_in)
        finalWipeSize = WipeInMode(wipeSize, g_circleWipeMax);

    // 画面の中央からの距離がワイプサイズ以下なら表示しない。
    //clip(length(posFromCenter) - finalWipeSize);

    if ((length(posFromCenter) - finalWipeSize) < 0)
        color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return color;
}

//縦縞ワイプ
float4 VerticalStripeWipe(float2 Inpos, float4 color)
{
    //最終的なワイプの大きさ
    float finalWipeSize = wipeSize;

    //区切り
    const float separation = g_verticalStripeWipeMax;
    // ピクセルのX座標を64で割った余りがワイプサイズ以下なら表示しない。
    float t = (int)fmod(Inpos.x, separation);

    //ワイプインの場合はワイプサイズを反転させる
    if (inOrOut == g_in)
        finalWipeSize = WipeInMode(wipeSize, g_verticalStripeWipeMax);

    //clip(t - finalWipeSize);
    if ((t - finalWipeSize) < 0)
        color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return color;
}

//横縞ワイプ
float4 HorizontalStripeWipe(float2 Inpos, float4 color)
{
    //最終的なワイプの大きさ
    float finalWipeSize = wipeSize;

    //区切り
    const float separation = g_horizontalStripeWipeMax;
    //  ピクセルのY座標を64で割った余りがワイプサイズ以下なら表示しない。
    float t = (int)fmod(Inpos.y, separation);

    //ワイプインの場合はワイプサイズを反転させる
    if (inOrOut == g_in)
        finalWipeSize = WipeInMode(wipeSize, g_horizontalStripeWipeMax);

    //clip(t - finalWipeSize);
    if ((t - finalWipeSize) < 0)
        color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return color;
}

//チェッカーボードワイプ
float4 CheckerboardWipe(float2 Inpos, float4 color)
{
    //最終的なワイプの大きさ
    float finalWipeSize = wipeSize;

    //区切り
    float separation = g_checkerboardWipeMax;
    //横にずらす値
    float misAlignment = 64.0f;

    //ピクセルのY座標を区切る
    float t = floor(Inpos.y / separation);
    //奇数段目には1、偶数段目には0が入る
    t = fmod(t, 2.0f);
    //奇数段目なら、横にずらしてワイプする
    t = (int)fmod(Inpos.x + misAlignment * t, separation);

    //ワイプインの場合はワイプサイズを反転させる
    if (inOrOut == g_in)
        finalWipeSize = WipeInMode(wipeSize, g_checkerboardWipeMax);

    //clip(t - finalWipeSize);
    if ((t - finalWipeSize) < 0)
        color = float4(0.0f, 0.0f, 0.0f, 0.0f);

    return color;
}


//ワイプインの時のワイプサイズを計算
float WipeInMode(float wipeOutSize, float maxWipeSize)
{
    return maxWipeSize - wipeOutSize;
}