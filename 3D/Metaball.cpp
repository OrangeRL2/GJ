#include "Metaball.h"
#include "math.h"

#define PI 3.14159265359
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;
using namespace DirectX;

ComPtr<ID3D12RootSignature>Metaball::rootsignature;
ComPtr<ID3D12PipelineState>Metaball::pipelinestate;

ID3D12Device* Metaball::device = nullptr;
Camera* Metaball::camera = nullptr;
Input* Metaball::input = nullptr;

void Metaball::CreateGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;    // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	assert(device);

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"VertexShader.hlsl",     // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"PixelShader.hlsl",   // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",    // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	/*gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;*/
	gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;    // RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;    // 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[2];
	// CBV（座標変換行列用）
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	// SRV（テクスチャ）
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(rootsignature.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(pipelinestate.ReleaseAndGetAddressOf()));
	if (FAILED(result)) { assert(0); }
}

void Metaball::Initialize()
{
	HRESULT result;

	CreateBuffers();
	//定数バッファの生成
	CD3DX12_HEAP_PROPERTIES v1 = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC v2 = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataTransform) + 0xff) & ~0xff);
	result = device->CreateCommittedResource(
		&v1,
		D3D12_HEAP_FLAG_NONE,
		&v2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffTransform)
	);
}

void Metaball::Update()
{
	//ASDWでXY、ZXでZ軸を移動
	/*if (input->PushKey(DIK_D))
	{
		position.x += 0.55 * 1/4 * 2.3f;
		position.z -= 0.55 * 1/6 * 2.3f;
	}
	if (input->PushKey(DIK_A))
	{
		position.x -= 0.55 * 1 / 4 * 2.3f;
		position.z += 0.55 * 1 / 6 * 2.3f;
	}*/
	/*if (input->PushKey(DIK_W))
	{
		position.y += 0.55;
	}
	if (input->PushKey(DIK_S))
	{
		position.y -= 0.55;
	}*/
	/*if (input->PushKey(DIK_W))
	{
		position.x += 0.55 * 1 / 4 * 2.3f;
		position.z += 0.55 * 1 / 3 * 2.3f;
	}
	if (input->PushKey(DIK_S))
	{
		position.x -= 0.55 * 1 / 4 * 2.3f;
		position.z -= 0.55 * 1 / 3 * 2.3f;
	}*/

	//if (input->PushKey(DIK_D))
	//{
	//	position.x += 0.55;
	//}
	//if (input->PushKey(DIK_A))
	//{
	//	position.x -= 0.55;
	//}
	///*if (input->PushKey(DIK_W))
	//{
	//	position.y += 0.55;
	//}
	//if (input->PushKey(DIK_S))
	//{
	//	position.y -= 0.55;
	//}*/
	//if (input->PushKey(DIK_W))
	//{
	//	position.z += 0.55;
	//}
	//if (input->PushKey(DIK_S))
	//{
	//	position.z -= 0.55;
	//}

	////スペースキーでジャンプ
	//if(input->PushKey(DIK_SPACE) && groundFlag == true)
	//{
	//	//接地フラグをfalseに
	//	fallTimer = -1;
	//	groundFlag = false;
	//}

	////地面に接していない場合の落下処理
	//if (groundFlag == false)
	//{
	//	//60フレームでタイマーを1進める
	//	fallTimer += 3.0f / 60.0f;
	//	//落下ベクトル計算
	//	float v = GAcceleration * fallTimer;
	//	fallVelocity.y = -(GAcceleration * fallTimer);
	//	//座標に加算
	//	position.x += fallVelocity.x;
	//	position.y += fallVelocity.y;
	//	position.z += fallVelocity.z;
	//}

	XMMATRIX matScale, matRot, matTrans;

	//スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(rotation.z);
	matRot *= XMMatrixRotationX(rotation.x);
	matRot *= XMMatrixRotationY(rotation.y);
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	//ワールド行列の生成
	matWorld = XMMatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//ビュープロジェクション行列
	const XMMATRIX& matViewProjection = camera->GetMatViewProjection();
	//カメラ座標
	const XMFLOAT3& cameraPos = camera->GetEye();

	HRESULT result;
	//定数バッファへデータ転送
	ConstBufferDataTransform* constMap = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result))
	{
		constMap->viewproj = matViewProjection;
		constMap->world = matWorld;
		constMap->cameraPos = cameraPos;
		constBuffTransform->Unmap(0, nullptr);
	}

	groundFlag = false;
}

