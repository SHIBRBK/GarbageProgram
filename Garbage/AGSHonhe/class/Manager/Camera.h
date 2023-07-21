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
	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 1000.0f, -500.0f };


	// FOLLOW : �J�����ʒu(�Ǐ]�ΏۂƂ̑��΍��W)
	static constexpr VECTOR RELATIVE_CAMERA_POS_FOLLOW = { 0.0f, 300.0f, -1000.0f };

	// FOLLOW : �����_(�Ǐ]�ΏۂƂ̑��΍��W)
	static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, 0.0f, 500.0f };


	// �Ǐ]�ΏۂƂ̒������W
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
	// �J�����������ʒu�ɖ߂�
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
	// �J��������
	VECTOR GetDir(void) const;
	void UpdateMouseState();

	//void ChangeMode(int cameraNo);
	void ChangeMode(MODE mode);
	float DegToRad(float deg);
	VECTOR Turn(double targetAngle, const VECTOR& axis);
	void SetPlayerPos(const Transform* target);
	void SetTransform(Transform* transform);
private:
	// �V���O���g���p�C���X�^���X

	static Camera* instance_;

	// �J�������Ǐ]�ΏۂƂ���Transform
	Transform* mSyncTransform;
	int mx = 0;
	int my = 0;
	bool isRightClock = false;

	//�J�����̈ʒu
	MODE mode_;

	//�J�����̈ʒu
	//�Ǐ]�Ώ�
	const Transform* target_;
	VECTOR pos_;
	VECTOR angles_;
	VECTOR targetPos_;
	VECTOR beforePos_;
	VECTOR cameraUp_;
	// �J�����p�x
	Quaternion quaRotOutX_;
	Quaternion quaRot_;
	void(Camera::*p_Update)(void);
	int state_;
	bool checkShiftKey=true;
	uniqueBB bb;
	// Transform�̈ʒu�Ɠ��������
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

