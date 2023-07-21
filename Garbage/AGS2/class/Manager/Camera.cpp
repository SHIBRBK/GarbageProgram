#include "../Common/DxCushion.h"
#include "../Object/PlayerBase.h"
#include "../Common/Transform.h"
#include "../../Resource.h"
#include "Camera.h"
#include<array>

Camera* Camera::instance_ = nullptr;

using namespace std;
void Camera::MyDrawGraph(int x, int y, int width, int height)
{
    array<VERTEX2DSHADER, 4> verts;
    for (auto& v : verts) {
        v.rhw = 1.0f;
        v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);
        v.u = 0.0f;
        v.v = 0.0f;
        v.su = 0.0f;
        v.sv = 0.0f;
        v.pos.z = 0.0f;
        v.spc = GetColorU8(0, 0, 0, 0);
    }
    verts[0].pos.x = x;
    verts[0].pos.y = y;
    verts[1].pos.x = x + width;
    verts[1].pos.y = y;
    verts[1].u = 1.0f;
    verts[2].pos.x = x;
    verts[2].pos.y = y + height;
    verts[2].v = 1.0f;
    verts[3].pos.x = x + width;
    verts[3].pos.y = y + height;
    verts[3].u = 1.0f;
    verts[3].v = 1.0f;
   //DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
    DrawPolygon2DToShader(verts.data(), verts.size());
}


void Camera::CreateInstance(void)
{
    if (instance_ == nullptr)
    {
        instance_ = new Camera();
    }
    instance_->Init();
}

Camera& Camera::GetInstance(void)
{
    if (instance_ == nullptr)
    {
        Camera::CreateInstance();
    }
    return *instance_;
}

Camera::Camera(void)
{
    rotationAngle = VECTOR();
    pos_ = VECTOR();
    targetPos_= VECTOR();
    angles_ = VECTOR();
    cameraUp_= VECTOR();
    mSyncTransform = nullptr;

}

Camera::~Camera(void)
{
   
}

void Camera::Init()
{
    ChangeMode(MODE::FREE);
     
}

void Camera::SetDefault(void)
{
    pos_ = DEFAULT_CAMERA_POS;
    targetPos_ = { 0.0f,0.0f,0.0f };
    rotationAngle.x = DegToRad(10.0f);
    rotationAngle.y = 0.0f;
    rotationAngle.z = 0.0f;
    cameraUp_ = { 0.0f,1.0f,0.0f };
    quaRot_ = Quaternion();

    shadIdP = Resource::MyLoadPixelShader(Resource::PATH_SHADER + "Post.pso");
    RenderT = MakeScreen(1024, 640);
    i = MakeScreen(1024, 640);
    cbufferH = CreateShaderConstantBuffer(sizeof(float) * 4);
}

void Camera::SetBeforeDraw()
{
    DxCushion::MySetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);
    //(this->*p_Update)();

    switch (mode_)
    {
    case Camera::MODE::FREE:
        SetBeforeDrawFree();
        break;
    case Camera::MODE::FOLLOW:
        SetBeforeDrawFollow();
        break;
    case Camera::MODE::ZOOM:
        SetBeforeDrawZoom();
        break;
    }




   SetCameraPositionAndTargetAndUpVec(
    pos_,
    targetPos_,
    cameraUp_);
}



void Camera::SetBeforeDrawZoom()
{
}

void Camera::SetBeforeDrawFree()
{
}

void Camera::Update()
{

}

void Camera::Draw()
{
    constexpr int screenWidth = 0;
    constexpr int screenHeight = 1;
    constexpr int time = 2;
 
   
    params = static_cast<float*>(GetBufferShaderConstantBuffer(cbufferH));
    params[screenWidth] = 1024.0f;
    params[screenHeight] = 640.0f;
    params[time] += 0.01f;

    SetDrawScreen(i);
    ClearDrawScreen();
   
    UpdateShaderConstantBuffer(cbufferH);
	SetShaderConstantBuffer(cbufferH, DX_SHADERTYPE_PIXEL, 6);
   SetUseTextureToShader(4, RenderT);
   
    SetUsePixelShader(shadIdP);

    array<VERTEX2DSHADER, 4> verts;
    for (auto& v : verts) {
        v.rhw = 1.0f;
        v.dif = GetColorU8(0xff, 0xff, 0xff, 0xff);
        v.u = 0.0f;
        v.v = 0.0f;
        v.su = 0.0f;
        v.sv = 0.0f;
        v.pos.z = 0.0f;
        v.spc = GetColorU8(0, 0, 0, 0);
    }
    verts[0].pos.x = 0;
    verts[0].pos.y = 0;
    verts[1].pos.x = 0 + params[screenWidth];
    verts[1].pos.y = 0;
    verts[1].u = 1.0f;
    verts[2].pos.x = 0;
    verts[2].pos.y = 0 + params[screenHeight];
    verts[2].v = 1.0f;
    verts[3].pos.x = 0 + params[screenWidth];
    verts[3].pos.y = 0 + params[screenHeight];
    verts[3].u = 1.0f;
    verts[3].v = 1.0f;
    DrawPrimitive2DToShader(verts.data(), verts.size(), DX_PRIMTYPE_TRIANGLESTRIP);
     SetUsePixelShader(-1);

    SetDrawScreen(DX_SCREEN_BACK);
    ClearDrawScreen();
    // �J�����ݒ�
    SetBeforeDraw();

    


        DrawFormatString(0, 210, 0xffffffff, "FPS=%f", GetFPS());

}

void Camera::Release()
{
    
}

void Camera::ProcessRot(void)
{
}

VECTOR Camera::GetPos(void) const
{
    return pos_;
}

VECTOR Camera::GetAngles(void) const
{
    return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
    return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
    return quaRot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
    return quaRotOutX_;
}

