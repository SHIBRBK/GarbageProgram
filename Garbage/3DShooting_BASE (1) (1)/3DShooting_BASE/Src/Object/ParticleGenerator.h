#pragma once
#include<Dxlib.h>
#include<vector>
#include "../Common/Quaternion.h"

class Particle;

class ParticleGenerator
{
public:
	// �p�[�e�B�N���̐�����
	static constexpr int NUM_CREATE = 30;

	static constexpr int NUM_ANGLE = 10;
	static constexpr float SPLIT_ANGLE = 360.0f / NUM_ANGLE;
	static constexpr float GEN_MIN_SIZE = 10.0f;
	static constexpr float GEN_MAX_SIZE = 20.0f;
	static constexpr float GEN_MIN_SPEED = 10.0f;
	static constexpr float GEN_MAX_SPEED = 20.0f;
	static constexpr float GEN_MIN_LIFE_TIME = 5.0f;
	static constexpr float GEN_MAX_LIFE_TIME = 10.0f;
	static constexpr float GEN_MIN_X_ROT = 30.0f;
	static constexpr float GEN_MAX_X_ROT = 90.0f;


	static constexpr float GEN_TIME = 0.1f;

	// �R���X�g���N�^
	ParticleGenerator(VECTOR pos, float radius);
	// �f�X�g���N�^
	~ParticleGenerator(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void DrawMeshSquare(void);
	void DrawMeshCircle(void);
	void Release(void);

	void SetPos(const VECTOR& pos);
	void SetRot(const Quaternion& rot);

private:
	// �p�[�e�B�N���摜
	int mImg;
	// ���W
	VECTOR mPos;
	// ����
	Quaternion mRot;
	// �p�x
	float mRadius;
	Quaternion mAngleQ;
	// ���_��
	int mCntVertex;
	// ���_�f�[�^(�l�p�p)
	VERTEX3D mVerticesSquare[6];
	// ���_�f�[�^(�~�`�p)
	VERTEX3D mVerticesCircle[NUM_ANGLE + 1];
	// ���_�C���f�b�N�X(���������|���S�� * ��|���S��������̒��_��)
	WORD mIndexes[NUM_ANGLE * 3];
	// �p�[�e�B�N��
	std::vector<Particle*> mParticles;
	// �l�p���b�V�������
	void CreateMeshSquare(void);
	// �~�`���b�V�������
	void CreateMeshCircle(void);
	// �p�[�e�B�N������
	Particle* Generate(Particle* particle);
	// Z�\�[�g
	void ZSort(void);
	//���X�ɐ���
	float mStepGenerate;

};

