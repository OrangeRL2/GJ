#include "Player.h"
#include "Vector3.h"
#include "math.h"

#define PI 3.14159265359
#define G 6.674	//万有引力定数
#define GAcceleration 9.80665 * 1/10	//重力加速度

ID3D12Device* Player::device = nullptr;
Camera* Player::camera = nullptr;
Input* Player::input = nullptr;
DXInput* Player::dxInput = nullptr;

void Player::Initialize()
{
	//オブジェクト初期化
	object0 = new FbxObject3D;
	object0->Initialize();
	object0->SetModel(model);
	object0->PlayAnimation();


	//ヒットボックスの設定
	CubeObject3D* newCubeObject = new CubeObject3D();
	newCubeObject->Initialize();
	cubeObject0.reset(newCubeObject);
	cubeObject0->SetModel(cubeModel);


	hitboxPosition0.x = position0.x;
	hitboxPosition0.y = position0.y ;
	hitboxPosition0.z = position0.z;
	hitboxRotation0 = { 0.0f,0.0f,0.0f };
	hitboxScale0 = { 3.0f,12.0f,3.0f };

}

void Player::Update()
{
	startTimer += 1;

	if (startTimer <= 120)
	{
		changeFlag = false;
		playerState = front;
	}

	//移動更新
	UpdateMove();
	//当たり判定更新
	UpdateCollision();


	position0.x += fallVelocity0.x;
	position0.y += fallVelocity0.y;
	position0.z += fallVelocity0.z;

	position0.x += velocity0.x;
	position0.y += velocity0.y;
	position0.z += velocity0.z;


	//hitboxの座標調整
	hitboxPosition0.x = position0.x;
	hitboxPosition0.y = position0.y + 6.0f;
	hitboxPosition0.z = position0.z;


	preGroundFlag0 = groundFlag0;


	//オブジェクト更新
	object0->SetPosition(position0);
	object0->SetScale(scale0);
	object0->SetRotation(rotation0);
	object0->Update();


	cubeObject0->SetPosition(hitboxPosition0);
	cubeObject0->SetScale(hitboxScale0);
	cubeObject0->SetRotation(hitboxRotation0);
	cubeObject0->Update();

}
void Player::UpdateMove()
{
	if (input->PushKey(DIK_W)) {
		position0.x += 0.5f;
	}
	if (input->PushKey(DIK_S)) {
		position0.x -= 0.5f;
	}
	if (input->PushKey(DIK_A)) {
		position0.z += 0.5f;
	}
	if (input->PushKey(DIK_D)) {
		position0.z -= 0.5f;
	}

	if (input->PushKey(DIK_SPACE) && groundFlag0 == true && playerState == front)
	{
		//接地フラグをfalseに
		fallTimer0 = -1.1f;
		groundFlag0 = false;
	}

	if (groundFlag0 == true && playerState == front && fallTimer0 > 0)
	{
		//接地フラグをfalseに
		groundFlag0 = false;
	}

	//地面に接していない場合の落下処理(表のオブジェクト)
	//60フレームでタイマーを1進める
	fallTimer0 += 3.0f / 60.0f;
	//落下ベクトル計算
	fallVelocity0.y = -(GAcceleration * fallTimer0);
	//60フレームでタイマーを1進める
	fallTimer1 += 3.0f / 60.0f;

}
void Player::Draw(ID3D12GraphicsCommandList* cmdList)
{
	//object0->Draw(cmdList);
	cubeObject0->Draw(cmdList);
	/*cubeObject0->Draw(cmdList);
	cubeObject1->Draw(cmdList);*/
}

void Player::SetTitle()
{
	groundFlag0 = false;

	changeFlag = false;
	keyFlag = false;
	goalFlag = false;

	fallTimer0 = 0.0f;
	fallTimer1 = 0.0f;



	/*playerState = back;*/
	startTimer = 0;

	SetPosition0({ 0,10,0 });


	camera->GetMode() == Camera::forward;
}

