#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include"Common/Transform.h"
#include "Rock.h"
#include "RockManager.h"
#include "../Utility/AsoUtility.h"

RockManager::RockManager(const Transform* target)
{
	mTarget = target;
}

RockManager::~RockManager(void)
{
}

void RockManager::Init(void)
{
}

void RockManager::Update(void)
{

	//mTarget=自機
	//①自機のマップ座標を計算
		VECTOR mapPos=mTarget->pos;
		mapPos.x /= MAP_SIZE;
		mapPos.y /= MAP_SIZE;
		mapPos.z /= MAP_SIZE;

		//②周囲9マップのデブリ生成状況を確認
		IntVector3 sPos = IntVector3(mapPos);
		sPos.Sub(1);
		IntVector3 checkrMapPos;
		int loop = 3;
		for (int x = 0; x< loop; x++) {
			for (int y = 0; y < loop; y++) {
				for (int z = 0; z < loop; z++) {
					checkrMapPos.x = sPos.x+x;
					checkrMapPos.y = sPos.y+y;
					checkrMapPos.z = sPos.z+z;

					if (mMapRocks.count(checkrMapPos) == 0) {
						std::vector<Rock*> vRocks;
						for (int i = 0; i < NUM_CREATE; i++) {
							vRocks.emplace_back(CreateRandom(checkrMapPos));

						
						}
						mMapRocks.emplace(checkrMapPos, vRocks);
					}
				}
			}
		}



	mMapRocks;
	
	std::map<IntVector3, std::vector<Rock*>> mMapRocks;
	//③デブリを生成していないマップがあったら作る
	//CreateRandom関数を使用
	
	//IntVector3 mapPos;
	
	std::vector<Rock*>;

	auto rock = CreateRandom(mapPos);
	//std::vector<Rock*>.push_back(Rock*);

	//mMapRocks.emplace(mapPos, vector<Rock*>);

	//岩がある程度離れたら岩を削除
	std::vector<IntVector3> deleteMap;

	IntVector3 shipMapPos = IntVector3(mapPos);

	IntVector3 rockMapPos;
	int len;
	for (const auto& p : mMapRocks)
	{
		rockMapPos = p.first;
		len = abs(rockMapPos.x - mapPos.x)
			+ abs(rockMapPos.y - mapPos.y)
			+ abs(rockMapPos.z - mapPos.z);
		if(len>4)
		{
		for(auto rock : p.second)
			{
			rock->Release();
			delete rock;
			}
		deleteMap.push_back(p.first);

		}
	}
	for(const auto& key:deleteMap)
	{
		mMapRocks.erase(key);//とある条件に達したやつを処理したい。動的
	}

}

void RockManager::Draw(void)
{
	for (const auto& p : mMapRocks) 
	{
		for (const auto& rock : p.second) 
		{

			rock->Draw();
		}
	}
}

void RockManager::Release(void)
{

	for (auto& p : mMapRocks)
	{
		for (const auto& rock : p.second)
		{

			rock->Release();
			delete rock;
		}
		p.second.clear();
	}
	mMapRocks.clear();
}

Rock* RockManager::CreateRandom(IntVector3 mapPos)
{
	//岩のモデルをランダム
	int r = GetRand(1);
	int rockModelID = -1;
	switch (r)
	{
	case0:
		rockModelID = ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::ROCK01
		);
		break;
	case1:
		rockModelID = ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::ROCK02
		);
		break;
	}

	const int MIN_SCL = 2;
	const int MAX_SCL = 10;
	//大きさ
	VECTOR scl;
	int s = MIN_SCL + GetRand(MAX_SCL - MIN_SCL);
	scl.x = static_cast<float>(s);
		scl.y = static_cast<float>(s);
		scl.z = static_cast<float>(s);
	//位置
	VECTOR pos;
	pos.x = static_cast<float>(mapPos.x * MAP_SIZE);
	pos.y = static_cast<float>(mapPos.y * MAP_SIZE);
	pos.z = static_cast<float>(mapPos.z * MAP_SIZE);




	int hMapSize = MAP_SIZE / 2;
	int rX = GetRand(MAP_SIZE) - hMapSize;
	int rY = GetRand(MAP_SIZE) - hMapSize;
	int rZ = GetRand(MAP_SIZE) - hMapSize;
	pos.x += rX;
	pos.y += rY;
	pos.z += rZ;




		//角度
	VECTOR rot;
	rot.x = AsoUtility::Deg2RadF(static_cast<float>(GetRand(360)));
	rot.y = AsoUtility::Deg2RadF(static_cast<float>(GetRand(360)));
	rot.z = 0.0f;

	Rock* ret = new Rock();
	ret->Init(rockModelID, pos, rot, scl);



	return ret;
}
