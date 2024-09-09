#include "Obstacle.h"

ID3D12Device* Obstacle::device = nullptr;
Camera* Obstacle::camera = nullptr;

void Obstacle::Initialize()
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

	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y;
	hitboxPosition.z = position.z;
	hitboxRotation = { 0.0f,0.0f,0.0f };
	hitboxScale = { 9.0f,3.0f,9.0f };
}

void Obstacle::Update()
{
	//hitbox調整
	/*hitboxPosition.x = position.x;
	hitboxPosition.y = position.y;
	hitboxPosition.z = position.z;
	hitboxRotation = { 0.0f,0.0f,0.0f };
	hitboxScale = { 6.0f,6.0f,6.0f };*/

	//変形行列セット
	object->SetPosition(position);
	object->SetRotation(rotation);
	object->SetScale(scale);
	//オブジェクト更新
	object->Update();

	cubeObject->SetPosition(hitboxPosition);
	cubeObject->SetScale(hitboxScale);
	cubeObject->SetRotation(hitboxRotation);
	cubeObject->Update();
}

void Obstacle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
	/*cubeObject->Draw(cmdList);*/
}

void Obstacle::SetHitbox()
{
	hitboxPosition.x = position.x;
	hitboxPosition.y = position.y + 3;
	hitboxPosition.z = position.z;
	hitboxRotation = { 0.0f,0.0f,0.0f };
	hitboxScale = { 11.0f,3.0f,11.0f };
}