void Metaball::CreateBuffers()
{
	HRESULT result;

	//頂点、頂点生成用データ、インデックスのサイズ設定
	vertices.resize(fine2);
	vertices2.resize(fine2);
	v.resize(fine2);
	v2.resize(fine4);
	v3.resize(fine4);
	indices.resize(fine3);

	//頂点データ生成
	CreateVertex();

	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv)) * vertices.size();

	//頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};	//ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//CPUへの転送用
	//リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB;	//頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = device->CreateCommittedResource(
		&heapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);
	assert(SUCCEEDED(result));

	//頂点バッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];	//座標をコピー
	}
	//つながりを解除
	vertBuff->Unmap(0, nullptr);

	//頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	//頂点インデックス全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	//インデックスバッファ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	//CPUへの転送用
	//リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB;	//インデックス情報が入る分のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//インデックスバッファ生成
	result = device->CreateCommittedResource(
		&heapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc,	//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff)
	);

	//インデックスバッファをマッピング
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	//全インデックスに対して
	for (int i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i];	//インデックスをコピー
	}
	//マッピング解除
	indexBuff->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;


	//テクスチャ設定
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);	//デスクリプタのサイズ
	imageData = new XMFLOAT4[imageDataCount];
	//全ピクセルを初期化
	for (size_t i = 0; i < imageDataCount; i++)
	{
		imageData[i].x = 1.0f;
		imageData[i].y = 0.0f;
		imageData[i].z = 0.0f;
		imageData[i].w = 1.0f;
	}

	//テクスチャバッファ設定
	//ヒープ設定
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty =
		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	//リソース設定
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureResourceDesc.Width = textureWidth;	//幅
	textureResourceDesc.Height = textureHeight;	//高さ
	textureResourceDesc.DepthOrArraySize = 1;
	textureResourceDesc.MipLevels = 1;
	textureResourceDesc.SampleDesc.Count = 1;
	//テクスチャバッファの生成
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);
	//テクスチャバッファにデータ転送
	result = texBuff->WriteToSubresource(
		0,
		nullptr,
		imageData,
		sizeof(XMFLOAT4) * textureWidth,
		sizeof(XMFLOAT4) * imageDataCount
	);

	//デスクリプタヒープ生成
	//SRVの最大個数
	const size_t kMaxSRVCount = 2056;

	//デスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見えるように
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	//設定をもとにSRV用デスクリプタヒープを生成
	result = device->CreateDescriptorHeap(
		&srvHeapDesc,
		IID_PPV_ARGS(&srvHeap)
	);
	assert(SUCCEEDED(result));

	//SRVヒープの先頭ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();
	//ここまで

	//テクスチャーの番号が0以降の場合ハンドルを進める
	/*if (texNum > 0)
	{
		srvHandle.ptr += (incrementSize * texNum);
	}*/
	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//設定構造体
	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.Shader4ComponentMapping =
		D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;
	//ハンドルの指す位置にシェーダリソースビュー作成
	device->CreateShaderResourceView(
		texBuff.Get(),
		&srvDesc,
		srvHeap->GetCPUDescriptorHandleForHeapStart()
	);
}

