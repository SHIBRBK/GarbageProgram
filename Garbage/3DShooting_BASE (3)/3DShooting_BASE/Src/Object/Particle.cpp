#include<DxLib.h>
#include "Particle.h"
#include"../Manager/SceneManager.h"

Particle::Particle(void)
{


}

Particle::~Particle(void)
{


}

void Particle::Generate(int img,VECTOR pos, float size, VECTOR dir, float speed, float lifetime)
{

	mImg = img;
	//�o���ꏊ
	mPos = pos;
	//�傫��
	mSize = size;
	//�ړ�����
	mDir = dir;
	//�X�s�[�h
	mSpeed = speed;
	//��������
	mLifeTime = lifetime;

}

void Particle::Update()
{
	float deltaTime = SceneManager::GetInstance().GetDeltaTime();
	if (mLifeTime > 0.0f) {
		mLifeTime -= deltaTime;
	}

	else 
	{
		return;
	}
	


	//�ړ�(���W+=�ړ������~�X�s�[�h�~�f���^�^�C��)
	mPos = VAdd(mPos,VScale(mDir,mSpeed*deltaTime));
	//���ƂŃI�y���[�^�[��+=���̂����肩��


}

void Particle::Draw()
{
	if (IsAlive()) {
	
		return;
	}	
	DrawBillboard3D(mPos, 0.5f, 0.5f, mSize, 0.0f, mImg, true);
	

}

void Particle::Release()
{


}

bool Particle::IsAlive(void) const
{


	return mLifeTime>0.0f;
}

VECTOR Particle::GetPos(void) const
{
	return mPos;
}

float Particle::GetZLen(void) const
{
	return mZLen;
}

void Particle::SetZLen(const float& zlen)
{
	mZLen = zlen;
}



void Particle::SetPos(const VECTOR& pos)
{
	mPos = pos;
}

