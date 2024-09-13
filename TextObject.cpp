#include "TextObject.h"

ID3D12Device* TextObject::device = nullptr;
Camera* TextObject::camera = nullptr;

void TextObject::Initialize()
{
	//オブジェクト初期化
	object = new FbxObject3D;
	object->Initialize();
	object->SetModel(model);
}

void TextObject::Update()
{
	//オブジェクト更新
	object->SetPosition(position);
	object->SetScale(scale);
	object->SetRotation(rotation);
	object->Update();
}

void TextObject::Draw(ID3D12GraphicsCommandList* cmdList)
{
	object->Draw(cmdList);
}

void TextObject::SetTutorial()
{
	/*SetPosition({ 50,10,50 });
	SetScale({ 50,10,50 });*/
}