#pragma once
#include<chrono>
#include<memory>
#include "Camera.h"
#include "../Scene/BaseScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/Transition/Fader.h"

//
#define lpSceneManager SceneManager::GetInstance()


class SceneManager
{
public:

	// �V�[���Ǘ��p
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME
	};


	// �ÓI�C���X�^���X
	static SceneManager* mInstance;

	//���I
	//static SceneMng* Create(void) 
	//{
	//	if (s_Instance == nullptr)				//	���̂��Ȃ��Ƃ��ɃR���X�g���N�^�Ȃǂ��Ăׂ�悤�ɂ���
	//	{
	//		s_Instance = new SceneMng();
	//	}
	//	return s_Instance;
	//}
	//static void Destroy(void)
	//{
	//	delete s_Instance;
	//	s_Instance = nullptr;					//	���̂����łȂ��A�h���X������������
	//}
		// �C���X�^���X�̐���
	static void CreateInstance(void);
	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);
	void Run(void);
	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);
	void Release(void);
	// ��ԑJ��
	void ChangeScene(SCENE_ID nextId, bool isFading);
	// �V�[��ID�̎擾
	SCENE_ID GetmSceneID(void);

	// �f���^�^�C���̎擾
	float GetDeltaTime(void) const;

	Camera* GetCamera()const ;
private:
	SCENE_ID sceneID_;
	SCENE_ID waitsceneID_;
	SceneManager();
	~SceneManager();
	int selectedShipIndex;
	int selectedStageIndex;
	uniqueScene scene_;
	uniqueCamera camera_;
	uniqueFade mFader;
	bool mIsSceneChanging;
	// �f���^�^�C��
	std::chrono::system_clock::time_point mPreTime;
	float mDeltaTime;

	void ResetDeltaTime(void);
	// �V�[���J��
	void DoChangeScene(void);
	// �t�F�[�h
	void Fade(void);

	void MyDrawGraph(int x, int y, int width, int height);

	GameScene* game_;
	int shadIdC;
	int shadIdP;
	int shadIdV;
	int fuck;
	int normal;
	int offscreen;
	int MoHandle;
	VECTOR pos;
	int Prevscreen;
	int screen;
	int cbufferH;
	float* params;
	int id_time;
	int id_cut;
};

