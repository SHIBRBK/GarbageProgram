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
	
	//GameScene��stage�N���X��錾���āA���s�ł���悤�ɂ��Ă��������B
	//DrawGrid��XYZ��`�悵�Ă�������

	//�Q�b�^�[
	int GetModelDungeon(void);
	int GetModelBattleShip(void);

	VECTOR GetPosBattleShip(void);

private:
	//�_���W����
	int mModelDungeon;
	//���
	int mModelBattleShip;

	VECTOR mPosDungeon;
	VECTOR mPosBattleShip;

};