void Player::UpdateCollision()
{
	//ヒットボックスの最大値、最小値を定義
	DirectX::XMFLOAT3 Min0 = { hitboxPosition0.x - hitboxScale0.x,
		hitboxPosition0.y - hitboxScale0.y, hitboxPosition0.z - hitboxScale0.z };
	DirectX::XMFLOAT3 Max0 = { hitboxPosition0.x + hitboxScale0.x,
		hitboxPosition0.y + hitboxScale0.y, hitboxPosition0.z + hitboxScale0.z };

	//-----------オブジェクトとの当たり判定---------
	for (std::unique_ptr<Collision>& collision : collisionsObstacle)
	{
#pragma region 表
		if (collision->GetPosition().y >= 0)
		{
			XMFLOAT3 h = { hitboxPosition0.x,hitboxPosition0.y + 3,hitboxPosition0.z };
		// Side collision only occurs if the player is not below the obstacle
// Only process side collisions if the player is above the obstacle or close to it
			// Allow side collisions only if the player is at or above the obstacle's minimum Y
			/*if (position0.y >= collision->GetMin().y) {*/
				// Left side collision
				if (-2 + velocity0.x <= Max0.x - collision->GetMin().x && Max0.x - collision->GetMin().x <= 2 + velocity0.x) {
					while (collision->Update(h, hitboxScale0) == 1) {
						position0.x -= 0.0002f;  // Push left
						hitboxPosition0.x -= 0.0002f;
						h.x -= 0.0002f;
					}
				}

				// Right side collision
				if (-2 + velocity0.x <= Min0.x - collision->GetMax().x && Min0.x - collision->GetMax().x <= 2 + velocity0.x) {
					while (collision->Update(h, hitboxScale0) == 1) {
						position0.x += 0.0002f;  // Push right
						hitboxPosition0.x += 0.0002f;
						h.x += 0.0002f;
					}
				}

				// Front side collision (Z axis)
				if (-2 + velocity0.z <= Max0.z - collision->GetMin().z && Max0.z - collision->GetMin().z <= 2 + velocity0.z) {
					while (collision->Update(h, hitboxScale0) == 1) {
						position0.z -= 0.0002f;  // Push back
						hitboxPosition0.z -= 0.0002f;
						h.z -= 0.0002f;
					}
				}

				// Back side collision (Z axis)
				if (-2 + velocity0.z <= Min0.z - collision->GetMax().z && Min0.z - collision->GetMax().z <= 2 + velocity0.z) {
					while (collision->Update(h, hitboxScale0) == 1) {
						position0.z += 0.0002f;  // Push forward
						hitboxPosition0.z += 0.0002f;
						h.z += 0.0002f;
					}
				}
			//}

			if (position0.y < collision->GetMin().y - 10.0f) {
				// Player is below the obstacle, prevent pushing upwards
				while (collision->Update(h, hitboxScale0) == 1) {
					position0.y -= 0.002f; // Push down instead of up
					hitboxPosition0.y -= 0.002f;
					h.y -= 0.002f;
				}
			}
			if (position0.y > collision->GetMin().y) {
			if (collision->Update(h, hitboxScale0) == 1) {
				// Handle normal collision if the player is above or at the obstacle
				groundFlag0 = true;
				fallTimer0 = 0;
				fallVelocity0.y = 0;

				while (collision->Update(h, hitboxScale0) == 1) {
					position0.y += 0.002f; // Push up if necessary
					hitboxPosition0.y += 0.002f;
					h.y += 0.002f;
				}
			}
			}
		}

#pragma endregion

	}
#pragma region 床
	//-----------床との当たり判定-----------------
	for (std::unique_ptr<Collision>& collision : collisionsFloor)
	{
		//表のオブジェクト
		if (hitboxPosition0.y >= collision->GetMax().y)
		{
			if (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				//接地フラグを立てる
				groundFlag0 = true;
				//自由落下Tの値をリセット
				fallTimer0 = 0;
				//落下ベクトルをリセット
				fallVelocity0.y = 0;
			}
			//めり込まなくなりまで加算
			while (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				position0.y += 0.002f;
				hitboxPosition0.y += 0.002f;
			}
		}

		//表
		//左からぶつかる場合
		if (-2 + velocity0.x <= Max0.x - collision->GetMin().x && Max0.x - collision->GetMin().x <= 2 + velocity0.x)
		{
			while (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				position0.x -= 0.0002f;
				hitboxPosition0.x -= 0.0002f;

			}
		}
		//右からぶつかる場合
		if (-2 + velocity0.x <= Min0.x - collision->GetMax().x && Min0.x - collision->GetMax().x <= 2 + velocity0.x)
		{
			while (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				position0.x += 0.0002f;
				hitboxPosition0.x += 0.0002f;
			}
		}
		//前からぶつかる場合
		if (-2 + velocity0.z <= Max0.z - collision->GetMin().z && Max0.z - collision->GetMin().z <= 2 + velocity0.z)
		{
			while (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				position0.z -= 0.0002f;
				hitboxPosition0.z -= 0.0002f;
			}
		}
		//後ろからぶつかる場合
		if (-2 + velocity0.z <= Min0.z - collision->GetMax().z && Min0.z - collision->GetMax().z <= 2 + velocity0.z)
		{
			while (collision->Update(hitboxPosition0, hitboxScale0) == 1)
			{
				position0.z += 0.0002f;
				hitboxPosition0.z += 0.0002f;
			}
		}

#pragma endregion
	}
}


void Player::SetCollisionObstacle(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale)
{
	std::unique_ptr<Collision>newCollision = std::make_unique<Collision>();
	newCollision->SetObject(position, scale);
	collisionsObstacle.push_back(std::move(newCollision));
}


void Player::ClearCollision()
{
	collisionsObstacle.clear();
	collisionsFloor.clear();
}

void Player::SetCollisionFloor(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale)
{
	std::unique_ptr<Collision>newCollision = std::make_unique<Collision>();
	newCollision->SetObject(position, scale);
	collisionsFloor.push_back(std::move(newCollision));
}