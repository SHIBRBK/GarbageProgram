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



	//生存判定
	bool IsAlive(void) const;

	VECTOR GetPos(void)const;
	void SetPos(const VECTOR& pos);

	float GetZLen(void) const;
	void SetZLen(const float& zlen);
private:
	//画像のハンドルID
	int mImg;

	//座標
	VECTOR mPos;

	//大きさ
	float mSize;

	//移動方向
	VECTOR mDir;

	//移動速度
	float mSpeed;

	//生存時間
	float mLifeTime;

	//Z距離
	float mZLen;

};

