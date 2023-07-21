#include<DxLib.h>
#include "Graphics.h"
#include "../Common/DxCushion.h"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{
}


void Graphics::SetWorldMatrix(const MATRIX& matrix)
{
	world_matrix_ = matrix;
}

void Graphics::ClearScreen(void)
{
	DxCushion::MyClearDrawScreen();
}


void Graphics::DrawModel(int modelHandle)
{
	MV1DrawModel(modelHandle);
}

