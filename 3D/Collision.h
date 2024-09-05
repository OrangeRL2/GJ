#pragma once
#include "DirectXMath.h"

class Collision
{
private:
	//当たり判定の計算に必要な構造体
	struct CubeVertex
	{
		DirectX::XMFLOAT3 c;	//中心
		//DirectX::XMFLOAT3 v[8];	//各頂点の座標
		DirectX::XMFLOAT3 min;	//-x,-y,-z
		DirectX::XMFLOAT3 max;	//x,y,z
	};
	//当たり判定の計算に必要な構造体
	struct SphereVertex
	{
		DirectX::XMFLOAT3 c;	//中心
		DirectX::XMFLOAT3 r;	//半径
		DirectX::XMFLOAT3 min;
		DirectX::XMFLOAT3 max;
	};
public:
	//衝突判定を行うオブジェクトをセット(直方体と球)
	void SetObject(DirectX::XMFLOAT3 cubePos, DirectX::XMFLOAT3 cubeScale);
	//更新
	bool Update(DirectX::XMFLOAT3 spherePos, DirectX::XMFLOAT3 sphereScale);

	//ゲッター
	DirectX::XMFLOAT3 GetPosition() { return cubeVertex1.c; }
	DirectX::XMFLOAT3 GetMin() { return cubeVertex1.min; }
	DirectX::XMFLOAT3 GetMax() { return cubeVertex1.max; }
private:
	//当たり判定に必要な変数
	CubeVertex cubeVertex1;
	SphereVertex sphereVertex1;

	/*DirectX::XMFLOAT3 cubePos;
	DirectX::XMFLOAT3 cubeScale;*/

	bool hit;
};

