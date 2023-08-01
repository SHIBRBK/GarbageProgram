#include "ParticleGenerator.h"
#include"../Utility/AsoUtility.h"
#include"../Application.h"
#include"../Common/Quaternion.h"
#include"../Manager/Resource.h"
#include"../Manager/ResourceManager.h"
#include"../Manager/SceneManager.h"
#include"../Manager/Camera.h"
#include"Particle.h"

ParticleGenerator::ParticleGenerator(VECTOR pos, float radius)
{
	mPos = pos;
	mRadius = radius;
}

ParticleGenerator::~ParticleGenerator(void)
{
}

void ParticleGenerator::Init()
{
	mImage = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::LIGHT
	).mHandleId;


	CreatemeshSquare();
	//�~�`���b�V��
	CreatemeshCircle();

	//��C�ɍ��
	//for(int i=0;i<NUM_CREATE;i++)
	//{
	//	mParticles_.emplace_back(Generate(nullptr));
	//}
	
	//���X�ɍ��
	for(int i=0;i<NUM_CREATE;i++)
	{
		mParticles_.emplace_back(new Particle());
	}


	//�p�x�̏�����
	mRot = Quaternion::Euler(0.0f, 0.0f, 0.0f);

	mStepGenerate = GEN_TIME;
}

void ParticleGenerator::Update()
{

	bool isGenerate = false;
	mStepGenerate -= SceneManager::GetInstance().GetDeltaTime();

	
	if (mStepGenerate < 0.0f) {
		isGenerate = true;
		mStepGenerate = GEN_TIME;
	}
	for(auto& p : mParticles_)
	{
		p->Update();

		//�������Ԃ��؂ꂽ��Đ���
		if (!p->IsAlive()) 
		{
			if (isGenerate)
			{
				p = Generate(p);
				mStepGenerate = GEN_TIME;
				isGenerate = false;
			}
		}
	}
#ifdef DEBUG
	//ParticleGenerator=mRot�̉�]����

	bool isHit=flase;


	//�E��]
	if (CheckHitKey(KEY_INPUT_L)) {
		mRot = mRot.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadD(1.0), { 0.0f,1.0f,0.0f }));

		mRot = mRot.Mult(axis);
		isHit = true;
		
	}


	if (CheckHitKey(KEY_INPUT_J)) {
		mRot = mRot.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadD(-1.0), { 0.0f,1.0f,0.0f }));
		isHit = true;

}

	if (CheckHitKey(KEY_INPUT_K)) {
		mRot = mRot.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadD(1.0), { 1.0f,0.0f,0.0f }));
		isHit = true;

	}
	//���]
	if (CheckHitKey(KEY_INPUT_I)) {
		mRot = mRot.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadD(-1.0), { 1.0f,0.0f,0.0f }));
		isHit = true;
	}

	if (isHit) {
		CreatemeshCircle();
	}

#endif 
}

