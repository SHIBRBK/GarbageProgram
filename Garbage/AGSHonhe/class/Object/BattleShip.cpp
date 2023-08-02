#include <algorithm>
#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Resource.h"
#include"Common/Collider.h"
#include "BattleShip.h"
#include"../Manager/SceneManager.h"

BattleShip::BattleShip() :PlayerBase()
{

}

BattleShip::~BattleShip()
{
}

bool BattleShip::Init()
{

    transform_.modelId = Resource::LoadModel(Resource::PATH_MODEL + "dairi.mv1");
    transform_.scl = VGet(SCALE, SCALE, SCALE);
    /* matScl = MGetIdent();
     matRot = MGetIdent();
     matPos = MGetIdent();
     MV1SetMatrix(transform_.modelId,transform_.matScl);
     MV1SetMatrix(transform_.modelId,transform_.matRot);
     MV1SetMatrix(transform_.modelId,transform_.matPos);*/

    transform_.quaRot = Quaternion();
    transform_.quaRotLocal =
        Quaternion::Euler(
            0.0f,
            DegToRad(180.0f),
            0.0f
        );
    transform_.pos = { 0.0f,0.0f,0.0f };
    rotationAngle = 0.0f;
    isMovingForward = false;
    isMovingBack = false;

    maxRudderAngle = 45.0f;  // �ő�ǂ̊p�x�i�x�j
    rudderAngle = 0.0f;  // ���݂̑ǂ̊p�x�i�x�j
    rudderSpeed = 30.0f;  // �ǂ̉�]���x�i�x/�b�j

    baseRotation = 0.0f;
    angles_ = 0.0f;
    rot_ = { 0.0f, 0.0f, 0.0f };
    currentSpeedLevel = SpeedLevel::STOP;
    maxspeed_ = 12.0f;
    steeringAngle = 0.0f;
    rotationSpeed = 0.1f;
    // MV1SetPosition(modelId_, pos_);

   // shotModelId_ = Resource::LoadModel(Resource::PATH_MODEL + "shell/shell.mv1");
    stepShotDelay_ = 0.0f;
    //�ϋv��
    hp_ = 50.0f;

    LoadDivGraph((Resource::PATH_TEXTURE + "Blast.png").c_str(),
        BLAST_ANIM_NUM, 4, 4, BLAST_SIZE_X, BLAST_SIZE_Y, blastImgs_, true);

    MV1SetDifColorScale(transform_.modelId, GetColorF(255, 0, 255, 1));
    //Laser = Resource::LoadEffect(Resource::PATH_EFFECT+"Laser01.efkproj");
    //������Ԃ𑖍s��Ԃɂ���
    ChangeState(STATE::BATTLE);
    


    return true;
}

void BattleShip::Update()
{

    switch (state_)
    {
    case BattleShip::STATE::BATTLE:
        UpdateAlive();
        break;
    case BattleShip::STATE::DESTROY:
        break;
    case BattleShip::STATE::END:
        break;
    }


}

