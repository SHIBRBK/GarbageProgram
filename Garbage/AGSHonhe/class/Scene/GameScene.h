#pragma once
#include<DxLib.h>
#include<vector>
#include<array>
#include <memory>
#include "BaseScene.h"
#include"../Common/Vector2D.h"
#include"../Manager/Camera.h"
#include"../Object/BattleShip.h"
#include"../Object/EnemyCannon.h"

using namespace std;

class SkyDome;

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	void SetSelectedStageIndex(int stageIndex);
	void SetSelectedShipIndex(int shipIndex);
	bool IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);
private:
	void Init(void)override;
	void Update()override;
	void Draw(void)override;
	void Release(void);
	int selectedStageIndex_;
	int selectedShipIndex_;
	uniqueBB bb;
	uniqueEC ec;
	uniqueCamera camera_;
	std::vector<EnemyCannon*>spawn_;
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
	SkyDome* mSkyDome;
	float mStepDestroy;
	int time;
	array<VERTEX2DSHADER, 4> verts;
};

