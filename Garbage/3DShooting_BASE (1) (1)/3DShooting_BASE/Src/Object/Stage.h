#pragma once
class Stage
{
public:

	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawGrid(void);
	void Release(void);

	int GetModelDungeon(void);
	int GetModelCargoShip(void);
	VECTOR GetPosCargoShip()
	{
		return mPosCargoShip;
	}
private:

	//ダンジョン
	int mModelDungeon;
	//戦艦
	int mModelCargoShip;

	VECTOR mPosDungeon;
	VECTOR mPosCargoShip;
};

