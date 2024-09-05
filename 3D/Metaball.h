#pragma once
#include "vector"
#include "DirectXMath.h"
#include "DirectXCommon.h"
#include "DirectXTex.h"
#include "windows.h"
#include "wrl.h"
#include "d3d12.h"
#include "d3dx12.h"
#include "Camera.h"
#include "Collision.h"
#include "input.h"

const int fine = 16;	//球体の細かさ	変数宣言用
const int fine2 = fine * fine * 2;	//描画に使う頂点の数
const int fine3 = fine * fine * 3;	//インデックスの数
const int fine4 = fine * fine + fine;	//頂点の数

class Metaball
{
private:	//エイリアス
//Microsoft::WRL::を省略
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;
	using TexMetadata = DirectX::TexMetadata;
	using ScracthImage = DirectX::ScratchImage;
	//std::を省略
	using string = std::string;
	template<class T>using vector = std::vector<T>;

public://サブクラス
//定数バッファ用データ構造体
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};
public:	//静的メンバ関数
//セッター
	static void SetDevice(ID3D12Device* device) { Metaball::device = device; }
	static void SetCamera(Camera* camera) { Metaball::camera = camera; }
	static void SetInput(Input* input) { Metaball::input = input; }
	//グラフィックスパイプラインの生成
	static void CreateGraphicsPipeline();

private://静的メンバ変数
	static ID3D12Device* device;
	static Camera* camera;
	static Input* input;

public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//バッファ生成
	void CreateBuffers();
	//頂点生成
	void CreateVertex();
	//色設定
	void SetImageData(XMFLOAT4 color);
	//当たり判定
	void UpdateCollision(Collision* collision);
	//描画
	void Draw(ID3D12GraphicsCommandList* cmdList);

	//モデルを変形させるとき呼ぶ
	void UpdateVertex();
	//頂点初期化(球体に戻す)
	void InitializeVertex();
	//一つの点に対してモデル変形
	void UpdateGravity(XMFLOAT3 gravityPoint);

	//移動
	void Move();

	//セッター
	void SetPosition(XMFLOAT3 pos) { position = pos; }
	void SetScale(XMFLOAT3 sca) { scale.x = sca.x/2; scale.y = sca.y / 2;scale.z = sca.z / 2;}
	void SetRotation(XMFLOAT3 rot) { rotation = rot; }

	//ゲッター
	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }

public:
	//頂点データ配列
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos;	//座標
		XMFLOAT3 normal;	//法線ベクトル
		XMFLOAT3 uv;	//uv座標
		VertexPosNormalUv* parent = nullptr;	//uv座標
	};
	//頂点データ配列
	vector<VertexPosNormalUv>vertices;
	vector<VertexPosNormalUv>vertices2;
	//頂点インデックス配列
	vector<unsigned short>indices;
private:
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//インデックスバッファ
	ComPtr<ID3D12Resource>indexBuff;
	//テクスチャバッファ
	ComPtr<ID3D12Resource>texBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;
	//アンビエント係数
	XMFLOAT3 ambient = { 1,1,1 };
	//ディフューズ係数
	XMFLOAT3 diffuse = { 1,1,1 };
	//テクスチャメタデータ
	TexMetadata metadata = {};
	//スクラッチイメージ
	ScracthImage scratchImg = {};
	//定数バッファ
	ComPtr<ID3D12Resource>constBuffTransform;
	//ルートシグネチャ
	static ComPtr<ID3D12RootSignature>rootsignature;
	//パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState>pipelinestate;
	//画像用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> srvHeap;
	const size_t textureWidth = 256;
	const size_t textureHeight = 256;
	const size_t imageDataCount = textureWidth * textureHeight;
	XMFLOAT4* imageData = new XMFLOAT4[imageDataCount];
	//テクスチャーのGPUのハンドル
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle;
private:
	//頂点生成用変数
	vector<VertexPosNormalUv> v;
	vector<VertexPosNormalUv> v2;
	vector<VertexPosNormalUv> v3;
	const float fineSize = fine;	//球体の細かさ
	float angleX, angleY;
	float oneAngle = (2 * 3.14159265359) / fineSize;

	//変形行列
	//ローカルスケール
	XMFLOAT3 scale = { 1,1,1 };
	//X,Y,Z軸回りのローカル行列
	XMFLOAT3 rotation = { 0,0,0 };
	//ローカル座標
	XMFLOAT3 position = { 0,0,0 };
	//ローカルワールド変換行列
	XMMATRIX matWorld;
	//モデル
	/*MetaballModel* model = nullptr;*/

	//移動用変数
	XMFLOAT3 fallVelocity = { 0,0,0 };
	float fallTimer = 0.0f;

	bool groundFlag = false;
};
