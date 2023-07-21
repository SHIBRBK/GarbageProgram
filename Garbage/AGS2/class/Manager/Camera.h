#pragma once
#include<memory>
#include<DxLib.h>
#include "../Object/BattleShip.h"
#include "../Common/Quaternion.h"

class Camera;
using uniqueCamera = std::unique_ptr<Camera>;
class Transform;


class Camera
{
public:
	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 1000.0f, -500.0f };


	// FOLLOW : カメラ位置(追従対象との相対座標)
	static constexpr VECTOR RELATIVE_CAMERA_POS_FOLLOW = { 0.0f, 300.0f, -1000.0f };

	// FOLLOW : 注視点(追従対象との相対座標)
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, 0.0f, 500.0f };


	// 追従対象との調整座標
	static constexpr VECTOR RELATIVE_TRANSFORM_POS = { 0.0f, 20.0f, 0.0f };

	enum class MODE
	{
		NONE,
		FOLLOW,
		ZOOM,
		FREE
	};
	void CreateInstance(void);
	Camera& GetInstance(void);
	Camera(void);
	~Camera(void);
	void Init();
	// カメラを初期位置に戻す
	void SetDefault(void);
	void SetBeforeDraw();
	void SetBeforeDrawFollow();
	void SetBeforeDrawZoom();
	void SetBeforeDrawFree();
	void Update();
	void Draw();
	void Release();
	void MyDrawGraph(int x, int y, int width, int height);


	void ProcessRot(void);
	VECTOR GetPos(void) const;
	VECTOR GetAngles(void) const;
	VECTOR GetTargetPos(void) const;
	Quaternion GetQuaRot(void) const;
	Quaternion GetQuaRotOutX(void) const;
	// カメラ方向
	VECTOR GetDir(void) const;
	void UpdateMouseState();

	//void ChangeMode(int cameraNo);
	void ChangeMode(MODE mode);
	float DegToRad(float deg);
	VECTOR Turn(double targetAngle, const VECTOR& axis);
	void SetPlayerPos(const Transform* target);
	void SetTransform(Transform* transform);
private:
	// シングルトン用インスタンス

	static Camera* instance_;

	// カメラが追従対象とするTransform
	Transform* mSyncTransform;
	int mx = 0;
	int my = 0;
	bool isRightClock = false;

	//カメラの位置
	MODE mode_;

	//カメラの位置
	//追従対象
	const Transform* target_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR targetPos_;
	VECTOR beforePos_;
	VECTOR cameraUp_;
	// カメラ角度
	Quaternion quaRotOutX_;
	Quaternion quaRot_;
	void(Camera::*p_Update)(void);
	int state_;
	bool checkShiftKey=true;
	uniqueBB bb;
	// Transformの位置と同期を取る
	//void SyncTransform(void);
	int mouseX = 0;
	int mouseY = 0;
	int prevMouseX = 0;
	int prevMouseY = 0;
	VECTOR rotationAngle;
	int Prevscreen = 0;
	int screen = 0;
	int cbufferH;
	float* params;
	int id_time;
	int shadIdC = 0;
	int shadIdP = 0;
	int shadIdV = 0;
	int fuck = 0;
	int normal = 0;
	int RenderT = 0;
	int i = 0;
	int MoHandle = 0;
	int cam=0;
};

