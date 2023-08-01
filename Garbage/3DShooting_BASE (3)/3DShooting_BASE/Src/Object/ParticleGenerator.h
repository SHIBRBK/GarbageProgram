#pragma once
#include<vector>
#include<DxLib.h>
#include"../Common/Quaternion.h"


class Particle;

class ParticleGenerator
{
public:

	//移動方向
	static constexpr float GEN_MIN_X_ROT = -40.0f;
	static constexpr float GEN_MAX_X_ROT = -40.0f;

	//大きさ
	static constexpr float GEN_MIN_SIZE = 3.0f;
	static constexpr float GEN_MAX_SIZE = 5.0f;

	//移動速度
	static constexpr float GEN_MIN_SPEED = 1.0f;
	static constexpr float GEN_MAX_SPEED = 2.0f;


	//生存時間
	static constexpr float GEN_MIN_LIFE_TIME = 1.0f;
	static constexpr float GEN_MAX_LIFE_TIME = 2.0f;

	//徐々に生成
	static constexpr float GEN_TIME = 0.05f;


	//パーティクルの生成数
	static constexpr int NUM_CREATE = 30;

	//自作円形メッシュ
	static constexpr int NUM_ANGLE = 10;
	static constexpr float SPLIT_ANGLE = 360.0f/ NUM_ANGLE;


	ParticleGenerator(VECTOR pos,float radius);
	~ParticleGenerator(void);

	void Init();
	void Update();
	void Draw();
	void DrawMeshSquare();
	void DrawMeshCircle();
	void Release();

	void SetPos(const VECTOR& pos);


	void SetRot(const Quaternion& rot);

private:

	//パーティクル画像
	int mImage;



	//座標
	VECTOR mPos;

	//向き(角度)
	Quaternion mRot;
	


	float mRadius;

	//頂点数
	int mCntVertex;



	//頂点データ(資格用)
	VERTEX3D mVerticesSquare[6];
	VERTEX3D mVerticesCircle[NUM_ANGLE+1];

	//頂点インデックス
	WORD mIndexs[NUM_ANGLE * 3];


	//徐々に生成するための経過時間
	float mStepGenerate;

	//パーティクル
	std::vector<Particle*> mParticles_;

	//
	void CreatemeshSquare();

	//円形メッシュ
	void CreatemeshCircle();

	//パーティクル生成
	Particle* Generate(Particle* particle);


	//Zソート　
	void ZSort(void);

};



