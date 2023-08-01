#pragma once
#include <DxLib.h>

class Particle
{
public:
    Particle(void);
    ~Particle(void);

    void Generate(int image, VECTOR pos, float size, VECTOR dir, float speed, float lifetime);
    void Update(void);
    void Draw(void);
    void Release(void);

    //��������
    bool IsAlive(void) const;

    VECTOR GetPos(void) const;
    void SetPos(const VECTOR& pos);

    float GetZLen(void)const;
    void SetZLen(const float& zlen);
private:

    //�摜
    int mImg;

    //���W
    VECTOR mPos;

    //�傫��
    float mSize;

    //�ړ�����
    VECTOR mDir;

    //�ړ����x
    float mSpeed;

    //��������
    float mLifeTime;

    //Z����
    float mZLen;
};