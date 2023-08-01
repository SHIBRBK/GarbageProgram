#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "../../Resource.h"
#include "EnemyCannon.h"
#include"../Manager/SceneManager.h"


EnemyCannon::EnemyCannon() :EnemyBase()
{
    hp_ = 0;
}

EnemyCannon::~EnemyCannon()
{
}

bool EnemyCannon::Init()
{
    transform_.modelId = Resource::LoadModel(Resource::PATH_MODEL + "Enemy/Fortress.mv1");
    transform_.scl = { 40.0f, 40.0f, 40.0f };
    transform_.rot = { 0.0f,180.0f,0.0f };
    transform_.pos = { 1400.0f,-350.0f,6000.0f };

    ChangeState(STATE::ALIVE);
    hp_ = 10;
    //transform_.matScl= MScale(transform_.matScl,400.0f);
    //transform_.matRot = MGetIdent();
    //transform_.matPos= MGetIdent();

    DeadExplo = LoadEffekseerEffect((Resource::PATH_EFFECT+"Explosion.efkefc").c_str(), 1.0f);

    DeadExploPlay = -1;

    return true;
}

void EnemyCannon::Update()
{
    MV1SetScale(transform_.modelId, transform_.scl);
    MV1SetPosition(transform_.modelId, transform_.pos);
    //MV1SetMatrix(transform_.modelId, transform_.matScl);
    //MV1SetMatrix(transform_.modelId, transform_.matRot);
    //MV1SetMatrix(transform_.modelId, transform_.matPos);
    //MV1SetRotationMatrix(transform_.modelId, transform_.matRot);//とりあえず、保持
    switch (state_)
    {
    case EnemyCannon::STATE::ALIVE:
        
        for (auto s : shots_)
        {
            s->Update();
        }
        ProcessShot();
     

        break;
    case EnemyCannon::STATE::DEAD:
        Damage();
        break;
    }
}


void EnemyCannon::UpdateDead()
{
}

void EnemyCannon::Draw()
{
    if (stepDamaged_ > 0.0f)
    {
        MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.8f, 0.1f, 0.1f, 0.8f));
        MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.8f, 0.1f, 0.1f, 0.8f));
    }
    else
    {
        MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.48f, 0.52f, 0.4f, 1.0f));
        MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.48f, 0.52f, 0.4f, 1.0f));
    }
    MV1DrawModel(transform_.modelId);
    for (auto s : shots_)
    {
        s->Draw();
    }
}

void EnemyCannon::Release()
{
    //弾の解放
    for (auto s : shots_)
    {
        s->Release();
        delete s;
    }
    shots_.clear();
    MV1DeleteModel(transform_.modelId);
}

void EnemyCannon::AddCollider(Collider* collider)
{
    colliders_.push_back(collider);
}

void EnemyCannon::ClearCollider(void)
{
    colliders_.clear();
}

void EnemyCannon::Damage()
{
   
    stepDamaged_ = TIME_DAMAGED_EFFECT;

    hp_ -= 1;
    DrawFormatString(0, 180, 0xffffff, "%d", hp_);
    if (hp_ <= 0)
    {
        ChangeState(STATE::DEAD);
    }
}

std::vector<EnemyShot*>& EnemyCannon::GetShots(void)
{
    return shots_;
}

void EnemyCannon::ProcessShot(void)
{
    stepShotDelay_ -= SceneManager::GetInstance().GetDeltaTime();
    if (stepShotDelay_ < 0.0f)
    {
        stepShotDelay_ = 0.0f;
    }
    
    //キーチェック
    if (stepShotDelay_ <= 0.0f)
    {
        //ディレイ時間をセット
        stepShotDelay_ = TIME_DELAY_SHOT;

        //弾を生成
        CreateShot();

    }
}

