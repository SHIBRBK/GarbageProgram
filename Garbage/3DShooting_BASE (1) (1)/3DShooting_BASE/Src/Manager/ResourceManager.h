#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		START_LOGO,
		END_LOGO,
		ALERT_MSG,
		LIGHT,
		SKYDOME,
		PLAYER_SHIP,
		LEADER_SHIP,
		BOSS_SHIP,
		DUNGEON,
		SHIP_EXPLOSION,
		SHOT_EXPLOSION,
		ALPHABET,
		SPEECH_BALLOON,
		SHOT_MODEL,
		TURRET_STAND,
		TURRET_GUN,
		ROCK01,
		ROCK02,
		JET,
		BOOST,
		EXPLOSION,
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* mInstance;

	// リソース管理の対象
	std::map<SRC, Resource> mResourcesMap;

	// 読み込み済みリソース
	std::map<SRC, Resource*> mLoadedMap;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void);

	// 内部ロード
	Resource* _Load(SRC src);

};

