#pragma once
#include<DxLib.h>


class Particle
{
public:
	Particle(void);
	~Particle(void);

	void Generate(int img,
	VECTOR pos,float size,VECTOR dir,
		float speed, float lifetime);
	void Update();
	void Draw();
	void Release();



	//��������
	bool IsAlive(void) const;

	VECTOR GetPos(void)const;
	void SetPos(const VECTOR& pos);

	float GetZLen(void) const;
	void SetZLen(const float& zlen);
private:
	//�摜�̃n���h��ID
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