void Metaball::CreateVertex()
{
	InitializeVertex();

	//法線の計算
	for (int i = 0; i < fine3 / 3; i++)
	{//三角形1つごとに計算していく
		//三角形のインデックスを取り出して、一時的な変数に入れる
		unsigned short indices0 = indices[i * 3 + 0];
		unsigned short indices1 = indices[i * 3 + 1];
		unsigned short indices2 = indices[i * 3 + 2];
		//三角形を構成する頂点座標をベクトルに代入
		XMVECTOR p0 = XMLoadFloat3(&vertices[indices0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[indices1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[indices2].pos);
		//p0→p1ベクトル、p0→p2ベクトルを計算　(ベクトルの減算)
		XMVECTOR v1 = DirectX::XMVectorSubtract(p1, p0);
		XMVECTOR v2 = DirectX::XMVectorSubtract(p2, p0);
		//外積は両方から垂直なベクトル
		XMVECTOR normal = DirectX::XMVector3Cross(v1, v2);
		//正規化
		normal = DirectX::XMVector3Normalize(normal);
		//求めた法線を頂点データに代入
		DirectX::XMStoreFloat3(&vertices[indices0].normal, normal);
		DirectX::XMStoreFloat3(&vertices[indices1].normal, normal);
		DirectX::XMStoreFloat3(&vertices[indices2].normal, normal);
	}
}

void Metaball::InitializeVertex()
{
	//球体一つの基礎サイズ
	XMFLOAT3 size = { 1.0f,1.0f,1.0f };

	//頂点データ
	float x, y, z;
	for (int i = 0; i < fine2; i++)
	{
		if (i == 0 || i % 4 == 0)
		{
			if (i == 0)
			{
				angleX = 0;
			}
			if (i == 0 || i % (fine * 4) == 0)
			{
				angleY = (2 * PI) * ((float)(i + fine * 4) / (float)(fine * fine * 4));
			}
			else
			{
				angleY += oneAngle;
			}

			v[i].pos.x = size.x * cos(angleX) * sin(angleY);
			v[i].pos.y = size.y * cos(angleY);
			v[i].pos.z = size.z * sin(angleX) * sin(angleY);

		}

		if (i == 1 || i % 4 == 1)
		{
			angleY -= oneAngle;

			v[i].pos.x = size.x * cos(angleX) * sin(angleY);
			v[i].pos.y = size.y * cos(angleY);
			v[i].pos.z = size.z * sin(angleX) * sin(angleY);

		}
		if (i == 2 || i % 4 == 2)
		{
			angleX += oneAngle;
			angleY += oneAngle;

			v[i].pos.x = size.x * cos(angleX) * sin(angleY);
			v[i].pos.y = size.y * cos(angleY);
			v[i].pos.z = size.z * sin(angleX) * sin(angleY);

		}
		if (i == 3 || i % 4 == 3)
		{
			angleY -= oneAngle;

			v[i].pos.x = size.x * cos(angleX) * sin(angleY);
			v[i].pos.y = size.y * cos(angleY);
			v[i].pos.z = size.z * sin(angleX) * sin(angleY);

		}
	}

	unsigned short in[fine3];
	for (int i = 0; i < fine3; i++)
	{
		double num_ = ((i / 6) * 6) * 2 / 3;
		if (i == 0 || i % 6 == 0) { in[i] = num_; }
		if (i == 1 || i == 4 || i % 6 == 1 || i % 6 == 4) { in[i] = num_ + 1; }
		if (i == 2 || i == 3 || i % 6 == 2 || i % 6 == 3) { in[i] = num_ + 2; }
		if (i == 5 || i % 6 == 5) { in[i] = num_ + 3; }
	}

	angleY = 0;
	angleX = 0;
	//頂点データ	上から順番に割り当てる
	for (int i = 0; i < fine4; i++)
	{
		if (i == 0 || i % fine == 0)
		{
			angleX = 0;
		}
		else
		{
			angleX += oneAngle;
		}
		if (i == 0)
		{
			angleY = 0;
		}
		else if (i != 0 && i >= fine && i % fine == 0)
		{
			angleY = (2 * PI) * ((float)(i) / (float)(fine * fine));
		}
		v2[i].pos.x = size.x * cos(angleX) * sin(angleY);
		v2[i].pos.y = size.y * cos(angleY);
		v2[i].pos.z = size.z * sin(angleX) * sin(angleY);
		v3[i].pos.x = v2[i].pos.x;
		v3[i].pos.y = v2[i].pos.y;
		v3[i].pos.z = v2[i].pos.z;
	}


	for (int i = 0; i < fine2; i++)
	{
		for (int j = 0; j < fine4; j++)
		{
			//uv(0.0f,0.0f)
			if (i == 1 || i % 4 == 1)
			{
				if (i == 1)
				{
					v[i].parent = &v2[0];
				}
				else if (i % 4 == 1 && i != 1)
				{
					v[i].parent = &v2[i / 4];
				}
			}
			//uv(1.0f,0.0f)
			if (i == 3 || i % 4 == 3)
			{
				if (i == 3)
				{
					v[i].parent = &v2[1];
				}
				else if (i % 4 == 3)
				{
					if (i % (fine * 4) != (fine * 4) - 1 && i != (fine * 4) - 1)
					{
						v[i].parent = &v2[(i + 1) / 4];
					}
					if (i % (fine * 4) == (fine * 4) - 1 || i == (fine * 4) - 1)
					{
						v[i].parent = &v2[(i / 4) - (fine - 1)];
					}
				}
			}

			//uv(0.0f,1.0f)
			if (i == 0 || i % 4 == 0)
			{
				if (i == 0)
				{
					v[i].parent = &v2[fine];
				}
				else if (i % 4 == 0 && i != 0)
				{
					v[i].parent = &v2[(i / 4) + fine];
				}
			}

			if (i == 2 || i % 4 == 2)
			{
				if (i == 2)
				{
					v[i].parent = &v2[fine + 1];
				}
				else if (i % 4 == 2)
				{
					if (i % (fine * 4) != (fine * 4) - 2 && i != (fine * 4) - 2)
					{
						v[i].parent = &v2[(i + 2) / 4 + fine];
					}
					if (i % (fine * 4) == (fine * 4) - 2 || i == (fine * 4) - 2)
					{
						v[i].parent = &v2[(i / 4) - (fine - 1) + fine];
					}
				}
			}
		}
	}

	//頂点座標、インデックスデータを代入
	for (int i = 0; i < fine2; i++)
	{
		vertices[i] = v[i];
		vertices2[i] = v[i];
	}

	for (int i = 0; i < fine3; i++)
	{
		indices[i] = in[i];
	}
}

void Metaball::SetImageData(XMFLOAT4 color)
{
	HRESULT result;
	for (size_t i = 0; i < imageDataCount; i++)
	{
		imageData[i].x = color.x;
		imageData[i].y = color.y;
		imageData[i].z = color.z;
		imageData[i].w = color.w;
	}
	//テクスチャバッファにデータ転送
	result = texBuff->WriteToSubresource(
		0,
		nullptr,
		imageData,
		sizeof(XMFLOAT4) * textureWidth,
		sizeof(XMFLOAT4) * imageDataCount
	);
}

void Metaball::UpdateCollision(Collision* collision)
{
	if (collision->Update(position, scale) == 1)
	{
		//接地フラグを立てる
		groundFlag = true;
		//自由落下Tの値をリセット
		fallTimer = 0;
		//落下ベクトルをリセット
		fallVelocity.y = 0;
	}

	//めり込まなくなりまで加算
	while (collision->Update(position, scale) == 1)
	{
		position.y += 0.02f;
		if (collision->Update(position, scale) == 0)
		{
			break;
		}
	}

	/*position.y -= 0.2f;
	if (collision->Update(position, scale) == 0)
	{
		groundFlag = false;
	}
	position.y += 0.1f;*/
}

void Metaball::UpdateVertex()
{
	//-----この上に頂点の更新処理を書く-----

	HRESULT result;
	//頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv)) * vertices.size();

	//頂点バッファへのデータ転送
	//GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	//全頂点に対して
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];	//座標をコピー
	}
	//つながりを解除
	vertBuff->Unmap(0, nullptr);
}

