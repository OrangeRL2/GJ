#pragma once
#include "Collision.h"
#include "Camera.h"
#include "FbxModel.h"
#include "FbxObject3D.h"
#include "CubeModel.h"
#include "CubeObject3D.h"

class Goal
{
	//メンバ関数
public:
	//静的メンバ関数
	static void SetDevice(ID3D12Device* device) { Goal::device = device; }
	static void SetCamera(Camera* camera) { Goal::camera = camera; }
	//コンストラクタ
	Goal() {};
	//デストラクタ
	~Goal() {};
	//初期化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//モデルセット
	void SetModel(FbxModel* model) { this->model = model; }
	//hitbox用のモデル
	void SetCubeModel(CubeModel* model) { this->cubeModel = model; }

	//シーンごとのセット
	void SetTutorial();
	void SetStage2();
	void SetStage4();

	//セッター
	//変形行列セット
	void SetPosition(DirectX::XMFLOAT3 pos) { position = pos; }
	void SetRotation(DirectX::XMFLOAT3 rot) { rotation = rot; }
	void SetScale(DirectX::XMFLOAT3 sca) { scale = sca; }

	//ゲッター
	//変形行列
	DirectX::XMFLOAT3 GetPosition() { return position; }
	DirectX::XMFLOAT3 GetRotation() { return rotation; }
	DirectX::XMFLOAT3 GetScale() { return scale; }

	//メンバ変数
private:
	//デバイス
	static ID3D12Device* device;
	//カメラ
	static Camera* camera;
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//モデル兼オブジェクト(プレイヤーが2つあるので2つ生成)
	/*std::unique_ptr<Metaball> metaball0;
	std::unique_ptr<Metaball> metaball1;*/

	//モデル
	FbxModel* model;
	CubeModel* cubeModel;
	FbxObject3D* object = nullptr;
	std::unique_ptr<CubeObject3D> cubeObject;

	//変形行列
	DirectX::XMFLOAT3 position = { 0.0f,-5.0f,0.0f };
	DirectX::XMFLOAT3 rotation = { -1.58f,0.0f,0.0f };
	DirectX::XMFLOAT3 scale = { 0.06f,0.06f,0.06f };

	//hitboxの変形行列
	DirectX::XMFLOAT3 hitboxPosition = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxRotation = { 0.0f,0.0f,0.0f };
	DirectX::XMFLOAT3 hitboxScale = { 0.01f,0.01f,0.01f };
};