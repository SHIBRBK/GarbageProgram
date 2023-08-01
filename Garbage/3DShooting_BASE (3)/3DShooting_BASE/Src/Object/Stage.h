#pragma once

class Stage
{
	static constexpr int LEN_LINE = 500;
public:
	Stage(void);
	~Stage(void);
	void Init(void);
	void Draw(void);
	void DrawGrid(void);
	void Update(void);
	void Release(void);
	
	//GameSceneにstageクラスを宣言して、実行できるようにしてください。
	//DrawGridでXYZを描画してください

	//ゲッター
	int GetModelDungeon(void);
	int GetModelBattleShip(void);

	VECTOR GetPosBattleShip(void);

private:
	//ダンジョン
	int mModelDungeon;
	//戦艦
	int mModelBattleShip;

	VECTOR mPosDungeon;
	VECTOR mPosBattleShip;

};

