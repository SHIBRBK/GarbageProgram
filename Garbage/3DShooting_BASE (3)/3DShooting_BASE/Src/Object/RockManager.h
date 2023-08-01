#pragma once
#include<map>
#include<vector>
#include"../Common/IntVector3.h"
class Transform;
class Rock;

class RockManager
{
public:

	//1マップあたりのサイズ
	static constexpr int MAP_SIZE = 2000;

	//1マップあたりのに生成する岩野和
	static constexpr int NUM_CREATE = 30;

	RockManager(const Transform* target);

	~RockManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
		//岩生成の中心
	const Transform* mTarget;

	std::map<IntVector3, std::vector<Rock*>> mMapRocks;

	//ランダム生成
	Rock* CreateRandom(IntVector3 mapPos);
};