void BattleShip::UpdateAlive()
{
    float steeringInterpolationSpeed = 2.0f;
    // �f���^�^�C��
    auto mPreTime = std::chrono::system_clock::now();
    auto currentFrameTime = std::chrono::system_clock::now();
    float deltaTime = static_cast<float>(
        std::chrono::duration_cast<std::chrono::nanoseconds>(currentFrameTime - mPreTime).count() / 1000000000.0);
    mPreTime = currentFrameTime;
    Quaternion currentSteeringRotation;

    float acceleration = 0.0f;         // ���݂̉����x
    float maxAcceleration = 1.0f;      // �����x�̏���l
    float accelerationRate = 0.01f;     // �����x�̑�����
    currentSpeed = 0.0f;
    float maxSpeed = GetSpeedFromLevel(SpeedLevel::MAX);
    // �����x�̏���l�𒴂��Ȃ��悤�ɐ���
    acceleration = (std::min)(acceleration, maxAcceleration);


    bool previousKeyState = false;  // �O��̃L�[���͏��
    bool currentKeyState = false;   // ���݂̃L�[���͏��
    // �L�[�̓��͏�Ԃ��擾
    currentKeyState = CheckHitKey(KEY_INPUT_W);
    if (currentKeyState && !previousKeyState)
    {
        if (currentSpeedLevel < SpeedLevel::MAX)
        {
            currentSpeedLevel =
                static_cast<SpeedLevel>(
                    static_cast<int>(
                        currentSpeedLevel) + 1);

        }
    }
    // ���݂̃L�[���͏�Ԃ�O��̃L�[���͏�ԂɍX�V
    previousKeyState = currentKeyState;

    // �L�[�̓��͏�Ԃ��擾
    currentKeyState = CheckHitKey(KEY_INPUT_S);
    if (currentKeyState && !previousKeyState)
    {
        if (currentSpeedLevel != SpeedLevel::BACK)
        {
            currentSpeedLevel =
                static_cast<SpeedLevel>(
                    static_cast<int>(
                        currentSpeedLevel) - 1);

        }

    }




    currentSpeed = GetSpeedFromLevel(
        currentSpeedLevel);

    // �ڕW�X�s�[�h���v�Z����
    float targetSpeed = GetSpeedFromLevel(currentSpeedLevel);
    if (currentSpeed < targetSpeed)
    {
        acceleration += accelerationRate * deltaTime;
        currentSpeed = (std::min)(currentSpeed, targetSpeed);
    }
    else if (currentSpeed > targetSpeed)
    {
        acceleration -= accelerationRate * deltaTime;
        currentSpeed = (std::max)(currentSpeed, -targetSpeed);
    }
    // ���x���X�V
    currentSpeed += acceleration * deltaTime;
    // ���x�̏���l�𒴂��Ȃ��悤�ɐ���
    currentSpeed = (std::min)(currentSpeed, maxSpeed);
    //if (currentSpeed >= 0 && targetSpeed >= 0)
    //{
    //    currentSpeed = std::clamp(currentSpeed, 0.0f, targetSpeed);//3�߂̈����ɂ͐^�񒆂̈��������傫���̓����ƈ�ԍ��̒l�����߂����
    //}
    // if (currentSpeed <= 0 && targetSpeed <= 0)
    //{
    //    currentSpeed = std::clamp(currentSpeed, targetSpeed, 0.0f);
    //}

    VECTOR velocity = VScale(
        transform_.GetForward(), currentSpeed);
    transform_.pos = VAdd(transform_.pos, velocity);



    if (currentSpeedLevel == SpeedLevel::STOP)
    {
        if (CheckHitKey(KEY_INPUT_A) || CheckHitKey(KEY_INPUT_D))
        {
            return;
        }
    }

    if (CheckHitKey(KEY_INPUT_A))
    {
        Turn(-SPEED_ROT_Y, { 0.0f,1.0f,0.0f });

    }
    else if (CheckHitKey(KEY_INPUT_D))
    {
        Turn(SPEED_ROT_Y, { 0.0f,1.0f,0.0f });
    }

    else
    {
        float deceleration = SPEED_ROT_Y * deltaTime;
        currentSteeringRotation = Quaternion::Slerp(currentSteeringRotation, Quaternion(), deceleration);
    }


    //transform_.get()->quaRot =
    //    transform_.get()->
    //    quaRot.Mult(transform_.get()->quaRotLocal);

    currentSteeringRotation = Quaternion::Slerp(
        transform_.quaRotLocal
        , axisQ,
        SPEED_ROT_Y * deltaTime);

    transform_.quaRotLocal = currentSteeringRotation;
    MV1SetPosition(transform_.modelId, transform_.pos);

    ProcessRot();
    ProcessShot();
    TurretRot();

    transform_.Update();

    //�����蔻��
    int ret = MV1RefreshCollInfo(transform_.modelId, -1);
}

void BattleShip::UpdateDead()
{
}



