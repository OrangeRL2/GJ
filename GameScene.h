#pragma once
#include "memory"
#include "list"

#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "FbxObject3D.h"
#include "CubeObject3D.h"
#include "CubeModel.h"
#include "Camera.h"

class GameScene
{
	//メンバ関数
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	void Update();
	void Draw();

	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();

	//カメラ
	std::unique_ptr<Camera> camera_;

	//fbx
	FbxModel* model1 = nullptr;
	FbxObject3D* object1 = nullptr;

	//キューブ
	std::unique_ptr<CubeModel> cubeModel;
	std::unique_ptr<CubeObject3D> cubeObject;
};