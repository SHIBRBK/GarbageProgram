#pragma once
#include<DxLib.h>
#include<memory>
class Graphics;
using uniqueGrap= std::unique_ptr<Graphics>;

class Graphics
{
public:
	static Graphics& GetInstance()
	{
		static Graphics instance;
		return instance;
	}

	void SetWorldMatrix(const MATRIX& matrix);
	void ClearScreen(void);
	void DrawModel(int modelHandle);
private:
	Graphics();
	~Graphics();
	MATRIX world_matrix_;
};

