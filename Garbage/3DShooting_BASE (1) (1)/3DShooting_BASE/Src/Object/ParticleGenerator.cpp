#include "ParticleGenerator.h"
#include"../Utility/AsoUtility.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"Particle.h"
#include"../Manager/SceneManager.h"
#include"../Manager/Camera.h"

ParticleGenerator::ParticleGenerator(VECTOR pos, float radius)
{
    mPos = pos;
    mRadius = radius;
}

ParticleGenerator::~ParticleGenerator(void)
{

}

void ParticleGenerator::Init(void)
{
    mImg = ResourceManager::GetInstance().Load(ResourceManager::SRC::LIGHT).mHandleId;

    // 四角メッシュを作成
    CreateMeshSquare();

    // 円形メッシュを作成
    CreateMeshCircle();

   
    //一気に
    //for (int i = 0; i < NUM_CREATE; i++)
    //{
    //    mParticles.emplace_back(Generate(nullptr));
    //}

    for (int i = 0; i < NUM_CREATE; i++)
    {
        mParticles.emplace_back(new Particle());
    }


    // 角度の初期化
    mRot = Quaternion::Euler(0.0f, 0.0f, 0.0f);

    mStepGenerate = GEN_TIME;

}

void ParticleGenerator::Update(void)
{

    bool isGenerate = false;
    mStepGenerate -= SceneManager::GetInstance().GetDeltaTime();
    if (mStepGenerate<0.0f)
    {
        isGenerate = true;
      
    }

    for (auto& p : mParticles)
    {
        p->Update();

        // 生存時間が切れたら再生成
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

#ifdef _DEBUG

     //ParticleGenerator = mRotの回転処理

    //if (particle == nullptr)
    //{
    //    particle = new Particle();
    //}

    //// 右回転
    //if (CheckHitKey(KEY_INPUT_L))
    //{

    //}

#endif

}

void ParticleGenerator::Draw(void)
{
    DrawMeshSquare();
    DrawMeshCircle();

    ZSort();

   // C(DX_BLENDMODE_ADD, 128);
    for (auto& p : mParticles)
    {
        p->Draw();
    }
  //  SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

    //白い線を描画
    Quaternion tmpQ = Quaternion::Euler(
        AsoUtility::Deg2RadF(-90.0f),
        0.0f,
        0.0f
    );

    tmpQ = mRot.Mult(tmpQ);

    DrawLine3D(
        mPos,
        VAdd(mPos, VScale(tmpQ.GetForward(), 300.0f)), 0xffffff);

}

void ParticleGenerator::DrawMeshSquare(void)
{
    DrawPolygon3D(mVerticesSquare, 2, DX_NONE_GRAPH, true);
   // DrawPolygon3D(mVerticesSquare, 1, DX_NONE_GRAPH, true);
}

void ParticleGenerator::DrawMeshCircle(void)
{
    DrawPolygonIndexed3D(mVerticesCircle, NUM_ANGLE + 1, mIndexes, NUM_ANGLE, DX_NONE_GRAPH, true);

    for (auto& vert : mVerticesCircle)
    {
        DrawSphere3D(vert.pos, 2.0f, 5, 0xff0000, 0xff0000, true);
    }
   
}


void ParticleGenerator::Release(void)
{
    for (auto& p : mParticles)
    {
        if (p != nullptr)
        {
            p->Release();
            delete p;
        }
    }
    mParticles.clear();
}

void ParticleGenerator::SetPos(const VECTOR& pos)
{
    mPos = pos;
    //デッバグ用
    CreateMeshSquare();

}

void ParticleGenerator::SetRot(const Quaternion& rot)
{
    mRot = rot;
}

void ParticleGenerator::CreateMeshSquare(void)
{
    // ディフューズカラー
    COLOR_U8 color = GetColorU8(255, 255, 255, 255);

    float z = 0.0f;
    float h = 100.0f;

    // 四角を作る
    mVerticesSquare[0].pos = { z,z,z };
    mVerticesSquare[0].dif = color;
    mVerticesSquare[1].pos = { z,z,h };
    mVerticesSquare[1].dif = color;
    mVerticesSquare[2].pos = { h,z,h };
    mVerticesSquare[2].dif = color;
    mVerticesSquare[3].pos = { z,z,z };
    mVerticesSquare[3].dif = color;
    mVerticesSquare[4].pos = { h,z,h };
    mVerticesSquare[4].dif = color;
    mVerticesSquare[5].pos = { h,z,z };
    mVerticesSquare[5].dif = color;
}

void ParticleGenerator::CreateMeshCircle(void)
{
    // ディフューズカラー
    COLOR_U8 colorW = GetColorU8(255, 255, 255, 255);
    COLOR_U8 colorR = GetColorU8(255, 0, 0, 255);
    COLOR_U8 colorG = GetColorU8(0, 255, 0, 255);

    mCntVertex = 0;

    // 円の中心
    mVerticesCircle[0].pos = mPos;
    mVerticesCircle[0].dif = colorW;

    float cntAngle = 0.0f;

    // 円の頂点を作成
    for (int i = 0; i < NUM_ANGLE; i++)
    {
        Quaternion rot = Quaternion::AngleAxis(
            AsoUtility::Deg2RadF(cntAngle), AsoUtility::AXIS_Y);

        rot = mRot.Mult(rot);

        //座標を回転
        VECTOR localPos = rot.PosAxis({ 0.0f,0.0f,mRadius });

        VERTEX3D vertex = VERTEX3D();
        //中心座標に加えて頂点座標とする
        vertex.pos = VAdd(mPos, localPos);
        vertex.dif = colorG;

        mVerticesCircle[i + 1] = vertex;

        cntAngle += SPLIT_ANGLE;
    }

    // 頂点インデックス
    int cntIdx = 0;
    int cntVIdx = 1;

    for (int i = 1; i < NUM_ANGLE; i++)
    {
        mIndexes[cntIdx++] = 0;
        mIndexes[cntIdx++] = cntVIdx;
        mIndexes[cntIdx++] = ++cntVIdx;
    }
    mIndexes[cntIdx++] = 0;
    mIndexes[cntIdx++] = cntVIdx;
    mIndexes[cntIdx++] = 1;
}

Particle* ParticleGenerator::Generate(Particle* particle)
{
    if (particle == nullptr)
    {
        particle = new Particle();
    }
    // ランダムにする
    // 最小値 + GetRand(最大値 - 最小値)
    // VECTOR pos = { 0.0f,0.0f,0.0f };
    float size = GEN_MIN_SIZE + GetRand(GEN_MAX_SIZE - GEN_MIN_SIZE);
    //VECTOR dir = { 0.0f,1.0f,0.0f };
    float speed = GEN_MIN_SPEED + GetRand(GEN_MAX_SPEED - GEN_MIN_SPEED);
    float lifeTime = GEN_MIN_LIFE_TIME + GetRand(GEN_MAX_LIFE_TIME - GEN_MIN_LIFE_TIME);

    // 発生位置
    // 頂点座標
    Quaternion rotY = Quaternion::AngleAxis(
        AsoUtility::Deg2RadF(static_cast<float>(GetRand(360))), AsoUtility::AXIS_Y);
    // 座標を回転
    float min = mRadius * (3.0f / 4.0f);
    VECTOR pos = rotY.PosAxis(
        { 0.0f,0.0f,static_cast<float>(min+GetRand(AsoUtility::Round(mRadius-min))) });//@@@
    // 中心座標に加える
    VECTOR randPos = VAdd(mPos, pos);

    // 移動方向
    float randDegX = GEN_MIN_X_ROT + GetRand(
        static_cast<int>(GEN_MAX_X_ROT - GEN_MIN_X_ROT));
    Quaternion rotX = Quaternion::AngleAxis(
        AsoUtility::Deg2RadF(randDegX), AsoUtility::AXIS_X);
    rotX = rotY.Mult(rotX);
    VECTOR dir = VTransform({ 0.0f,0.0f,1.0f }, rotX.ToMatrix());
    //VECTOR dir = rotX.PosAxis();


    particle->Generate(mImg, pos, size, dir, speed, lifeTime);

    return particle;
}

void ParticleGenerator::ZSort(void)
{
    // カメラ
    Camera* camera = SceneManager::GetInstance().GetCamera();
    // カメラの方向
    VECTOR cameraDir = camera->GetDir();

    for (auto p : mParticles)
    {
        // カメラから見たエフェクト方向
        VECTOR camera2Particle;
        // エフェクト方向(エフェクト位置 - カメラ位置)
        camera2Particle = VSub(p->GetPos(), camera->GetPos());


        // Z距離を設定
        p->SetZLen(VDot(cameraDir, camera2Particle));
    }

    // ソート
    sort(mParticles.begin(), mParticles.end(),
        [](Particle* x, Particle* y) {return x->GetZLen() > y->GetZLen(); }
    );



}