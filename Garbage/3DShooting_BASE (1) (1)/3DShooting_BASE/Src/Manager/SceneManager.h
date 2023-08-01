#pragma once
#include <chrono>
class SceneBase;
class Fader;
class Camera;
class ResourceManager;

class SceneManager
{

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		EVENT,
		BATTLE
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId, bool isFading);

	// シーンIDの取得
	SCENE_ID GetmSceneID(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	// カメラの取得
	Camera* GetCamera(void) const;

private:

	// 静的インスタンス
	static SceneManager* mInstance;

	SCENE_ID mSceneID;
	SCENE_ID mWaitSceneID;

	SceneBase* mScene;
	Fader* mFader;
	Camera* mCamera;

	bool mIsSceneChanging;

	// デルタタイム
	std::chrono::system_clock::time_point mPreTime;
	float mDeltaTime;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);
	~SceneManager(void);

	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(void);

	// フェード
	void Fade(void);

};