void ParticleGenerator::Draw()
{
	DrawMeshSquare();
	DrawMeshCircle();

	//Z�\�[�g
	ZSort();

	SetDrawBlendMode(DX_BLENDMODE_ADD,128);
	for (auto& p : mParticles_)
	{
		p->Draw();
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	Quaternion tmpQ = Quaternion::Euler(
		AsoUtility::Deg2RadF(-90.0f),
		0.0f, 
		0.0f
	);

	tmpQ = mRot.Mult(tmpQ);
	//DrawLine3D(
	//	mPos,
	//	VAdd()
	//);

}

void ParticleGenerator::DrawMeshSquare()
{
	DrawPolygon3D(mVerticesSquare, 1, DX_NONE_GRAPH, true);
}

void ParticleGenerator::DrawMeshCircle()
{
	DrawPolygonIndexed3D(mVerticesCircle, NUM_ANGLE+1,mIndexs,NUM_ANGLE,
		DX_NONE_GRAPH,true);

	for (auto& vert : mVerticesCircle) 
	{
		DrawSphere3D(vert.pos, 2.0f, 5, 0xff0000, 0xff0000, true);
	}

}

void ParticleGenerator::Release()
{
	
	for (auto& p : mParticles_)
	{
		if (p != nullptr) {
			p->Release();
			delete p;
		}
	}
	mParticles_.clear();
}

void ParticleGenerator::SetPos(const VECTOR& pos)
{
	
		mPos = pos;//�ʒu�̓��@�H
	
		CreatemeshCircle();
}

void ParticleGenerator::SetRot(const Quaternion& rot)
{
	mRot=rot;
}

void ParticleGenerator::CreatemeshSquare()
{
	COLOR_U8 colorW = GetColorU8(255, 255, 255, 255);
	COLOR_U8 colorR = GetColorU8(255, 0, 0, 255);

	float z = 0.0f;
	float h = 100.0f;

	//�l�p�`�����
	mVerticesSquare[0].pos = { z,z,z };
	mVerticesSquare[0].dif = colorW;
	mVerticesSquare[1].pos = {z,z,h};
	mVerticesSquare[1].dif = colorW;
	mVerticesSquare[2].pos = {h,z,h};
	mVerticesSquare[2].dif = colorW;

	mVerticesSquare[3].pos = { z,z,z };
	mVerticesSquare[3].dif = colorW;
	mVerticesSquare[4].pos = { h,z,h };
	mVerticesSquare[4].dif = colorW;
	mVerticesSquare[5].pos = { h,z,z };
	mVerticesSquare[5].dif = colorW;

}

void ParticleGenerator::CreatemeshCircle()
{
	COLOR_U8 colorW = GetColorU8(255, 255, 255, 255);
	COLOR_U8 colorR = GetColorU8(255, 0, 0, 255);
	COLOR_U8 colorG = GetColorU8(0, 255, 0, 255);
	COLOR_U8 colorB = GetColorU8(0, 0, 255, 255);

	mCntVertex = 0;

	//�~�̒��S
	mVerticesCircle[0].pos = mPos;
	mVerticesCircle[0].dif = colorB;

	//�~�`�̒��_���쐬

	int cntAngle=0;
	for (int i = 1; i < NUM_ANGLE; i++) {


		//���_���W(�N�H�[�^�j�I��)
		Quaternion rot = Quaternion::AngleAxis(
			AsoUtility::Deg2RadF(cntAngle), AsoUtility::AXIS_Y);

		rot = mRot.Mult(rot);


		//���W����]
		VECTOR localPos = rot.PosAxis({ 0.0f,0.0f,mRadius });

		VERTEX3D vertex = VERTEX3D();

		vertex.pos = VAdd(mPos, localPos);

		vertex.dif = colorG;

		mVerticesCircle[i + 1] = vertex;

		cntAngle += SPLIT_ANGLE;

	}

	int cntIdx = 0;

	int cntVIdx = 1;
	for(int i=1;i<NUM_ANGLE;i++)
	{
		mIndexs[cntIdx++] = 0;
		mIndexs[cntIdx++] = cntVIdx;
		mIndexs[cntIdx++] = ++cntVIdx;
	}


	mIndexs[cntIdx++] = 0;
	mIndexs[cntIdx++] = cntVIdx;
	mIndexs[cntIdx++] = 1;


	/*Quaternion axis;
	axis = Quaternion::AngleAxis(
		AsoUtility::Deg2RadF(36.0f), AsoUtility::AXIS_Y);*/


	//��������Ȃ��q��
	//float localRadY=AsoUtility::Deg2RadF(36.0f);
	//Quaternion axis;
	//axis=Quaternion::AngleAxis(localRadY, { 1.0f,0.0f,0.0f });

	//Quaternion::PosAxis(axis, mPos);
	


	

	

}

Particle* ParticleGenerator::Generate(Particle* particle)
{

	//�AmRot���g�p���āA�ʒu���������]���Ă�������
	//1�s�����Ή�]���܂�
	//�~�`���b�V����x��]



	//mRot.AngleAxis(AsoUtility::Deg2RadD(1.0),mPos);

	if (particle == nullptr) {
		particle = new Particle();	//���̓C���X�^���X���ďd���@�d�Ȃ�ƂȂ�����

	}

	//�����̑O�������̂����������W�@���a�@36�x�@���������܂��g���Ɖ~�`�ł��邮��񂹂�B
	//�����_����x����n��

	//�����_���Ȉʒu
	//�����_���ȑ傫��
	//�����_���Ȍ���
	// �����_���ȃX�s�[�h
	//�傫���Ƃ���
	//��قǂ̉~����鎞��36�x�̉�]���q���g�ɂȂ邩��

	//Quaternion rot = Quaternion::AngleAxis(AsoUtility::Deg2RadF(cntAngle), AsoUtility::AXIS_Y);
	
	
	//VECTOR localPos = rot.PosAxis({ 0.0f,0.0f,mRadius });

	//�ʒu�H(��Փx�R)

	//���_���W(�N�H�[�^�j�I��)
		Quaternion rotY = Quaternion::AngleAxis(AsoUtility::Deg2RadF(static_cast<float>(GetRand(360))), 
			AsoUtility::AXIS_Y);

		float min = mRadius * (3.0f / 4.0f);
		//���W����]
		VECTOR pos = rotY.PosAxis({ 0.0f,0.0f,static_cast<float>(
			min +GetRand(AsoUtility::Round(mRadius-min)))});

		//���S���W�ɉ�����B
		VECTOR randPos = VAdd(mPos, pos);


	

	//�傫�� ��Փx 1
	float size = GEN_MIN_SIZE +GetRand(static_cast<int>(GEN_MAX_SIZE-GEN_MIN_SIZE));
	//�����@��Փx 5

	float randDegX = GEN_MIN_X_ROT + GetRand(
		static_cast<int>(GEN_MAX_X_ROT - GEN_MIN_X_ROT));


	Quaternion rotX = Quaternion::AngleAxis(
		AsoUtility::Deg2RadF(randDegX), AsoUtility::AXIS_X);

	rotY = rotY.Mult(rotY);//���������̈�s


	//rotX��dir��vector�ɕϊ�

	//�s���
	//VECTOR dir = VTransform(AsoUtility::AXIS_Z, rotX.ToMatrix());//Z�̐������ɔ�΂�
	//VECTOR dir = rotX.PosAxis(rotY,AsoUtility::AXIS_Z);
	VECTOR dir = rotX.GetForward();
	//����]�̏ꍇ��z�̐Õ���




	//�����B�����Ă邩�ǂ���
	/*VECTOR dir = { 0.0f,1.0f,0.0f};
	Quaternion tmpQ=rotX.PosAxis(rotX, dir);
	Quaternion mAxisQ = mAxisQ.Mult(tmpQ);*/
	



	//�X�s�[�h ��Փx 1
	float speed = GEN_MIN_SPEED + GetRand(static_cast<int>(GEN_MAX_SPEED - GEN_MIN_SPEED));
	//�������� ��Փx 1
	float lifetime = GEN_MIN_LIFE_TIME + GetRand(static_cast<int>(GEN_MAX_LIFE_TIME - GEN_MIN_LIFE_TIME));

	rotY = mRot.Mult(rotY);


	//����
	particle->Generate(mImage, pos, size,dir, speed, lifetime);



	return particle;
}

void ParticleGenerator::ZSort(void)
{

	//�J����
	Camera* camera=SceneManager::GetInstance().GetCamera();
	//1�J�����̕���
	VECTOR cameraDir = camera->GetDir();

	

	for (auto p: mParticles_) 
	{
		//���낢��v�Z
		
		//�J�������猩���G�t�F�N�g����
	

		//Z������ݒ�
		//camera2Particle
		//cameraDir

		

		VECTOR NormCam=VNorm(cameraDir);

		VECTOR camera2Particle;
		camera2Particle = VSub(p->GetPos(), camera->GetPos());
		p->SetZLen(VDot(cameraDir, camera2Particle));

	}

	//�\�[�g
	sort(mParticles_.begin(), mParticles_.end(),
		[](Particle* x, Particle* y)
	{
			return x->GetZLen() > y->GetZLen(); 
	}
	);
}
