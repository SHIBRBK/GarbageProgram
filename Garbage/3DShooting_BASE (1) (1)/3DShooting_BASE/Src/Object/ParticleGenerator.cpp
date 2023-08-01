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

    // �l�p���b�V�����쐬
    CreateMeshSquare();

    // �~�`���b�V�����쐬
    CreateMeshCircle();

   
    //��C��
    //for (int i = 0; i < NUM_CREATE; i++)
    //{
    //    mParticles.emplace_back(Generate(nullptr));
    //}

    for (int i = 0; i < NUM_CREATE; i++)
    {
        mParticles.emplace_back(new Particle());
    }


    // �p�x�̏�����
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

        // �������Ԃ��؂ꂽ��Đ���
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

     //ParticleGenerator = mRot�̉�]����

    //if (particle == nullptr)
    //{
    //    particle = new Particle();
    //}

    //// �E��]
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

    //��������`��
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
    //�f�b�o�O�p
    CreateMeshSquare();

}

void ParticleGenerator::SetRot(const Quaternion& rot)
{
    mRot = rot;
}

void ParticleGenerator::CreateMeshSquare(void)
{
    // �f�B�t���[�Y�J���[
    COLOR_U8 color = GetColorU8(255, 255, 255, 255);

    float z = 0.0f;
    float h = 100.0f;

    // �l�p�����
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
    // �f�B�t���[�Y�J���[
    COLOR_U8 colorW = GetColorU8(255, 255, 255, 255);
    COLOR_U8 colorR = GetColorU8(255, 0, 0, 255);
    COLOR_U8 colorG = GetColorU8(0, 255, 0, 255);

    mCntVertex = 0;

    // �~�̒��S
    mVerticesCircle[0].pos = mPos;
    mVerticesCircle[0].dif = colorW;

    float cntAngle = 0.0f;

    // �~�̒��_���쐬
    for (int i = 0; i < NUM_ANGLE; i++)
    {
        Quaternion rot = Quaternion::AngleAxis(
            AsoUtility::Deg2RadF(cntAngle), AsoUtility::AXIS_Y);

        rot = mRot.Mult(rot);

        //���W����]
        VECTOR localPos = rot.PosAxis({ 0.0f,0.0f,mRadius });

        VERTEX3D vertex = VERTEX3D();
        //���S���W�ɉ����Ē��_���W�Ƃ���
        vertex.pos = VAdd(mPos, localPos);
        vertex.dif = colorG;

        mVerticesCircle[i + 1] = vertex;

        cntAngle += SPLIT_ANGLE;
    }

    // ���_�C���f�b�N�X
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
    // �����_���ɂ���
    // �ŏ��l + GetRand(�ő�l - �ŏ��l)
    // VECTOR pos = { 0.0f,0.0f,0.0f };
    float size = GEN_MIN_SIZE + GetRand(GEN_MAX_SIZE - GEN_MIN_SIZE);
    //VECTOR dir = { 0.0f,1.0f,0.0f };
    float speed = GEN_MIN_SPEED + GetRand(GEN_MAX_SPEED - GEN_MIN_SPEED);
    float lifeTime = GEN_MIN_LIFE_TIME + GetRand(GEN_MAX_LIFE_TIME - GEN_MIN_LIFE_TIME);

    // �����ʒu
    // ���_���W
    Quaternion rotY = Quaternion::AngleAxis(
        AsoUtility::Deg2RadF(static_cast<float>(GetRand(360))), AsoUtility::AXIS_Y);
    // ���W����]
    float min = mRadius * (3.0f / 4.0f);
    VECTOR pos = rotY.PosAxis(
        { 0.0f,0.0f,static_cast<float>(min+GetRand(AsoUtility::Round(mRadius-min))) });//@@@
    // ���S���W�ɉ�����
    VECTOR randPos = VAdd(mPos, pos);

    // �ړ�����
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
    // �J����
    Camera* camera = SceneManager::GetInstance().GetCamera();
    // �J�����̕���
    VECTOR cameraDir = camera->GetDir();

    for (auto p : mParticles)
    {
        // �J�������猩���G�t�F�N�g����
        VECTOR camera2Particle;
        // �G�t�F�N�g����(�G�t�F�N�g�ʒu - �J�����ʒu)
        camera2Particle = VSub(p->GetPos(), camera->GetPos());


        // Z������ݒ�
        p->SetZLen(VDot(cameraDir, camera2Particle));
    }

    // �\�[�g
    sort(mParticles.begin(), mParticles.end(),
        [](Particle* x, Particle* y) {return x->GetZLen() > y->GetZLen(); }
    );



}