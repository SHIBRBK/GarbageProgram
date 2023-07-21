#include <DxLib.h>
//#include "../../Utility/AsoUtility.h"
#include "Transform.h"

Transform::Transform(void)
{
	modelId = -1;

	scl = { 1.0f,1.0f,1.0f };
	rot = { 0.0f,0.0f,0.0f };
	pos = { 1.0f,1.0f,1.0f };

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();
	collider = nullptr;
}

Transform::Transform(int model)
{
	modelId = model;

	scl = { 1.0f,1.0f,1.0f };
	rot = { 0.0f,0.0f,0.0f };
	pos = { 1.0f,1.0f,1.0f };

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();
	
}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{

	// 大きさ
	matScl = MGetScale(scl);

	// 回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// 位置
	matPos = MGetTranslate(pos);

	// 行列の合成
	MATRIX mat = MGetIdent();//単位行列を取得する
	mat = MMult(mat, matScl);//大きさを合成		MMult()// 行列の乗算を行う
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());//回転を合成
	mat = MMult(mat, matPos);//位置を合成

	// 行列をモデルに判定
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

	// 衝突判定の更新
	//if (collider != nullptr)
	//{
	//	MV1RefreshCollInfo(modelId);
	//}

}

void Transform::Release(void)
{
	if (collider != nullptr)
	{
		MV1TerminateCollInfo(modelId);
	}
}


void Transform::SetModel(int model)
{
	modelId = model;
}

VECTOR Transform::GetForward(void) const
{
	return GetDir({ 0.0f, 0.0f, 1.0f });
}

VECTOR Transform::GetBack(void) const
{
	return GetDir({ 0.0f, 0.0f, -1.0f });
}

VECTOR Transform::GetRight(void) const
{
	return GetDir({ 1.0f, 0.0f, 0.0f });
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir({ -1.0f, 0.0f, 0.0f });
}

VECTOR Transform::GetUp(void) const
{
	return GetDir({ 0.0f, 1.0f, 0.0f });
}

VECTOR Transform::GetDown(void) const
{
	return GetDir({ 0.0f, -1.0f, 0.0f });
}

VECTOR Transform::GetDir(VECTOR vec) const
{
	return VNorm(VTransform(vec, quaRot.ToMatrix()));
}

void Transform::MakeCollider(Collider::TYPE type)
{
	if (modelId == -1)
	{
		return;
	}

	collider = new Collider(type, modelId);
	int ret = MV1SetupCollInfo(modelId, -1, 1, 1, 1);
}
