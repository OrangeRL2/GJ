#include "Camera.h"
#include "Math.h"
#include "Math2.h"
//#define PI 3.14159265359

Input* Camera::input = nullptr;
DXInput* Camera::dxInput = nullptr;

Camera* Camera::GetInstance()
{
	static Camera instance;
	return &instance;
}

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	//射影変換
	matProjection_ = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//上下画角45度
		(float)window_width / window_height,//アスペクト比(画面横幅/画面立幅)
		0.1f, 1000.0f						//前端、奥端
	);

	//行列計算
	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));
}


void Camera::Update()
{
	//デバッグ用
	/*if (input->PushKey(DIK_D))
	{
		position.x += 0.55;
	}
	if (input->PushKey(DIK_A))
	{
		position.x -= 0.55;
	}
	if (input->PushKey(DIK_W))
	{
		eye_.y += 0.55;
	}
	if (input->PushKey(DIK_S))
	{
		eye_.y -= 0.55;
	}
	if (input->PushKey(DIK_W))
	{
		position.z += 0.55;
	}
	if (input->PushKey(DIK_S))
	{
		position.z -= 0.55;
	}*/


	matView_ = XMMatrixLookAtLH(XMLoadFloat3(&eye_), XMLoadFloat3(&target_), XMLoadFloat3(&up_));

	//現在のフレームのキーを保存
	preKeyR = keyR;
	preKeyL = keyL;
	preBKeyR = bKeyR;
	preBKeyL = bKeyL;
}
void Camera::StageSelect(DirectX::XMFLOAT3 pos, int playerState)
{
	//前のフレームと現在のフレームで状態が違ったらフラグを立てる
	if (playerState != prePlayerState)
	{
		changeMode = true;
	}

	//changeMode==trueのとき
	if (changeMode == true)
	{
		//タイマー開始
		changeModeTimer += 1.0f;

		/*easeInOutQuart(changeModeTimer / 60.0f);*/
		//表から裏の場合
		if (playerState == 1)
		{
			eye_.y = eyeTitleY - ((eyeTitleY * easeOutQuint(changeModeTimer / 60.0f)) * 2);
			target_.y = targetTitlePosY - ((targetTitlePosY * easeOutQuint(changeModeTimer / 60.0f)) * 2);
			eye_.x = pos.x;
			target_.x = pos.x;
		}
		//裏から表の場合
		if (playerState == 0)
		{
			eye_.y = -eyeTitleY + ((eyeTitleY * easeOutQuint(changeModeTimer / 60.0f)) * 2);
			target_.y = -targetTitlePosY + ((targetTitlePosY * easeOutQuint(changeModeTimer / 60.0f)) * 2);
			eye_.x = pos.x;
			target_.x = pos.x;
		}

		//設定していた時間まで到達したらchangeMode解除
		if (changeModeTimer >= changeModeTime)
		{
			changeMode = false;
			changeModeTimer = 0;
		}

	}

	//プレイヤーの状態によって視点を変更
	if (playerState == 0 && changeMode == false)
	{
		eye_ = { pos.x,eyeTitleY,pos.z - 100 };
		target_ = { pos.x,targetTitlePosY,pos.z + 20 };
	}
	if (playerState == 1 && changeMode == false)
	{
		eye_ = { pos.x,-eyeTitleY,pos.z - 100 };
		target_ = { pos.x,-targetTitlePosY,pos.z + 20 };
	}

	//前のフレームの状態を取得
	prePlayerState = playerState;
}

void Camera::PlayerAim(DirectX::XMFLOAT3 pos0, DirectX::XMFLOAT3 pos1, int playerState)
{
	//コントローラー入力を変数に代入
	if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)keyR = 1;
	else { keyR = 0; }
	if (dxInput->GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)keyL = 1;
	else { keyL = 0; }
	if (dxInput->GamePad.state.Gamepad.bRightTrigger)bKeyR = 1;
	else { bKeyR = 0; }
	if (dxInput->GamePad.state.Gamepad.bLeftTrigger)bKeyL = 1;
	else { bKeyL = 0; }

	//注視点をプレイヤーのx座標に合わせる
	if (playerState == 0)
	{
		target_ = { pos0.x,pos0.y,pos0.z };
	}
	if (playerState == 1)
	{
		target_ = { pos1.x,pos1.y,pos1.z };
	}

#pragma region Y座標関連
	//前のフレームと現在のフレームで状態が違ったらフラグを立てる
	if (playerState != prePlayerState)
	{
		changeMode = true;
	}

	//changeMode==trueのとき
	if (changeMode == true)
	{
		//タイマー開始
		changeModeTimer += 1.0f;

		/*easeInOutQuart(changeModeTimer / 60.0f);*/
		//表から裏の場合
		if (playerState == 1)
		{
			eye_.y = changePosY - ((changePosY * easeOutQuint(changeModeTimer / 60.0f))* 2);
		}
		//裏から表の場合
		if (playerState == 0)
		{
			eye_.y = -changePosY + ((changePosY * easeOutQuint(changeModeTimer / 60.0f))*2);
		}

		//設定していた時間まで到達したらchangeMode解除
		if (changeModeTimer >= changeModeTime)
		{
			changeMode = false;
			changeModeTimer = 0;
		}
		
	}