void Metaball::UpdateGravity(XMFLOAT3 gravityPoint)
{
	//重力ポイントをワールド座標の原点に移動
	XMFLOAT3 graPoint(gravityPoint.x - position.x,
		gravityPoint.y - position.y,
		gravityPoint.z - position.z);

	//頂点の質量と各頂点の質量を仮に定義
	float graPointWeight = 1.0f;
	float vertexWeight = 1.0f;

	float x, y, z, length, vecX,vecY, vecZ;


	for (int i = 0; i < vertices.size(); i++)
	{
		//変数の値を計算
		x = (abs(vertices[i].pos.x - graPoint.x)) * (abs(vertices[i].pos.x - graPoint.x));
		y = (abs(vertices[i].pos.y - graPoint.y)) * (abs(vertices[i].pos.y - graPoint.y));
		z = (abs(vertices[i].pos.z - graPoint.z)) * (abs(vertices[i].pos.z - graPoint.z));
		length = sqrt(x + y + z);
		vecX = abs(vertices[i].pos.x - graPoint.x) / length;
		vecY = abs(vertices[i].pos.y - graPoint.y) / length;
		vecZ = abs(vertices[i].pos.z - graPoint.z) / length;

		//頂点x座標の計算
		if (vertices2[i].pos.x - graPoint.x < 0)
		{
			vertices[i].pos.x = vertices2[i].pos.x + ((vertexWeight * graPointWeight) / (length * length)) * G * vecX;
		}
		else
		{
			vertices[i].pos.x = vertices2[i].pos.x - ((vertexWeight * graPointWeight) / (length * length)) * G * vecX;
		}

		//頂点y座標の計算
		if (vertices2[i].pos.y - graPoint.y < 0)
		{
			vertices[i].pos.y = vertices2[i].pos.y + ((vertexWeight * graPointWeight) / (length * length)) * G * vecY;
		}
		else
		{
			vertices[i].pos.y = vertices2[i].pos.y - ((vertexWeight * graPointWeight) / (length * length)) * G * vecY;
		}

		//頂点z座標の計算
		if (vertices2[i].pos.z - graPoint.z < 0)
		{
			vertices[i].pos.z = vertices2[i].pos.z + ((vertexWeight * graPointWeight) / (length * length)) * G * vecZ;
		}
		else
		{
			vertices[i].pos.z = vertices2[i].pos.z - ((vertexWeight * graPointWeight) / (length * length)) * G * vecZ;
		}
	}
}

void Metaball::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//パイプラインステートの設定
	cmdList->SetPipelineState(pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(rootsignature.Get());
	//プリミティブ形状の設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetGPUVirtualAddress());

	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &vbView);
	//インデックスバッファをセット
	cmdList->IASetIndexBuffer(&ibView);

	//デスクリプタヒープのセット
	ID3D12DescriptorHeap* ppHeaps[] = { srvHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, srvHeap->GetGPUDescriptorHandleForHeapStart());

	//描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}
