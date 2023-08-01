#pragma once
class Stage
{
public:

	// �R���X�g���N�^
	Stage(void);

	// �f�X�g���N�^
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

	//�_���W����
	int mModelDungeon;
	//���
	int mModelCargoShip;

	VECTOR mPosDungeon;
	VECTOR mPosCargoShip;
};