VECTOR Camera::GetDir(void) const
{
    return VNorm(VSub(targetPos_, pos_));
}

void Camera::UpdateMouseState()
{
    DxCushion::MyGetMousePoint(&mx,&my);

    if (GetMouseInput() && MOUSE_INPUT_RIGHT)
    {
        isRightClock = true;
    }
    else
    {
        isRightClock = false;
    }
}

//void Camera::ChangeMode(int cameraNo)
//{
//    state_ = cameraNo;
//    switch (state_)
//    {
//    case 0:
//        p_Update = &Camera::SetBeforeDrawFollow;
//        break;
//    case 1:
//        p_Update = &Camera::SetBeforeDrawZoom;
//        break;
//    case 2:
//        p_Update = &Camera::SetBeforeDrawFree;
//        break;
//    }
//}

void Camera::ChangeMode(MODE mode)
{
    SetDefault();
    mode_ = mode;
    switch (mode_)
    {
    case Camera::MODE::FOLLOW:
        break;
    case Camera::MODE::ZOOM:
        break;
    case Camera::MODE::FREE:
        break;
    }
}

//void Camera::SwitchCameraMode()
//{
//    // �t���[�����Ƃ̏���
//    if (CheckHitKey(KEY_INPUT_LSHIFT))
//    {
//        // LShift�L�[�������ꂽ�^�C�~���O�ŃJ�������[�h��؂�ւ���
//        if (mode_ == MODE::FOLLOW)
//        {
//            mode_ = MODE::ZOOM;
//        }
//        else
//        {
//            mode_ = MODE::FOLLOW;
//        }
//    }
//
//    // �J�������[�h�ɉ������������s��
//    if (mode_ == MODE::FOLLOW)
//    {
//        // TPS���_���[�h�̏���
//    }
//    else if (mode_ == MODE::ZOOM)
//    {
//        // �o�ዾ���_���[�h�̏���
//    }
//}

float Camera::DegToRad(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

VECTOR Camera::Turn(double targetAngle, const VECTOR& axis)
{
    float rad = DegToRad(targetAngle);
    Quaternion  axisQ = Quaternion::AngleAxis(rad, axis);
    quaRot_ = quaRot_.Mult(axisQ);

   return targetPos_;
}

void Camera::SetPlayerPos(const Transform* target)
{
    target_ = target;
}

void Camera::SetTransform(Transform* transform)
{
    mSyncTransform = transform;
}



void Camera::SetBeforeDrawFollow()
{
    GetMousePoint(&mouseX, &mouseY);
    int deltaX = mouseX - prevMouseX;
    int deltaY = mouseY - prevMouseY;
    prevMouseX = mouseX;
    prevMouseY = mouseY;


   
    if (deltaX > 0)
    {
        rotationAngle.y += DegToRad(1.0f);

    }
    else if(deltaX < 0)
    {
        rotationAngle.y -= DegToRad(1.0f);

    }
    else
    {
        rotationAngle.y += 0.0f;
    }


    if (deltaY > 0)
    {
        //3D�A�N�V�����ɂ������ˁB������������
        rotationAngle.x -= DegToRad(0.5f);

    }
    else if (deltaY < 0)
    {
        rotationAngle.x += DegToRad(0.5f);

    }
    else
    {
        rotationAngle.x += 0.0f;
    }
    deltaX = 0;
    deltaY = 0;
    VECTOR targetPos = target_->pos; //�v���C���[��   ������̈ʒu�Ƃ�����

    Quaternion anotherQua = target_->quaRot;

    VECTOR axisX = { 1.0f, 0.0f, 0.0f };  // X����]�Ȃ̂�(1, 0, 0)
    VECTOR axisY = { 0.0f, 1.0f, 0.0f };  // Y����]�Ȃ̂�(0, 1, 0)

    //�J������]�p�̃N�H�[�^��AngleAxis�ɑ΂��ă}�E�X�̍��W�̍��������Ƃɉ��Z���s���B
    //���[��ƁA�������Z�o���ꂽ�N�H�[�^�j�I����������
    quaRotOutX_ = anotherQua.Mult(Quaternion::AngleAxis(rotationAngle.y, axisY));
    quaRot_ = quaRotOutX_.Mult(Quaternion::AngleAxis(rotationAngle.x, axisX));
    //cameraRotQua = cameraRotQua.Mult(cameraRotQua);

    //�Z�o���ꂽ�N�H�[�^�j�I��������
    VECTOR localPos;
    localPos = quaRotOutX_.PosAxis(RELATIVE_TARGET_POS);//�����_�̑��΃x�N�g��
    targetPos_ = VAdd(targetPos, localPos);
    localPos = quaRot_.PosAxis(RELATIVE_CAMERA_POS_FOLLOW);//�J�����̏������΃x�N�g���ƃv���C���[
   //���Ƃ͒����_����邾��   �v���C���[�̍��W����J�����̐������B�ŁA�Ō�́A�����_�̋����B
    pos_ = VAdd(targetPos, localPos);
    cameraUp_ = anotherQua.GetUp();

    //----------------��T�܂ł̒Ǐ]�J����-----------------------
    //VECTOR targetPos = target_->pos;
    //VECTOR cameraPos = VTransform(RELATIVE_CAMERA_POS_FOLLOW, target_->matRot);
    //// �J�����̒����_���v���C���[�̈ʒu�ɐݒ肷��i�����_�̓v���C���[�̏����O���ɐݒ肷�邱�ƂŒǏ]�����o���j

    //VECTOR CtargetPos = VTransform(RELATIVE_TARGET_POS, target_->matRot);
    //targetPos_ = targetPos;
    //pos_ = VAdd(targetPos, cameraPos);
}