#pragma once
#include<chrono>
#include<memory>
#include "Camera.h"
#include "../Scene/BaseScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/Transition/Fader.h"

//
#define lpSceneManager SceneManager::GetInstance()

using namespace std;

class SceneManager
{
public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME
	};


	// 静的インスタンス
	static SceneManager* mInstance;

	//動的
	//static SceneMng* Create(void) 
	//{
	//	if (s_Instance == nullptr)				//	実体がないときにコンストラクタなどを呼べるようにする
	//	{
	//		s_Instance = new SceneMng();
	//	}
	//	return s_Instance;
	//}
	//static void Destroy(void)
	//{
	//	delete s_Instance;
	//	s_Instance = nullptr;					//	実体だけでなくアドレス情報も消去する
	//}
		// インスタンスの生成
	static void CreateInstance(void);
	// インスタンスの取得
	static SceneManager& GetInstance(void);
	void Run(void);
	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);
	void DrawShader(void);
	void Release(void);
	// 状態遷移
	void ChangeScene(SCENE_ID nextId, bool isFading);
	// シーンIDの取得
	SCENE_ID GetmSceneID(void);

	// デルタタイムの取得
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
	// デルタタイム
	std::chrono::system_clock::time_point mPreTime;
	float mDeltaTime;

	void ResetDeltaTime(void);
	// シーン遷移
	void DoChangeScene(void);
	// フェード
	void Fade(void);

	void MyDrawGraph(int x, int y, int width, int height);

	GameScene* game_;
	int shadIdP;
	int shadIdV;
	int shadIdC;
	int test;
	int screen;
	int offscreen;
	int fuck;
	int normal;
	int cbufferH;
	int Prevscreen;
	float* params;
	int id_time;
	int id_cut;
	array<VERTEX2DSHADER, 4> verts;
};

