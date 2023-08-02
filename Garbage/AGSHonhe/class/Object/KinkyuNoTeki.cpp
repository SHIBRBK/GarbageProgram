#include <DxLib.h>
#include<EffekseerForDXLib.h>
#include "../../Resource.h"
#include "KinkyuNoTeki.h"
#include"../Manager/SceneManager.h"

KinkyuNoTeki::KinkyuNoTeki()
{
    hp_ = 0;

}

KinkyuNoTeki::~KinkyuNoTeki()
{
}

bool KinkyuNoTeki::Init()
{
    transform_.modelId = Resource::LoadModel(Resource::PATH_MODEL + "Enemy/Fortress.mv1");
    transform_.scl = { 40.0f, 40.0f, 40.0f };
    transform_.rot = { 0.0f,180.0f,0.0f };
    transform_.pos = { -1400.0f,-350.0f,6000.0f };


    ChangeState(STATE::ALIVE);
    hp_ = 10;
    //transform_.matScl= MScale(transform_.matScl,400.0f);
    //transform_.matRot = MGetIdent();
    //transform_.matPos= MGetIdent();

    DeadExplo = LoadEffekseerEffect((Resource::PATH_EFFECT + "Explosion.efkefc").c_str(), 1.0f);

    DeadExploPlay = -1;

    return true;
}

void KinkyuNoTeki::Update()
{
    MV1SetScale(transform_.modelId, transform_.scl);
    MV1SetPosition(transform_.modelId, transform_.pos);
    //MV1SetMatrix(transform_.modelId, transform_.matScl);
    //MV1SetMatrix(transform_.modelId, transform_.matRot);
    //MV1SetMatrix(transform_.modelId, transform_.matPos);
    //MV1SetRotationMatrix(transform_.modelId, transform_.matRot);//とりあえず、保持
    switch (state_)
    {
    case KinkyuNoTeki::STATE::ALIVE:

        for (auto s : shots_)
        {
            s->Update();
        }
        ProcessShot();


        break;
    case KinkyuNoTeki::STATE::DEAD:

        break;
    }
}

void KinkyuNoTeki::UpdateDead()
{
}

void KinkyuNoTeki::Draw()
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

void KinkyuNoTeki::Release()
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

void KinkyuNoTeki::AddCollider(Collider* collider)
{
    colliders_.push_back(collider);
}

void KinkyuNoTeki::ClearCollider(void)
{
    colliders_.clear();
}


std::vector<EnemyShot*>& KinkyuNoTeki::GetShots(void)
{
    return shots_;
}

void KinkyuNoTeki::ProcessShot(void)
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

void KinkyuNoTeki::CreateShot(void)
{
    bool isCreate = false;
    auto matF = MV1GetFrameLocalWorldMatrix(transform_.modelId, 2);
    auto rotF = MGetRotElem(matF);
    auto rotQuaF = Quaternion::GetRotation(rotF);
    auto framePosF = MV1GetFramePosition(transform_.modelId, 2);
    VECTOR offsetF = { 0.0f,0.0f,0.0f };
    auto slidePosF = VAdd(framePosF, rotQuaF.PosAxis(offsetF));


    for (auto s : shots_)
    {
        if (!s->IsAlive())
        {
            s->CreateShot(framePosF, transform_.quaRot.GetBack());

            isCreate = true;
        }
    }
    if (!isCreate)
    {
        auto newShot = new EnemyShot(&transform_);
        newShot->CreateShot(framePosF, transform_.quaRot.GetBack());

        shots_.emplace_back(newShot);
    }
}

bool KinkyuNoTeki::IsAlive(void) const
{
    return state_ == STATE::ALIVE;
}

bool KinkyuNoTeki::IsDead(void) const
{
    return state_ == STATE::DEAD;
}

const Transform& KinkyuNoTeki::GetTransform(void) const
{
    return transform_;
}

float KinkyuNoTeki::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

void KinkyuNoTeki::ChangeState(STATE state)
{
    //状態を更新
    state_ = state;
    //状態遷移時の初期化
    switch (state_)
    {

    case KinkyuNoTeki::STATE::ALIVE:

        break;
    case KinkyuNoTeki::STATE::DEAD:
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
