#include "ParticleGenerator.h"
#include"../Utility/AsoUtility.h"
#include"../Application.h"
#include"../Common/Quaternion.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/Camera.h"
#include"Particle.h"

ParticleGenerator::ParticleGenerator(VECTOR pos, float radius)
{
	mPos = pos;
	mRadius = radius;
}

ParticleGenerator::~ParticleGenerator(void)
{
}

void ParticleGenerator::Init()
{
	mImage = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::LIGHT
	).mHandleId;


	CreatemeshSquare();
	//円形メッシュ
	CreatemeshCircle();

	//一気に作る
	//for(int i=0;i<NUM_CREATE;i++)
	//{
	//	mParticles_.emplace_back(Generate(nullptr));
	//}
	
	//徐々に作る
	for(int i=0;i<NUM_CREATE;i++)
	{
		mParticles_.emplace_back(new Particle());
	}


	//角度の初期化
	mRot = Quaternion::Euler(0.0f, 0.0f, 0.0f);

	mStepGenerate = GEN_TIME;
}

void ParticleGenerator::Update()
{

	bool isGenerate = false;
	mStepGenerate -= SceneManager::GetInstance().GetDeltaTime();

	
	if (mStepGenerate < 0.0f) {
		isGenerate = true;
		mStepGenerate = GEN_TIME;
	}
	for(auto& p : mParticles_)
	{
		p->Update();

		//生成時間が切れたら再生成
		if (!p->IsAlive()) 
		{
			if (isGenerate)
			{
				p = Generate(p);
				mStepGenerate = GEN_TIME;
				isGenerate = false;
			}
		}
	}
#ifdef DEBUG
	//ParticleGenerator=mRotの回転処理

	bool isHit=flase;


	//右回転
	if (CheckHitKey(KEY_INPUT_L)) {
		mRot = mRot.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadD(1.0), { 0.0f,1.0f,0.0f }));

		mRot = mRot.Mult(axis);
		isHit = true;
		
	}


	if (CheckHitKey(KEY_INPUT_J)) {
		mRot = mRot.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadD(-1.0), { 0.0f,1.0f,0.0f }));
		isHit = true;

}

	if (CheckHitKey(KEY_INPUT_K)) {
		mRot = mRot.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadD(1.0), { 1.0f,0.0f,0.0f }));
		isHit = true;

	}
	//上回転
	if (CheckHitKey(KEY_INPUT_I)) {
		mRot = mRot.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadD(-1.0), { 1.0f,0.0f,0.0f }));
		isHit = true;
	}

	if (isHit) {
		CreatemeshCircle();
	}

#endif 
}

