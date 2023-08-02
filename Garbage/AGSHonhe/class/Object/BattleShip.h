#pragma once
#include <memory>
#include <vector>
#include <DxLib.h>
#include "PlayerBase.h"
#include "PlayerShot.h"
#include "../Common/Quaternion.h"
#include "../Common/Transform.h"
class BattleShip;
using uniqueBB = std::unique_ptr<BattleShip>;
class PlayerShot;
class Collider;
class PlayerShot;

class BattleShip :
    public PlayerBase
{
public:
    //���
    enum class STATE
    {
        NONE,
        BATTLE,
        DESTROY,
        END
    };

    enum class SpeedLevel
    {
        BACK = -1,
        STOP,
        QUARTER,
        HALF,
        THREE_QUARTER,
        MAX,
    };

    //�Փ˔���G���̂̔��a
    static constexpr float COLLISION_RADIUS = 150.0f;
    static constexpr float SCALE = 0.5f;
    static constexpr float YAW = 135.0f;//Y������
    static constexpr float PITCH = 0.0f;//X������
    static constexpr float ROLL = 0.0f;//Z�����
    static constexpr VECTOR FRONT = { 0.0f, 0.0f, 1.0f };
    static constexpr float SPEED_MOVE = 10.0f;
    static constexpr float SPEED_ROT_Y = DX_PI_F / 180.0f * 15.0f;
    static constexpr int BLAST_ANIM_NUM = 16;
    static constexpr int BLAST_SIZE_X = 32;
    static constexpr int BLAST_SIZE_Y = 32;
    static constexpr float TIME_DELAY_SHOT = 0.6f;
    //��_���[�W�G�t�F�N�g
    static constexpr float TIME_DAMAGED_EFFECT = 2.0f;
    //��]�����܂ł̎���
    static constexpr float TIME_ROT = 2.0f;
    BattleShip();
    ~BattleShip()override;
    bool Init()override;
    void Update()override;
    void UpdateAlive();
    void UpdateDead();

    void Draw()override;
    void DrawAlive();
    void DrawDead();
    void Damage();
    void Release()override;
    VECTOR GetPos(void)const override;
    const Transform& GetTransform(void) const;
    VECTOR GetDir(void)const;
    double DegToRad(double deg);
    float DegToRadF(float deg);
    void Turn(double targetAngle, const VECTOR& axis);
    Quaternion GetSteeringRotation() const;
    //Quaternion Conjugate(const Quaternion& q);
   // Quaternion RotateQuaternion(const Quaternion& q, float angle, const VECTOR& axis);
    float GetSpeedFromLevel(SpeedLevel level);
    std::vector<PlayerShot*>&GetShots(void);
    double LerpDeg(double start, double end, double t);
    double Lerp(double start, double end, double t);
    VECTOR GetQua() const;
    //�Փ˔���ɗp������R���C�_����
    void AddCollider(Collider* collider);
    void ClearCollider(void);
    void TurretRot();
    //��������
    bool IsAlive(void) const;
protected:

private:
    int modelId_;
    // �e�̃��f��ID
    int shotModelId_;
    bool isMovingForward; // �D���O�i���Ă��邩�ǂ����̃t���O
    bool isMovingBack;
    VECTOR pos_;
    VECTOR rot_;
    VECTOR scale_;
    float speed_;
    float maxspeed_;
    float lastFrameTime_;
    float angles_;
    float rotationAngle;
    float maxRudderAngle;  // �ő�ǂ̊p�x�i�x�j
    float rudderAngle;  // ���݂̑ǂ̊p�x�i�x�j
    float rudderSpeed;  // �ǂ̉�]���x�i�x/�b�j
    float baseRotation;
    float steeringAngle;
    float rotationSpeed;
    float currentSpeed;
    Quaternion quaRot_;
    Quaternion axisQ;
    VECTOR dir_;
    MATRIX TmatRot;
    MATRIX matRot;
    MATRIX matScl;
    MATRIX matPos;
    SpeedLevel currentSpeedLevel;
    Transform transform_;
    STATE state_;
    // �V���b�g(�|�C���^)
    std::vector<PlayerShot*> shots_;
    // ���ˑ���
    void ProcessShot(void);
    void CreateShot(void);
    //void Effect();
    void ProcessRot(void);
    // �L���Ȓe���擾����
    
    // �e���ˌ�̍d�����Ԍv�Z�p
    float stepShotDelay_;
    int blastImgs_[BLAST_ANIM_NUM];

    Quaternion TurretQua;
    Quaternion barrelQua;
    Quaternion BackTurretQua;
    std::vector<Collider*> colliders_;

    int mouseX;
    int mouseY;
    int prevMouseX;
    int prevMouseY;
    int hp_;
    //�����́A�`�F���W�X�e�[�g����
    void ChangeState(STATE state);
    //��_���[�W�G�t�F�N�g�̎��Ԍv���p
    float mStepDamaged;
};