void BattleShip::Draw()
{
    ////�傫���@��]�@�ʒu
    //auto mat = MV1GetFrameBaseLocalMatrix(transform_.modelId, 1);// �w��̃t���[���̏�����Ԃł̍��W�ϊ��s����擾����

    //auto scale = MGetSize(mat);//�傫��

    //auto matscl = MGetScale(scale);// �g��s����擾����

    //auto pos = MGetTranslateElem(mat);//�s��̕��s�ړ��������擾����

    ////VECTOR ps=MV1GetFramePosition(transform_.modelId,1);

    //VECTOR TurretLocalPos = TurretQua.PosAxis({ 0.0f, -85.3f , 1170.0f });
    //pos = VAdd(pos, TurretLocalPos);

    //auto matPos = MGetTranslate(pos);// ���s�ړ��s����擾����

    //mat = MMult(mat, matscl);
    //mat = MMult(mat, TurretQua.ToMatrix());
    //mat = MMult(mat, matPos);
    //MV1SetFrameUserLocalMatrix(transform_.modelId, 1, mat);

    if (mStepDamaged > 0.0f)
    {
        MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.8f, 0.1f, 0.1f, 0.8f));
    }
    else
    {
        MV1SetMaterialDifColor(transform_.modelId, 0, GetColorF(0.48f, 0.52f, 0.4f, 1.0f));
    }

    MV1DrawModel(transform_.modelId);
    transform_.Update();

    DrawFormatString(10, 10, 0xffffff, "x=%f,y=%f,z=%f",
        transform_.pos.x,
        transform_.pos.y,
        transform_.pos.z);
    DrawFormatString(10, 30, 0xffffff, "x=%f,y=%f,z=%f",
        transform_.quaRot.x,
        transform_.quaRot.y,
        transform_.quaRot.z);
    DrawFormatString(10, 50, 0xffffff, "x=%d", static_cast<int>(
        currentSpeedLevel));
    DrawFormatString(10, 70, 0xffffff, "x=%f", currentSpeed);
    DrawFormatString(10, 140, 0xffffff, "x=%d", hp_);
   // DrawFormatString(10, 130, 0xffffff, "x=%d", shotModelId_);
   // DrawFormatString(10, 150, 0xffffff, "x=%f,y=%f,z=%f", pos.x, pos.y, pos.z);
}

void BattleShip::DrawAlive()
{

}

void BattleShip::DrawDead()
{
}

void BattleShip::Damage()
{
    mStepDamaged = TIME_DAMAGED_EFFECT;
    hp_ -= 1;

    if (hp_ <= 0)
    {
        ChangeState(STATE::DESTROY);
    }
}

void BattleShip::Release()
{
    for (auto shot : shots_)
    {
        shot->Release();
        delete shot;
    }
    for (int i = 0; i < BLAST_ANIM_NUM; i++)
    {
        DeleteGraph(blastImgs_[i]);
    }
}

VECTOR BattleShip::GetPos(void) const
{
    return transform_.pos;
}

const Transform& BattleShip::GetTransform(void) const
{
    return transform_;
}

VECTOR BattleShip::GetDir(void) const
{
    return dir_;
}

double BattleShip::DegToRad(double deg)
{
    return deg * (DX_PI_F / 180.0f);
}

float BattleShip::DegToRadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

void BattleShip::Turn(double targetAngle, const VECTOR& axis)
{
    float rad = DegToRad(targetAngle);
    axisQ = Quaternion::AngleAxis(rad, axis);
    transform_.quaRot = transform_.quaRot.Mult(axisQ);
}

Quaternion BattleShip::GetSteeringRotation() const
{
    return Quaternion::Euler(0.0f, steeringAngle, 0.0f);
}

//Quaternion BattleShip::Conjugate(const Quaternion& q)
//{
//    return Quaternion(q.w, -q.x, -q.y, -q.z);
//}
//
//Quaternion BattleShip::RotateQuaternion(const Quaternion& q, float angle, const VECTOR& axis)
//{
//    float rad = DegToRad(angle);
//    Quaternion axisQ = Quaternion::AngleAxis(rad, axis);
//    return q.Mult(axisQ).Normalized();
//}

float BattleShip::GetSpeedFromLevel(SpeedLevel level)
{
    switch (level)
    {
    case SpeedLevel::STOP:
        return 0.0f;
    case SpeedLevel::QUARTER:
        return maxspeed_ * 0.25f;
    case SpeedLevel::HALF:
        return maxspeed_ * 0.5f;
    case SpeedLevel::THREE_QUARTER:
        return maxspeed_ * 0.75f;
    case SpeedLevel::MAX:
        return maxspeed_;
    case SpeedLevel::BACK:
        return -maxspeed_ * 0.25f;
    }
}

std::vector<PlayerShot*>& BattleShip::GetShots(void)
{
    return shots_;
}