void EnemyCannon::CreateShot(void)
{
    bool isCreate = false;
    auto matF = MV1GetFrameLocalWorldMatrix(transform_.modelId, 2);
    auto rotF = MGetRotElem(matF);
    auto rotQuaF = Quaternion::GetRotation(rotF);
    auto framePosF = MV1GetFramePosition(transform_.modelId, 2);
    VECTOR offsetF = { 0.0f,0.0f,0.0f };
    auto slidePosF = VAdd(framePosF, rotQuaF.PosAxis(offsetF));

    auto matS = MV1GetFrameLocalWorldMatrix(transform_.modelId, 11);
    auto rotS = MGetRotElem(matS);
    auto rotQuaS = Quaternion::GetRotation(rotS);
    auto framePosS = MV1GetFramePosition(transform_.modelId, 11);
    VECTOR offsetS = { 0.0f,0.0f,0.0f };
    auto slidePosS = VAdd(framePosS, rotQuaS.PosAxis(offsetS));


    auto matT = MV1GetFrameLocalWorldMatrix(transform_.modelId, 20);
    auto rotT = MGetRotElem(matT);
    auto rotQuaT = Quaternion::GetRotation(rotT);
    auto framePosT = MV1GetFramePosition(transform_.modelId, 20);
    VECTOR offsetT = { 0.0f,0.0f,0.0f };
    auto slidePosT = VAdd(framePosT, rotQuaT.PosAxis(offsetT));



    auto matFor = MV1GetFrameLocalWorldMatrix(transform_.modelId, 29);
    auto rotFor = MGetRotElem(matFor);
    auto rotQuaFor = Quaternion::GetRotation(rotFor);
    auto framePosFor = MV1GetFramePosition(transform_.modelId, 29);
    VECTOR offsetFor = { 0.0f,0.0f,0.0f };
    auto slidePosFor = VAdd(framePosFor, rotQuaFor.PosAxis(offsetFor));



    auto matFiv = MV1GetFrameLocalWorldMatrix(transform_.modelId, 38);
    auto rotFiv = MGetRotElem(matFiv);
    auto rotQuaFiv = Quaternion::GetRotation(rotFiv);
    auto framePosFiv = MV1GetFramePosition(transform_.modelId, 38);
    VECTOR offsetFiv = { 0.0f,0.0f,0.0f };
    auto slidePosFiv = VAdd(framePosFiv, rotQuaFiv.PosAxis(offsetFiv));


    auto matSix = MV1GetFrameLocalWorldMatrix(transform_.modelId, 47);
    auto rotSix = MGetRotElem(matSix);
    auto rotQuaSix = Quaternion::GetRotation(rotSix);
    auto framePosSix = MV1GetFramePosition(transform_.modelId, 47);
    VECTOR offsetSix = { 0.0f,0.0f,0.0f };
    auto slidePosSix = VAdd(framePosSix, rotQuaSix.PosAxis(offsetSix));


    auto matSev = MV1GetFrameLocalWorldMatrix(transform_.modelId, 56);
    auto rotSev = MGetRotElem(matSev);
    auto rotQuaSev = Quaternion::GetRotation(rotSev);
    auto framePosSev = MV1GetFramePosition(transform_.modelId, 56);
    VECTOR offsetSev = { 0.0f,0.0f,0.0f };
    auto slidePosSev = VAdd(framePosSev, rotQuaSev.PosAxis(offsetSev));


    for (auto s : shots_)
    {
        if (!s->IsAlive())
        {
            s->CreateShot(framePosF, transform_.quaRot.GetForward());
            s->CreateShot(framePosS, transform_.quaRot.GetForward());
            s->CreateShot(framePosT, transform_.quaRot.GetForward());
            s->CreateShot(framePosFor, transform_.quaRot.GetForward());
            s->CreateShot(framePosFiv, transform_.quaRot.GetForward());
            s->CreateShot(framePosSix, transform_.quaRot.GetForward());
            s->CreateShot(framePosSev, transform_.quaRot.GetForward());
            isCreate = true;
        }
    }
    if (!isCreate)
    {
        auto newShot = new EnemyShot(&transform_);
        newShot->CreateShot(framePosF, transform_.quaRot.GetForward());
        newShot->CreateShot(framePosS, transform_.quaRot.GetForward());
        newShot->CreateShot(framePosT, transform_.quaRot.GetForward());
        newShot->CreateShot(framePosFor, transform_.quaRot.GetForward());
        newShot->CreateShot(framePosFiv, transform_.quaRot.GetForward());
        newShot->CreateShot(framePosSix, transform_.quaRot.GetForward());
        newShot->CreateShot(framePosSev, transform_.quaRot.GetForward());

        shots_.emplace_back(newShot);
    }
}

bool EnemyCannon::IsAlive(void) const
{
    return state_ == STATE::ALIVE;
}

bool EnemyCannon::IsDead(void) const
{
    return state_ == STATE::DEAD;
}

const Transform& EnemyCannon::GetTransform(void) const
{
    return transform_;
}

void EnemyCannon::ChangeState(STATE state)
{
    //状態を更新
    state_ = state;
    //状態遷移時の初期化
    switch (state_)
    {

    case EnemyCannon::STATE::ALIVE:

        break;
    case EnemyCannon::STATE::DEAD:
    {



        //爆発エフェクトを出す
        DeadExploPlay = PlayEffekseer3DEffect(DeadExplo);
        VECTOR pos = transform_.pos;
        pos = VAdd(pos, VScale(transform_.GetForward(), 1.0f));

        //位置
        SetPosPlayingEffekseer3DEffect(DeadExploPlay, pos.x, pos.y, pos.z);
        //大きさ
        SetScalePlayingEffekseer3DEffect(DeadExploPlay, 200.0f, 200.0f, 200.0f);
        //回転
        SetRotationPlayingEffekseer3DEffect(DeadExploPlay, 0.0f, 0.0f, 0.0f);
    }
    break;
    }
}

