#include "MagmaBlock.h"
ID3D12Device* MagmaBlock::device = nullptr;
Camera* MagmaBlock::camera = nullptr;
Input* MagmaBlock::input = nullptr;
void MagmaBlock::Initialize()
{
	//オブジェクト初期化
	magmaObject = new FbxObject3D;
	magmaObject->Initialize();
	magmaObject->SetModel(model);
	magmaObject->PlayAnimation();

	//ヒットボックスの設定
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject.reset(newCubeObject);
	cubeObject->SetModel(cubeModel);
}

void MagmaBlock::Update()
{
	//オブジェクト更新
	magmaObject->SetPosition(position);
	magmaObject->SetScale(scale);
	magmaObject->SetRotation(rotation);
	magmaObject->Update();
	Erupt();
}

void MagmaBlock::Draw(ID3D12GraphicsCommandList* cmdList)
{
	magmaObject->Draw(cmdList);
}

void MagmaBlock::Erupt()
{
	if (eruptFlag == true) {
		shakeTimer++;
		if (shakeTimer == 1) {
			position.y = -400.0f;
			//scale.y = 0.0f;
		}
		if (shakeTimer <= 100.0f && shakeTimer>=30.0f) {
			position.x;
			position.y+=5.0f;
			position.z;
			//scale.y += 0.2f;
		}
		else if (shakeTimer >= 100.0f) {
			eruptFlag = false;
			shakeTimer = 0.0f;
			
		}
	}
	else if(eruptFlag==false){
	if (position.y >= -400.0f){
		position.y += 2.0f;
	}
	if (scale.y >= 0.0f) {
		//scale.y -= 0.1f;
	}
	}
}
void MagmaBlock::SetEruptFlag()
{
	eruptFlag = true;

}
void MagmaBlock::RemoveErupt()
{
	position.z = 100.0f;
	magmaDelete == false;
}