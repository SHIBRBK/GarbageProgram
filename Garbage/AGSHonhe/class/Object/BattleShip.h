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
    //状態
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

    //衝突判定；球体の半径
    static constexpr float COLLISION_RADIUS = 150.0f;
    static constexpr float SCALE = 0.5f;
    static constexpr float YAW = 135.0f;//Y軸周り
    static constexpr float PITCH = 0.0f;//X軸周り
    static constexpr float ROLL = 0.0f;//Z軸回り
    static constexpr VECTOR FRONT = { 0.0f, 0.0f, 1.0f };
    static constexpr float SPEED_MOVE = 10.0f;
    static constexpr float SPEED_ROT_Y = DX_PI_F / 180.0f * 15.0f;
    static constexpr int BLAST_ANIM_NUM = 16;
    static constexpr int BLAST_SIZE_X = 32;
    static constexpr int BLAST_SIZE_Y = 32;
    static constexpr float TIME_DELAY_SHOT = 0.6f;
    //被ダメージエフェクト
    static constexpr float TIME_DAMAGED_EFFECT = 2.0f;
    //回転完了までの時間
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
    //衝突判定に用いられるコライダ制御
    void AddCollider(Collider* collider);
    void ClearCollider(void);
    void TurretRot();
    //生存判定
    bool IsAlive(void) const;
protected:

private:
    int modelId_;
    // 弾のモデルID
    int shotModelId_;
    bool isMovingForward; // 船が前進しているかどうかのフラグ
    bool isMovingBack;
    VECTOR pos_;
    VECTOR rot_;
    VECTOR scale_;
    float speed_;
    float maxspeed_;
    float lastFrameTime_;
    float angles_;
    float rotationAngle;
    float maxRudderAngle;  // 最大舵の角度（度）
    float rudderAngle;  // 現在の舵の角度（度）
    float rudderSpeed;  // 舵の回転速度（度/秒）
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
    // ショット(ポインタ)
    std::vector<PlayerShot*> shots_;
    // 発射操作
    void ProcessShot(void);
    void CreateShot(void);
    //void Effect();
    void ProcessRot(void);
    // 有効な弾を取得する
    
    // 弾発射後の硬直時間計算用
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
    //続きは、チェンジステートから
    void ChangeState(STATE state);
    //被ダメージエフェクトの時間計測用
    float mStepDamaged;
};

