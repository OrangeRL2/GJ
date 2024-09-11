#pragma once
#include "Collision.h"
#include "Camera.h"
#include "Input.h"
#include "DXInput.h"
#include "FbxModel.h"
#include "FbxObject3D.h"
#include "CubeModel.h"
#include "CubeObject3D.h"
class MagmaBlock
{
public:
	//静的メンバ関数
	static void SetDevice(ID3D12Device* device) { MagmaBlock::device = device; }
	static void SetCamera(Camera* camera) { MagmaBlock::camera = camera; }
	static void SetInput(Input* input) { MagmaBlock::input = input; }
	static void SetDXInput(DXInput* dxInput) { MagmaBlock::dxInput = dxInput; }

	//コンストラクタ
	MagmaBlock() {};
	//デストラクタ
	~MagmaBlock() {};
	//初期化
	void Initialize();
	//更新
	void Update();

	void Erupt();
	void RemoveErupt();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);
	//モデルセット
	void SetModel(FbxModel* model) { this->model = model; }
	
	//セッター
	//変形行列セット
	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
	void SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }
	void SetScale(DirectX::XMFLOAT3 sca) { scale = sca; }
	void SetEruptFlag();
	//ゲッター
//変形行列
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }
	bool GetDelete() { return magmaDelete; }
private:
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;

	//モデル
	FbxModel* model;
	CubeModel* cubeModel;
	FbxObject3D* magmaObject = nullptr;
	std::unique_ptr<CubeObject3D> cubeObject;
	//当たり判定
	std::list<std::unique_ptr<Collision>> collisionsFloor;
	std::list<std::unique_ptr<Collision>> collisionsObstacle;
	std::list<std::unique_ptr<Collision>> collisionsGoal;

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,100.0f,100.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.1f,4.0f,0.1f };

	bool eruptFlag = false;
	bool magmaDelete = false;
	float shakeTimer = 0.0f;
};

