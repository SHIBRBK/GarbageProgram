#pragma once
#include<Dxlib.h>
#include<vector>
#include "../Common/Quaternion.h"

class Particle;

class ParticleGenerator
{
public:
	// パーティクルの生成数
	static constexpr int NUM_CREATE = 30;

	static constexpr int NUM_ANGLE = 10;
	static constexpr float SPLIT_ANGLE = 360.0f / NUM_ANGLE;
	static constexpr float GEN_MIN_SIZE = 10.0f;
	static constexpr float GEN_MAX_SIZE = 20.0f;
	static constexpr float GEN_MIN_SPEED = 10.0f;
	static constexpr float GEN_MAX_SPEED = 20.0f;
	static constexpr float GEN_MIN_LIFE_TIME = 5.0f;
	static constexpr float GEN_MAX_LIFE_TIME = 10.0f;
	static constexpr float GEN_MIN_X_ROT = 30.0f;
	static constexpr float GEN_MAX_X_ROT = 90.0f;


	static constexpr float GEN_TIME = 0.1f;

	// コンストラクタ
	ParticleGenerator(VECTOR pos, float radius);
	// デストラクタ
	~ParticleGenerator(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawMeshSquare(void);
	void DrawMeshCircle(void);
	void Release(void);

	void SetPos(const VECTOR& pos);
	void SetRot(const Quaternion& rot);

private:
	// パーティクル画像
	int mImg;
	// 座標
	VECTOR mPos;
	// 向き
	Quaternion mRot;
	// 角度
	float mRadius;
	Quaternion mAngleQ;
	// 頂点数
	int mCntVertex;
	// 頂点データ(四角用)
	VERTEX3D mVerticesSquare[6];
	// 頂点データ(円形用)
	VERTEX3D mVerticesCircle[NUM_ANGLE + 1];
	// 頂点インデックス(分割したポリゴン * 一ポリゴン当たりの頂点数)
	WORD mIndexes[NUM_ANGLE * 3];
	// パーティクル
	std::vector<Particle*> mParticles;
	// 四角メッシュを作る
	void CreateMeshSquare(void);
	// 円形メッシュを作る
	void CreateMeshCircle(void);
	// パーティクル生成
	Particle* Generate(Particle* particle);
	// Zソート
	void ZSort(void);
	//徐々に生成
	float mStepGenerate;

};