void ParticleGenerator::Draw()
{
	DrawMeshSquare();
	DrawMeshCircle();

	//Zソート
	ZSort();

	SetDrawBlendMode(DX_BLENDMODE_ADD,128);
	for (auto& p : mParticles_)
	{
		p->Draw();
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	Quaternion tmpQ = Quaternion::Euler(
		AsoUtility::Deg2RadF(-90.0f),
		0.0f, 
		0.0f
	);

	tmpQ = mRot.Mult(tmpQ);
	//DrawLine3D(
	//	mPos,
	//	VAdd()
	//);

}

void ParticleGenerator::DrawMeshSquare()
{
	DrawPolygon3D(mVerticesSquare, 1, DX_NONE_GRAPH, true);
}

void ParticleGenerator::DrawMeshCircle()
{
	DrawPolygonIndexed3D(mVerticesCircle, NUM_ANGLE+1,mIndexs,NUM_ANGLE,
		DX_NONE_GRAPH,true);

	for (auto& vert : mVerticesCircle) 
	{
		DrawSphere3D(vert.pos, 2.0f, 5, 0xff0000, 0xff0000, true);
	}

}

void ParticleGenerator::Release()
{
	
	for (auto& p : mParticles_)
	{
		if (p != nullptr) {
			p->Release();
			delete p;
		}
	}
	mParticles_.clear();
}

void ParticleGenerator::SetPos(const VECTOR& pos)
{
	
		mPos = pos;//位置の動機？
	
		CreatemeshCircle();
}

void ParticleGenerator::SetRot(const Quaternion& rot)
{
	mRot=rot;
}

void ParticleGenerator::CreatemeshSquare()
{
	COLOR_U8 colorW = GetColorU8(255, 255, 255, 255);
	COLOR_U8 colorR = GetColorU8(255, 0, 0, 255);

	float z = 0.0f;
	float h = 100.0f;

	//四角形を作る
	mVerticesSquare[0].pos = { z,z,z };
	mVerticesSquare[0].dif = colorW;
	mVerticesSquare[1].pos = {z,z,h};
	mVerticesSquare[1].dif = colorW;
	mVerticesSquare[2].pos = {h,z,h};
	mVerticesSquare[2].dif = colorW;

	mVerticesSquare[3].pos = { z,z,z };
	mVerticesSquare[3].dif = colorW;
	mVerticesSquare[4].pos = { h,z,h };
	mVerticesSquare[4].dif = colorW;
	mVerticesSquare[5].pos = { h,z,z };
	mVerticesSquare[5].dif = colorW;

}

void ParticleGenerator::CreatemeshCircle()
{
	COLOR_U8 colorW = GetColorU8(255, 255, 255, 255);
	COLOR_U8 colorR = GetColorU8(255, 0, 0, 255);
	COLOR_U8 colorG = GetColorU8(0, 255, 0, 255);
	COLOR_U8 colorB = GetColorU8(0, 0, 255, 255);

	mCntVertex = 0;

	//円の中心
	mVerticesCircle[0].pos = mPos;
	mVerticesCircle[0].dif = colorB;

	//円形の頂点を作成

	int cntAngle=0;
	for (int i = 1; i < NUM_ANGLE; i++) {


		//頂点座標(クォータニオン)
		Quaternion rot = Quaternion::AngleAxis(
			AsoUtility::Deg2RadF(cntAngle), AsoUtility::AXIS_Y);

		rot = mRot.Mult(rot);


		//座標を回転
		VECTOR localPos = rot.PosAxis({ 0.0f,0.0f,mRadius });

		VERTEX3D vertex = VERTEX3D();

		vertex.pos = VAdd(mPos, localPos);

		vertex.dif = colorG;

		mVerticesCircle[i + 1] = vertex;

		cntAngle += SPLIT_ANGLE;

	}

	int cntIdx = 0;

	int cntVIdx = 1;
	for(int i=1;i<NUM_ANGLE;i++)
	{
		mIndexs[cntIdx++] = 0;
		mIndexs[cntIdx++] = cntVIdx;
		mIndexs[cntIdx++] = ++cntVIdx;
	}


	mIndexs[cntIdx++] = 0;
	mIndexs[cntIdx++] = cntVIdx;
	mIndexs[cntIdx++] = 1;


	/*Quaternion axis;
	axis = Quaternion::AngleAxis(
		AsoUtility::Deg2RadF(36.0f), AsoUtility::AXIS_Y);*/


	//ここいらない子説
	//float localRadY=AsoUtility::Deg2RadF(36.0f);
	//Quaternion axis;
	//axis=Quaternion::AngleAxis(localRadY, { 1.0f,0.0f,0.0f });

	//Quaternion::PosAxis(axis, mPos);
	


	

	

}

Particle* ParticleGenerator::Generate(Particle* particle)
{

	//②mRotを使用して、位置や方向を回転してください
	//1行書けば回転します
	//円形メッシュをx回転



	//mRot.AngleAxis(AsoUtility::Deg2RadD(1.0),mPos);

	if (particle == nullptr) {
		particle = new Particle();	//実はインスタンスって重い　重なるとなおさら

	}

	//自分の前方方向のそうたい座標　半径　36度　こいつをうまく使うと円形でぐるぐる回せる。
	//ランダムなx軸を渡す

	//ランダムな位置
	//ランダムな大きさ
	//ランダムな向き
	// ランダムなスピード
	//大きさとかを
	//先ほどの円を作る時の36度の回転がヒントになるかも

	//Quaternion rot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(cntAngle), AsoUtility::AXIS_Y);
	
	
	//VECTOR localPos = rot.PosAxis({ 0.0f,0.0f,mRadius });

	//位置？(難易度３)

	//頂点座標(クォータニオン)
		Quaternion rotY = Quaternion::AngleAxis(AsoUtility::Deg2RadF(static_cast<float>(GetRand(360))), 
			AsoUtility::AXIS_Y);

		float min = mRadius * (3.0f / 4.0f);
		//座標を回転
		VECTOR pos = rotY.PosAxis({ 0.0f,0.0f,static_cast<float>(
			min +GetRand(AsoUtility::Round(mRadius-min)))});

		//中心座標に加える。
		VECTOR randPos = VAdd(mPos, pos);


	

	//大きさ 難易度 1
	float size = GEN_MIN_SIZE +GetRand(static_cast<int>(GEN_MAX_SIZE-GEN_MIN_SIZE));
	//方向　難易度 5

	float randDegX = GEN_MIN_X_ROT + GetRand(
		static_cast<int>(GEN_MAX_X_ROT - GEN_MIN_X_ROT));


	Quaternion rotX = Quaternion::AngleAxis(
		AsoUtility::Deg2RadF(randDegX), AsoUtility::AXIS_X);

	rotY = rotY.Mult(rotY);//こいつがその一行


	//rotXをdirのvectorに変換

	//行列編
	//VECTOR dir = VTransform(AsoUtility::AXIS_Z, rotX.ToMatrix());//Zの正方向に飛ばす
	//VECTOR dir = rotX.PosAxis(rotY,AsoUtility::AXIS_Z);
	VECTOR dir = rotX.GetForward();
	//無回転の場合はzの静方向




	//俺流。あってるかどうか
	/*VECTOR dir = { 0.0f,1.0f,0.0f};
	Quaternion tmpQ=rotX.PosAxis(rotX, dir);
	Quaternion mAxisQ = mAxisQ.Mult(tmpQ);*/
	



	//スピード 難易度 1
	float speed = GEN_MIN_SPEED + GetRand(static_cast<int>(GEN_MAX_SPEED - GEN_MIN_SPEED));
	//生存時間 難易度 1
	float lifetime = GEN_MIN_LIFE_TIME + GetRand(static_cast<int>(GEN_MAX_LIFE_TIME - GEN_MIN_LIFE_TIME));

	rotY = mRot.Mult(rotY);


	//生成
	particle->Generate(mImage, pos, size,dir, speed, lifetime);



	return particle;
}

void ParticleGenerator::ZSort(void)
{

	//カメラ
	Camera* camera=SceneManager::GetInstance().GetCamera();
	//1カメラの方向
	VECTOR cameraDir = camera->GetDir();

	

	for (auto p: mParticles_) 
	{
		//いろいろ計算
		
		//カメラから見たエフェクト方向
	

		//Z距離を設定
		//camera2Particle
		//cameraDir

		

		VECTOR NormCam=VNorm(cameraDir);

		VECTOR camera2Particle;
		camera2Particle = VSub(p->GetPos(), camera->GetPos());
		p->SetZLen(VDot(cameraDir, camera2Particle));

	}

	//ソート
	sort(mParticles_.begin(), mParticles_.end(),
		[](Particle* x, Particle* y)
	{
			return x->GetZLen() > y->GetZLen(); 
	}
	);
}
