#pragma once
#include<map>
#include<vector>
#include"../Common/IntVector3.h"

class Transform;
class Rock;

class RockManager
{
public:

	//一マップ当たりのサイズ

	static constexpr int MAP_SIZE = 2000;

	//１マップ当たりに生成する岩
	static constexpr int NUM_CREATE = 30;

	// コンストラクタ
	RockManager(const Transform* target);

	// デストラクタ
	~RockManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawGrid(void);
	void Release(void);
private:
	
	//岩生成の中心
	const Transform* mTarget;

	//マップ管理
	std::map < IntVector3,std::vector<Rock* >> mMapRocks;

	//ランダム生成
	Rock* CreateRandom(IntVector3 mapPos);
};