#pragma endregion

#pragma region プレイヤーとの距離
	//後ろのRボタン
	if (bKeyR == 1 && preBKeyR == 0 && approachFlag == false && approachMode == Max)
	{
		approachFlag = true;
		approachMin = true;
		approachMode = Min;
	}
	//後ろのLボタン
	else if (bKeyL == 1 && preBKeyL == 0 && approachFlag == false && approachMode == Min)
	{
		approachFlag = true;
		approachMax = true;
		approachMode = Max;
	}

	//1フレームあたりの接近量(60/distanceTime)
	float ap = -(maxPlayerDistance - minPlayerDistance) / distanceTime;
	if (approachFlag)
	{
		//タイマー更新
		distanceTimer++;
		if (approachMax)
		{
			playerDistance -= ap;
		}
		if (approachMin) 
		{
			playerDistance += ap;
		}

		if (distanceTimer >= distanceTime)
		{
			distanceTimer = 0;
			approachFlag = false;
			approachMax = false;
			approachMin = false;
		}
	}

#pragma endregoin 

#pragma region XZ関連

	//90度
	float rot = (float)PI * 1 / 2;
	//キー入力で切り替え
	//Rのトリガーキー
	if (keyR == 1 && preKeyR == 0 && changeModeR == false && changeModeL == false)
	{
		changeModeR = true;
	}
	//Lのトリガーキー
	if (keyL == 1 && preKeyL == 0 && changeModeR == false && changeModeL == false)
	{
		changeModeL = true;
		//モードを変更
		/*if (mode == forward)	mode = left;
		else if (mode == left)		mode = backward;
		else if (mode == backward)	mode = right;
		else if (mode == right)		mode = forward;*/
	}

	//1フレームあたりに足す角度
	float rot2 = rot / changeTime;
	//角度を加算する処理
	if (changeModeR || changeModeL)
	{
		//タイマー更新
		changeTimer++;

		//Rの場合
		if (changeModeR)
		{
			changeRot += rot / changeTime;
			//タイマーが半分まで進んだらモード変更
			if (changeTimer == changeTime / 2)
			{
				//モードを変更
				if (mode == forward)	mode = right;
				else if (mode == right)		mode = backward;
				else if (mode == backward)	mode = left;
				else if (mode == left)		mode = forward;
			}
		}
		//Lの場合
		if (changeModeL)
		{
			changeRot -= rot / changeTime;
			//タイマーが半分まで進んだらモード変更
			if (changeTimer == changeTime / 2)
			{
				//モードを変更
				if (mode == forward)	mode = left;
				else if (mode == left)		mode = backward;
				else if (mode == backward)	mode = right;
				else if (mode == right)		mode = forward;
			}
		}

		if (changeTimer >= changeTime)
		{
			changeTimer = 0;
			changeModeR = false;
			changeModeL = false;
		}
	}

	//XZ座標設定
	if (playerState == 0)
	{
		changeRotX = cos(changeRot) * playerDistance + pos0.x;
		changeRotZ = sin(changeRot) * playerDistance + pos0.z;
	}
	if (playerState == 1)
	{
		changeRotX = cos(changeRot) * playerDistance + pos1.x;
		changeRotZ = sin(changeRot) * playerDistance + pos1.z;
	}


#pragma endregion

	//プレイヤーの状態によって視点を変更
	if (playerState == 0 && changeMode == false)
	{
		eye_ = { changeRotX,changePosY,changeRotZ };
	}
	if (playerState == 1 && changeMode == false)
	{
		eye_ = { changeRotX,-changePosY,changeRotZ };
	}

	//前のフレームの状態を取得
	prePlayerState = playerState;
}
//void Camera::PlayerAim(DirectX::XMFLOAT3 pos, Player::State playerState)
//{
//	//注視点をプレイヤーのx座標に合わせる
//	target_ = { pos.x,0,pos.z };
//
//	if (playerState == Player::State::front)
//	{
//		eye_ = { pos.x,10,pos.z - 30 };
//	}
//	if (playerState == Player::State::back)
//	{
//		eye_ = { pos.x,-10,pos.z - 30 };
//	}
//}
void Camera::SetTarget(XMFLOAT3 pos)
{
	target_ = pos;
}

void Camera::SetEye(XMFLOAT3 pos)
{
	eye_ = pos;
}

void Camera::SetCamera()
{
	//カメラの位置
	mode = forward;
	approachMode = Max;
	changeRot = PI * 1 / 2;
}