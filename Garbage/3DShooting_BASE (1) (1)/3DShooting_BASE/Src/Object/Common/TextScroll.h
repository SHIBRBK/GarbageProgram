#pragma once
#include<vector>
#include<string>
#include<map>
#include<DxLib.h>
#include"../../Manager/Resource.h"

class TextScroll
{
public:
    // ビルボードの表示倍率
    static constexpr float SIZE_3D = 20.0f;



    // 描画開始位置
    static constexpr float START_Y = -150.0f;
    static constexpr float START_Z = -200.0f;



    // 移動スピード
    static constexpr float SPEED_MOVE = 0.5f;



    // 文字の透明度(フェード用)
    static constexpr int ALPHA_S = 255;
    static constexpr int ALPHA_E = 0;
    static constexpr float ALPHA_DIS_S = 700.0f;
    static constexpr float ALPHA_DIS_E = 800.0f;



    // テキストの種別
    enum class TYPE
    {
        TITLE
    };

    //行ごとの文字情報
    struct MsgInfo
    {
        //行ごとの座標
        VECTOR pos = { 0, 0, 0 };
        //画像配列の添え字に変換された文字
        std::vector<int> messages;
    };






	TextScroll();
    ~TextScroll();

    void Init(void);
    void Update(void);
    void Draw(void);
    void Release(void);

private:
    //アルファベットの画像配列
    Resource mResource;

    //テキスト種別
    TYPE mType;
    
    //
    std::map<TYPE, std::vector<MsgInfo>> mTextMap;

    //移動方向
    VECTOR mVelocity;

    //テキストデータの作成
    MsgInfo MakeMsgInfo(const std::string& msg, size_t mapCount);
   

};
