#include "Goal.h"

ID3D12Device* Goal::device = nullptr;
Camera* Goal::camera = nullptr;

void Goal::Initialize()
{
	//オブジェクト初期化
	object = new FbxObject3D;
	object->Initialize();
	object->SetModel(model);

	//ヒットボックスの設定
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject.reset(newCubeObject);
	cubeObject->SetModel(cubeModel);
	//ヒットボックスのスケール設定
	hitboxScale.x = 6.0f;
	hitboxScale.y = 6.0f;
	hitboxScale.z = 6.0f;

}

void Goal::Update()
{
	/*hitboxPosition.x = position.x;
	hitboxPosition.y = position.y;
	hitboxPosition.z = position.z;*/

	//オブジェクト更新
	object->SetPosition(position);
	object->SetScale(scale);
	object->SetRotation(rotation);
	object->Update();

	cubeObject->SetPosition(hitboxPosition);
	cubeObject->SetScale(hitboxScale);
	cubeObject->SetRotation(hitboxRotation);
	cubeObject->Update();
}

void Goal::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
	cubeObject->Draw(cmdList);
}

void Goal::SetTutorial()
{
	SetPosition({ -40.0	,190.0	,40.0 });

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxScale = { 10.0f,10.0f,10.0f };
}
void Goal::SetStage2()
{
	SetPosition({ 0.0f	,200.0f	,30.0f });

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxScale = { 6.0f,6.0f,6.0f };
}
void Goal::SetStage3()
{
	SetPosition({ 0.0f	,160.0f	,80.0f });

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxScale = { 6.0f,6.0f,6.0f };
}
void Goal::SetStage4()
{
	SetPosition({ 50.0,   -31.0,  54.0 });
	SetScale({ 1.0,   1.0,  1.0 });

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxScale = { 6.0f,6.0f,6.0f };
}