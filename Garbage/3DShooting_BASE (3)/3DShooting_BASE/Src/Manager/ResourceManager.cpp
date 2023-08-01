#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::mInstance = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (mInstance == nullptr)
	{
		mInstance = new ResourceManager();
	}
	mInstance->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *mInstance;
}

void ResourceManager::Init(void)
{

	Resource res;

	// スタートロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "StartLogo.png");
	mResourcesMap.emplace(SRC::START_LOGO, res);

	// エンドロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "EndLogo.png");
	mResourcesMap.emplace(SRC::END_LOGO, res);

	// アラート
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Alert.png");
	mResourcesMap.emplace(SRC::ALERT_MSG, res);

	// パーティクルエフェクト用
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Light.png");
	mResourcesMap.emplace(SRC::LIGHT, res);

	// スカイドーム
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	mResourcesMap.emplace(SRC::SKYDOME, res);

	// プレイヤー機
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "PlayerShip/PlayerShip.mv1");
	mResourcesMap.emplace(SRC::PLAYER_SHIP, res);

	// リーダー機
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "LeaderShip/LeaderShip.mv1");
	mResourcesMap.emplace(SRC::LEADER_SHIP, res);

	// ボス戦艦
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "BossShip/BossShip.mv1");
	mResourcesMap.emplace(SRC::BOSS_SHIP, res);

	// ダンジョン
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Rock/Dungeon.mv1");
	mResourcesMap.emplace(SRC::DUNGEON, res);

	// 機体の爆発エフェクト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "ShipExplosion.png", 7, 1, 120, 120);
	mResourcesMap.emplace(SRC::SHIP_EXPLOSION, res);

	// 弾の爆発エフェクト
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "ShotExplosion.png", 7, 1, 120, 120);
	mResourcesMap.emplace(SRC::SHOT_EXPLOSION, res);

	// 文字画像
	res = Resource(Resource::TYPE::IMGS, Application::PATH_IMAGE + "Alphabet.png", 10, 6, 32, 32);
	mResourcesMap.emplace(SRC::ALPHABET, res);

	// 吹き出し
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SpeechBalloon.png");
	mResourcesMap.emplace(SRC::SPEECH_BALLOON, res);

	// 弾
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Shot/Shot.mv1");
	mResourcesMap.emplace(SRC::SHOT_MODEL, res);

	// 砲台
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "BossShip/Turret.mv1");
	mResourcesMap.emplace(SRC::TURRET_STAND, res);

	// 砲身
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "BossShip/Turret_Gun.mv1");
	mResourcesMap.emplace(SRC::TURRET_GUN, res);

	// 背景岩１
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Rock/Rock21.mv1");
	mResourcesMap.emplace(SRC::ROCK01, res);

	// 背景岩２
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Rock/Rock23.mv1");
	mResourcesMap.emplace(SRC::ROCK02, res);

	// 自機噴射
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Jet.efkefc");
	mResourcesMap.emplace(SRC::JET, res);

	// 自機ブースト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Boost.efkefc");
	mResourcesMap.emplace(SRC::BOOST, res);

	// 自機ブースト
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "Explosion.efkefc");
	mResourcesMap.emplace(SRC::EXPLOSION, res);


}

void ResourceManager::Release(void)
{
	for (auto& p : mLoadedMap)
	{
		p.second->Release();
		delete p.second;
	}

	mLoadedMap.clear();
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->mHandleId);
	res->mDuplicateModelIds.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
	delete mInstance;
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = mLoadedMap.find(src);
	if (lPair != mLoadedMap.end())
	{
		return lPair->second;
	}

	const auto& rPair = mResourcesMap.find(src);
	if (rPair == mResourcesMap.end())
	{
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	mLoadedMap.emplace(src, ret);

	return ret;
}