double BattleShip::LerpDeg(double start, double end, double t)
{
    double ret;

    double diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0;
        ret = Lerp(start, end, t);
        if (ret >= 360.0)
        {
            ret -= 360.0;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0;
        ret = Lerp(start, end, t);
        if (ret < 0.0)
        {
            ret += 360.0;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;
}

double BattleShip::Lerp(double start, double end, double t)
{
    // ���`���
    if (t >= 1.0)
    {
        return end;
    }

    double ret = start;
    ret += t * (end - start);
    return ret;
}

VECTOR BattleShip::GetQua() const
{
    return TurretQua.ToEuler();
}

void BattleShip::AddCollider(Collider* collider)
{
    colliders_.push_back(collider);
}

void BattleShip::ClearCollider(void)
{
    colliders_.clear();
}

void BattleShip::TurretRot()
{
    //1�Ԏ�C
    MV1SetFrameDifColorScale(transform_.modelId, 1, GetColorF(0.1f, 0.1f, 1.0f, 1.0f));
    //�傫���@��]�@�ʒu
    auto mat = MV1GetFrameBaseLocalMatrix(transform_.modelId, 1);// �w��̃t���[���̏�����Ԃł̍��W�ϊ��s����擾����

    auto scale = MGetSize(mat);//�傫��

    auto matscl = MGetScale(scale);// �g��s����擾����

    auto turretRotMat = TurretQua.ToMatrix();

    auto pos = MGetTranslateElem(mat);//�s��̕��s�ړ��������擾����

    VECTOR TurretLocalPos = TurretQua.PosAxis({ 0.0f, -85.337f , 1170.922f });
    pos = VAdd(pos, TurretLocalPos);



    auto matPos = MGetTranslate(pos);// ���s�ړ��s����擾����

    MATRIX turretMat = MGetIdent();
    turretMat = MMult(turretMat, matscl);
    turretMat = MMult(mat, turretRotMat);
    turretMat = MMult(turretMat, matPos);


    MV1SetFrameUserLocalMatrix(transform_.modelId, 1, turretMat);

    //2�Ԏ�C
    MV1SetFrameDifColorScale(transform_.modelId, 4, GetColorF(0.1f, 0.1f, 1.0f, 1.0f));
    //�傫���@��]�@�ʒu
    auto matTwo = MV1GetFrameBaseLocalMatrix(transform_.modelId, 4);// �w��̃t���[���̏�����Ԃł̍��W�ϊ��s����擾����

    auto scaleTwo = MGetSize(matTwo);//�傫��

    auto matsclTwo = MGetScale(scaleTwo);// �g��s����擾����
    Quaternion TurretQuaTwo;
    auto turretRotMatTwo = TurretQua.ToMatrix();

    auto posTwo = MGetTranslateElem(matTwo);//�s��̕��s�ړ��������擾����

    VECTOR TurretLocalPosTwo = TurretQua.PosAxis({ 0.0f, -145.143f , 932.302f });
    posTwo = VAdd(posTwo, TurretLocalPosTwo);



    auto matPosTwo = MGetTranslate(posTwo);// ���s�ړ��s����擾����

    MATRIX turretMatTwo = MGetIdent();
    turretMatTwo = MMult(turretMatTwo, matsclTwo);
    turretMatTwo = MMult(matTwo, turretRotMatTwo);
    turretMatTwo = MMult(turretMatTwo, matPosTwo);


    MV1SetFrameUserLocalMatrix(transform_.modelId, 4, turretMatTwo);


    //3�Ԏ�C
    MV1SetFrameDifColorScale(transform_.modelId, 5, GetColorF(0.1f, 0.1f, 1.0f, 1.0f));
    //�傫���@��]�@�ʒu
    auto matThree = MV1GetFrameBaseLocalMatrix(transform_.modelId, 5);// �w��̃t���[���̏�����Ԃł̍��W�ϊ��s����擾����

    auto scaleThree = MGetSize(matThree);//�傫��

    auto matsclThree = MGetScale(scaleThree);// �g��s����擾����
    Quaternion TurretQuaThree;
    auto turretRotMatThree = BackTurretQua.ToMatrix();

    auto posThree = MGetTranslateElem(matThree);//�s��̕��s�ړ��������擾����

    VECTOR TurretLocalPosThree = BackTurretQua.PosAxis({ 0.0f, -101.851f , -450.882f });
    posThree = VAdd(posThree, TurretLocalPosThree);



    auto matPosThree = MGetTranslate(posThree);// ���s�ړ��s����擾����

    MATRIX turretMatThree = MGetIdent();
    turretMatThree = MMult(turretMatThree, matsclThree);
    turretMatThree = MMult(matThree, turretRotMatThree);
    turretMatThree = MMult(turretMatThree, matPosThree);

    MV1SetFrameUserLocalMatrix(transform_.modelId, 5, turretMatThree);



    //4�Ԏ�C
    MV1SetFrameDifColorScale(transform_.modelId, 6, GetColorF(0.1f, 0.1f, 1.0f, 1.0f));
    //�傫���@��]�@�ʒu
    auto matFour = MV1GetFrameBaseLocalMatrix(transform_.modelId, 6);// �w��̃t���[���̏�����Ԃł̍��W�ϊ��s����擾����

    auto scaleFour = MGetSize(matFour);//�傫��

    auto matsclFour = MGetScale(scaleFour);// �g��s����擾����
    Quaternion TurretQuaFour;
    auto turretRotMatFour = BackTurretQua.ToMatrix();

    auto posFour = MGetTranslateElem(matFour);//�s��̕��s�ړ��������擾����

    VECTOR TurretLocalPosFour = BackTurretQua.PosAxis({ 0.1f, -31.055f , -1230.0f });
    posFour = VAdd(posFour, TurretLocalPosFour);



    auto matPosFour = MGetTranslate(posFour);// ���s�ړ��s����擾����

    MATRIX turretMatFour = MGetIdent();
    turretMatFour = MMult(turretMatFour, matsclFour);
    turretMatFour = MMult(matFour, turretRotMatFour);
    turretMatFour = MMult(turretMatFour, matPosFour);

    MV1SetFrameUserLocalMatrix(transform_.modelId, 6, turretMatFour);

}

bool BattleShip::IsAlive(void) const
{
    return state_ == STATE::BATTLE;
}


void BattleShip::ProcessRot(void)
{
    auto mouse = GetMousePoint(&mouseX, &mouseY);
    int deltaX = mouseX - prevMouseX;
    int deltaY = mouseY - prevMouseY;
    prevMouseX = mouseX;
    prevMouseY = mouseY;


    if (CheckHitKey(KEY_INPUT_DOWN))
    {
        transform_.Update();
        auto rot = Quaternion::AngleAxis(
            DegToRadF(0.5f), { 1.0f,0.0f,0.0f });
        barrelQua = barrelQua.Mult(rot);
    }

    if (CheckHitKey(KEY_INPUT_UP))
    {
        transform_.Update();
        auto rot = Quaternion::AngleAxis(
            DegToRadF(-0.5f), { 1.0f,0.0f,0.0f });
        barrelQua = barrelQua.Mult(rot);
    }


    if (CheckHitKey(KEY_INPUT_LEFT))
    {
        transform_.Update();

        auto rot = Quaternion::AngleAxis(
            DegToRadF(-1.0f), { 0.0f,1.0f,0.0f });
        TurretQua = TurretQua.Mult(rot);
        BackTurretQua = BackTurretQua.Mult(rot.Inverse());


        //auto q = transform_.quaRot.Mult(transform_.quaRotLocal);
        //TurretQua = transform_.quaRot.Mult(rot);
    }

    if (CheckHitKey(KEY_INPUT_RIGHT))
    {
        transform_.Update();

        auto rot = Quaternion::AngleAxis(
            DegToRadF(1.0f), { 0.0f,1.0f,0.0f });
        TurretQua = TurretQua.Mult(rot);
        BackTurretQua = BackTurretQua.Mult(rot.Inverse());

        // auto q = transform_.quaRot.Mult(transform_.quaRotLocal);
         //TurretQua = transform_.quaRot.Mult(rot);
    }
    //currentSteeringRotation = Quaternion::Slerp(
    //    transform_.quaRotLocal
    //    , axisQ,
    //    SPEED_ROT_Y * deltaTime);

}

void BattleShip::ChangeState(STATE state)
{
    state_ = state;
    switch (state)
    {
    case BattleShip::STATE::BATTLE:
        break;
    case BattleShip::STATE::DESTROY:
        break;
    }



}

void BattleShip::ProcessShot(void)
{
    stepShotDelay_ -= SceneManager::GetInstance().GetDeltaTime();
    if (stepShotDelay_ < 0.0f)
    {
        stepShotDelay_ = 0.0f;
    }
    if (CheckHitKey(KEY_INPUT_Z) && stepShotDelay_ <= 0.0f)
    {
        stepShotDelay_ = TIME_DELAY_SHOT;

        CreateShot();
      //  Effect();

    }


}

void BattleShip::CreateShot(void)
{
    bool isCreate = false;
    //�C�g�̉�]�s��
//�C�g�̉�]�s��

    //�ꉞ�Ƃ��Ƃ�    { 0.0f, -85.337f , 1170.922f }
    
    //1
    auto matF = MV1GetFrameLocalWorldMatrix(transform_.modelId, 1);
    auto rotF = MGetRotElem(matF);
    auto rotQuaF = Quaternion::GetRotation(rotF);//�s�񂩂�N�H�[�^�j�I���Ɏ���
    auto framePosF = MV1GetFramePosition(transform_.modelId, 1);	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
    VECTOR offsetF = { 0.0f,0.0f,0.0f };
    auto slidePosF = VAdd(framePosF, rotQuaF.PosAxis(offsetF));
    //2
    auto matS = MV1GetFrameLocalWorldMatrix(transform_.modelId, 4);
    auto rotS = MGetRotElem(matS);
    auto rotQuaS = Quaternion::GetRotation(rotS);//�s�񂩂�N�H�[�^�j�I���Ɏ���
    auto framePosS = MV1GetFramePosition(transform_.modelId, 4);	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�

    //3
    auto matT = MV1GetFrameLocalWorldMatrix(transform_.modelId, 5);
    auto rotT = MGetRotElem(matT);
    auto rotQuaT = Quaternion::GetRotation(rotT);//�s�񂩂�N�H�[�^�j�I���Ɏ���
    auto framePosT = MV1GetFramePosition(transform_.modelId, 5);	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�

    //4
    auto matFor = MV1GetFrameLocalWorldMatrix(transform_.modelId, 6);
    auto rotFor = MGetRotElem(matFor);
    auto rotQuaFor = Quaternion::GetRotation(rotFor);//�s�񂩂�N�H�[�^�j�I���Ɏ���
    auto framePosFor = MV1GetFramePosition(transform_.modelId, 6);	// �w��̃t���[���̃��[�J�����W���烏�[���h���W�ɕϊ�����s��𓾂�
   // VECTOR offsetR = { 80.0f,0.0f,0.0f };
    //auto slidePosL = VAdd(framePosR, rotQuaR.PosAxis({0.0f,80.0f,0.0f}));
    for (auto s : shots_)
    {
        if (!s->IsAlive())
        {
            auto newShot = new PlayerShot(&transform_);
            newShot->CreateShot(slidePosF, TurretQua.GetForward());
            isCreate = true;
        }
    }
    if (!isCreate)
    {
        transform_.Update();
        auto newShot = new PlayerShot(&transform_);
        newShot->CreateShot(slidePosF, TurretQua.GetForward());

        shots_.emplace_back(newShot);
    }


}

//void BattleShip::Effect()
//{
//    int playHandle = PlayEffekseer3DEffect(Laser);
//    //�ʒu��ݒ�
//    VECTOR pos = transform_.pos;
//    SetPosPlayingEffekseer3DEffect(playHandle, pos.x, pos.y, pos.z);
//    //�傫����ݒ�
//    SetScalePlayingEffekseer3DEffect(playHandle, 5.0f, 5.0f, 5.0f);
//}



//PlayerShot* BattleShip::GetValidShot(void)
//{
//    size_t size = shots_.size();
//    for (int i = 0; i < size; i++)
//    {
//        if (!shots_[i]->IsAlive())
//        {
//            return shots_[i];
//        }
//    }
//    PlayerShot* shot = new PlayerShot(shotModelId_, blastImgs_, BLAST_ANIM_NUM);
//    shots_.push_back(shot);
//    return shot;
//}


