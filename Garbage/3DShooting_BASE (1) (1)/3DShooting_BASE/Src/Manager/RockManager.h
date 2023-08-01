#pragma once
#include<map>
#include<vector>
#include"../Common/IntVector3.h"

class Transform;
class Rock;

class RockManager
{
public:

	//��}�b�v������̃T�C�Y

	static constexpr int MAP_SIZE = 2000;

	//�P�}�b�v������ɐ��������
	static constexpr int NUM_CREATE = 30;

	// �R���X�g���N�^
	RockManager(const Transform* target);

	// �f�X�g���N�^
	~RockManager(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawGrid(void);
	void Release(void);
private:
	
	//�␶���̒��S
	const Transform* mTarget;

	//�}�b�v�Ǘ�
	std::map < IntVector3,std::vector<Rock* >> mMapRocks;

	//�����_������
	Rock* CreateRandom(IntVector3 mapPos);
};

