#include "Floor.h"

ID3D12Device* Floor::device = nullptr;
Camera* Floor::camera = nullptr;

void Floor::Initialize()
{
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject.reset(newCubeObject);
	cubeObject->SetModel(cubeModel);
}

void Floor::Update()
{
	//オブジェクト更新
	cubeObject->SetPosition(position);
	cubeObject->SetScale(scale);
	cubeObject->SetRotation(rotation);
	cubeObject->Update();
}

void Floor::Draw(ID3D12GraphicsCommandList* cmdList)
{
	cubeObject->Draw(cmdList);
}

void Floor::SetTitle()
{
	SetScale({ 1000,0.5,1000});
	SetPosition({ 180,0,0 });
}

void Floor::SetTutorial()
{
	SetScale({ 120,0.5,120 });
	SetPosition({ 0,0,0 });
}
