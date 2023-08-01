#pragma once
#include<DxLib.h>
#include<string>
#include<vector>
#include<map>
#include"../../Manager/Resource.h"
#include"../Common/Transform.h"


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



    struct MsgInfo
    {
        VECTOR pos = { 0, 0, 0 };
        std::vector<int> messages;
    };


    TextScroll(void);
	~TextScroll(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
    //アルファベットの画像配列
    Resource mResource;

    Transform mTransform_;

    //テキスト種別
    TYPE mType;


    std::map<TYPE, std::vector<MsgInfo>> mTextMap;

    //移動速度
    VECTOR mVelocity;

    //テキストデータを作成
    MsgInfo MakeMsgInfo(const std::string& msg, size_t mapCount);
};

