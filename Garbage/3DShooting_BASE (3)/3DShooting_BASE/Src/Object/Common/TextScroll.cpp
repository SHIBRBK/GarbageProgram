#include "TextScroll.h"
#include"../../Manager/ResourceManager.h"
#include"../../Common/Quaternion.h"
#include"../../Utility/AsoUtility.h"

TextScroll::TextScroll(void)
{

}

TextScroll::~TextScroll(void)
{

}

void TextScroll::Init(void)
{
    //アルファベット画像の読み込み。
    //リソース全部読み込み。配列使うから
    mResource = ResourceManager::GetInstance().Load
    (ResourceManager::SRC::ALPHABET);

    mType = TYPE::TITLE;

    //1行当たりの情報
    std::vector<MsgInfo> infos;
    //文章の設定(1行ずつ)
    infos.emplace_back(MakeMsgInfo("Kimotiyosugi",infos.size()));
    infos.emplace_back(MakeMsgInfo("tin",infos.size()));

    mTextMap.emplace(TYPE::TITLE, infos);

    //移動方向(演習1)
    // 
    //まさかまさかのオイラー角
    Quaternion rot = Quaternion::Euler(
        AsoUtility::Deg2RadF(-40.0f), 0.0f, 0.0f
    );

    mVelocity=rot.GetForward();

    //-40.0f
    //移動速度(方向×速度)
    mVelocity = VScale(mVelocity,SPEED_MOVE);

    //xのー40のベクトルを作った。
    //クォータニオン使うかも
}

void TextScroll::Update(void)
{
    std::vector<MsgInfo>& infos=mTextMap[mType];
    for(auto& info : infos)
    {
        //文字の移動処理
        info.pos = VAdd(info.pos, mVelocity);
    }
}

void TextScroll::Draw(void)
{
    const std::vector<MsgInfo>& infos = mTextMap[mType];
    VECTOR pos;

    float plusX=0.0f;
    for (auto& info : infos)//あくまで行の情報
    {
        for (auto& id : info.messages) //1文字ずつの情報
        {
            pos = info.pos;
            pos.x += plusX;
            //演習2 横に座標をずらす
            DrawBillboard3D(pos, pos.x, pos.y, SIZE_3D, 0.0f,
                mResource.mHandleIds[id],true);//id渡し

            plusX += SIZE_3D;
        }
    }
}

void TextScroll::Release(void)
{

}

TextScroll::MsgInfo TextScroll::MakeMsgInfo(const std::string& msg, size_t mapCount)
{
    MsgInfo ret;

    //画像配列画像配列の添え字に変換
    std::vector<int>messages;

    int ascii;
    int idx;
    size_t len = msg.size();
    for(int i=0;i<len;i++)
    {
        //演習3変換処理を完成させよ
        idx = 0;

        //スペース
        ascii = msg.at(i);
        if (ascii == 32) {
            idx = 52;
           
        }

        //ドット
        if (ascii == 46) {
            idx = 53;
        }

        //カンマ
        if (ascii == 44) {
            idx = 54;
        }

        //大文字(のつもり)
        for (ascii = 65; ascii <= 90; ascii++) {
            for (idx = 0; idx <= 25;idx++) {
                if (ascii == 46) {
                    idx ++;
                }
            }
        }


        //小文字(のつもり)
        for (ascii = 97; ascii <= 122; ascii++) {
            for (idx = 26; idx <= 51; idx++) {
                if (ascii == 46) {
                    idx++;
                }
            }
        }


        //32だったらスペース(画像添え字52)
        messages.emplace_back(idx);
    }


    //4 行ごとの座標
    ret.pos = {
        -400.0f,
        START_Y - (mapCount * SIZE_3D),
        START_Z - (mapCount * SIZE_3D)
    };


    //変換されたIDを代入する
    ret.messages = messages;
    return ret;
}
