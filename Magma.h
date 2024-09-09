#pragma once
#include "FbxObject3D.h"
#include "FbxModel.h"
#include "CubeModel.h"
#include "CubeObject3D.h"
#include "DirectXMath.h"

class Magma
{
public:
	//静的メンバ関数
	static void SetDevice(ID3D12Device* device) { Magma::device = device; }
	static void SetCamera(Camera* camera) { Magma::camera = camera; }
	//モデルセット
	void SetModel(FbxModel* model) { this->model = model; }
	//hitbox用のモデル
	void SetCubeModel(CubeModel* model) { this->cubeModel = model; }

	//更新
	void Initialize();
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//ゲッター
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }
	//hitbox
	DirectX::XMFLOAT3 GetHitboxPosition() { return hitboxPosition; }
	DirectX::XMFLOAT3 GetHitboxRotation() { return hitboxRotation; }
	DirectX::XMFLOAT3 GetHitboxScale() { return hitboxScale; }
	//セッター
	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
	void SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }
	void SetScale(DirectX::XMFLOAT3 sca) { scale = sca; }

	//ヒットボックスセット(ファイルを読み込んだ後にする)
	void SetHitbox();

private:
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//オブジェクト
	FbxModel* model;
	FbxObject3D* object = nullptr;

	//hitbox
	CubeModel* cubeModel;
	std::unique_ptr<CubeObject3D> cubeObject;

	//変形行列
	DirectX::XMFLOAT3 position = { 10.0f,5.0f,0.0f };
	DirectX::XMFLOAT3 rotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.06f,0.06f,0.06f };

	//hitboxの変形行列
	DirectX::XMFLOAT3 hitboxPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxRotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxScale = { 0.01f,0.01f,0.01f };
};

