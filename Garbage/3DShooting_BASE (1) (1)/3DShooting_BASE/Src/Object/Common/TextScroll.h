#pragma once
#include<vector>
#include<string>
#include<map>
#include<DxLib.h>
#include"../../Manager/Resource.h"

class TextScroll
{
public:
    // �r���{�[�h�̕\���{��
    static constexpr float SIZE_3D = 20.0f;



    // �`��J�n�ʒu
    static constexpr float START_Y = -150.0f;
    static constexpr float START_Z = -200.0f;



    // �ړ��X�s�[�h
    static constexpr float SPEED_MOVE = 0.5f;



    // �����̓����x(�t�F�[�h�p)
    static constexpr int ALPHA_S = 255;
    static constexpr int ALPHA_E = 0;
    static constexpr float ALPHA_DIS_S = 700.0f;
    static constexpr float ALPHA_DIS_E = 800.0f;



    // �e�L�X�g�̎��
    enum class TYPE
    {
        TITLE
    };

    //�s���Ƃ̕������
    struct MsgInfo
    {
        //�s���Ƃ̍��W
        VECTOR pos = { 0, 0, 0 };
        //�摜�z��̓Y�����ɕϊ����ꂽ����
        std::vector<int> messages;
    };






	TextScroll();
    ~TextScroll();

    void Init(void);
    void Update(void);
    void Draw(void);
    void Release(void);

private:
    //�A���t�@�x�b�g�̉摜�z��
    Resource mResource;

    //�e�L�X�g���
    TYPE mType;
    
    //
    std::map<TYPE, std::vector<MsgInfo>> mTextMap;

    //�ړ�����
    VECTOR mVelocity;

    //�e�L�X�g�f�[�^�̍쐬
    MsgInfo MakeMsgInfo(const std::string& msg, size_t mapCount);
   

};
