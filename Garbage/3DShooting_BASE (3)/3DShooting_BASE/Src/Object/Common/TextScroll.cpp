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
    //�A���t�@�x�b�g�摜�̓ǂݍ��݁B
    //���\�[�X�S���ǂݍ��݁B�z��g������
    mResource = ResourceManager::GetInstance().Load
    (ResourceManager::SRC::ALPHABET);

    mType = TYPE::TITLE;

    //1�s������̏��
    std::vector<MsgInfo> infos;
    //���͂̐ݒ�(1�s����)
    infos.emplace_back(MakeMsgInfo("Kimotiyosugi",infos.size()));
    infos.emplace_back(MakeMsgInfo("tin",infos.size()));

    mTextMap.emplace(TYPE::TITLE, infos);

    //�ړ�����(���K1)
    // 
    //�܂����܂����̃I�C���[�p
    Quaternion rot = Quaternion::Euler(
        AsoUtility::Deg2RadF(-40.0f), 0.0f, 0.0f
    );

    mVelocity=rot.GetForward();

    //-40.0f
    //�ړ����x(�����~���x)
    mVelocity = VScale(mVelocity,SPEED_MOVE);

    //x�́[40�̃x�N�g����������B
    //�N�H�[�^�j�I���g������
}

void TextScroll::Update(void)
{
    std::vector<MsgInfo>& infos=mTextMap[mType];
    for(auto& info : infos)
    {
        //�����̈ړ�����
        info.pos = VAdd(info.pos, mVelocity);
    }
}

void TextScroll::Draw(void)
{
    const std::vector<MsgInfo>& infos = mTextMap[mType];
    VECTOR pos;

    float plusX=0.0f;
    for (auto& info : infos)//�����܂ōs�̏��
    {
        for (auto& id : info.messages) //1�������̏��
        {
            pos = info.pos;
            pos.x += plusX;
            //���K2 ���ɍ��W�����炷
            DrawBillboard3D(pos, pos.x, pos.y, SIZE_3D, 0.0f,
                mResource.mHandleIds[id],true);//id�n��

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

    //�摜�z��摜�z��̓Y�����ɕϊ�
    std::vector<int>messages;

    int ascii;
    int idx;
    size_t len = msg.size();
    for(int i=0;i<len;i++)
    {
        //���K3�ϊ�����������������
        idx = 0;

        //�X�y�[�X
        ascii = msg.at(i);
        if (ascii == 32) {
            idx = 52;
           
        }

        //�h�b�g
        if (ascii == 46) {
            idx = 53;
        }

        //�J���}
        if (ascii == 44) {
            idx = 54;
        }

        //�啶��(�̂���)
        for (ascii = 65; ascii <= 90; ascii++) {
            for (idx = 0; idx <= 25;idx++) {
                if (ascii == 46) {
                    idx ++;
                }
            }
        }


        //������(�̂���)
        for (ascii = 97; ascii <= 122; ascii++) {
            for (idx = 26; idx <= 51; idx++) {
                if (ascii == 46) {
                    idx++;
                }
            }
        }


        //32��������X�y�[�X(�摜�Y����52)
        messages.emplace_back(idx);
    }


    //4 �s���Ƃ̍��W
    ret.pos = {
        -400.0f,
        START_Y - (mapCount * SIZE_3D),
        START_Z - (mapCount * SIZE_3D)
    };


    //�ϊ����ꂽID��������
    ret.messages = messages;
    return ret;
}
