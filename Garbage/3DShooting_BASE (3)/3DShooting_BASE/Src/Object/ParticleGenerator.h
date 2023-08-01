#pragma once
#include<vector>
#include<DxLib.h>
#include"../Common/Quaternion.h"


class Particle;

class ParticleGenerator
{
public:

	//�ړ�����
	static constexpr float GEN_MIN_X_ROT = -40.0f;
	static constexpr float GEN_MAX_X_ROT = -40.0f;

	//�傫��
	static constexpr float GEN_MIN_SIZE = 3.0f;
	static constexpr float GEN_MAX_SIZE = 5.0f;

	//�ړ����x
	static constexpr float GEN_MIN_SPEED = 1.0f;
	static constexpr float GEN_MAX_SPEED = 2.0f;


	//��������
	static constexpr float GEN_MIN_LIFE_TIME = 1.0f;
	static constexpr float GEN_MAX_LIFE_TIME = 2.0f;

	//���X�ɐ���
	static constexpr float GEN_TIME = 0.05f;


	//�p�[�e�B�N���̐�����
	static constexpr int NUM_CREATE = 30;

	//����~�`���b�V��
	static constexpr int NUM_ANGLE = 10;
	static constexpr float SPLIT_ANGLE = 360.0f/ NUM_ANGLE;


	ParticleGenerator(VECTOR pos,float radius);
	~ParticleGenerator(void);

	void Init();
	void Update();
	void Draw();
	void DrawMeshSquare();
	void DrawMeshCircle();
	void Release();

	void SetPos(const VECTOR& pos);


	void SetRot(const Quaternion& rot);

private:

	//�p�[�e�B�N���摜
	int mImage;



	//���W
	VECTOR mPos;

	//����(�p�x)
	Quaternion mRot;
	


	float mRadius;

	//���_��
	int mCntVertex;



	//���_�f�[�^(���i�p)
	VERTEX3D mVerticesSquare[6];
	VERTEX3D mVerticesCircle[NUM_ANGLE+1];

	//���_�C���f�b�N�X
	WORD mIndexs[NUM_ANGLE * 3];


	//���X�ɐ������邽�߂̌o�ߎ���
	float mStepGenerate;

	//�p�[�e�B�N��
	std::vector<Particle*> mParticles_;

	//
	void CreatemeshSquare();

	//�~�`���b�V��
	void CreatemeshCircle();

	//�p�[�e�B�N������
	Particle* Generate(Particle* particle);


	//Z�\�[�g�@
	void ZSort(void);

};



