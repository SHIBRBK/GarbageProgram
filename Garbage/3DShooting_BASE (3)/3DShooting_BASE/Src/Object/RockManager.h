#pragma once
#include<map>
#include<vector>
#include"../Common/IntVector3.h"
class Transform;
class Rock;

class RockManager
{
public:

	//1�}�b�v������̃T�C�Y
	static constexpr int MAP_SIZE = 2000;

	//1�}�b�v������̂ɐ���������a
	static constexpr int NUM_CREATE = 30;

	RockManager(const Transform* target);

	~RockManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:
		//�␶���̒��S
	const Transform* mTarget;

	std::map<IntVector3, std::vector<Rock*>> mMapRocks;

	//�����_������
	Rock* CreateRandom(IntVector3 mapPos);
};

