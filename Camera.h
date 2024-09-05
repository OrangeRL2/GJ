#pragma once
#include "DirectXMath.h"
#include "Input.h"
#include "DXInput.h"
#include "WinApp.h"
//#include "Player.h"
#define PI 3.14159265359

using namespace DirectX;

class Camera
{
public:
	static enum Mode
	{
		forward,
		right,
		backward,
		left,
	};
	static enum ApproachMode
	{
		Max,
		Min,
	};

public:
	//シングルトンインスタンス
	static Camera* GetInstance();
	static void SetInput(Input* input) { Camera::input = input; }
	static void SetDXInput(DXInput* dxInput) { Camera::dxInput = dxInput; }
	//インストラクタ デストラクタ
	Camera();
	~Camera();
	//初期化
	void Initialize();
	//更新
	void Update();
	void SetCamera();
	

	//タイトル、ステージセレクトの挙動
	void StageSelect(DirectX::XMFLOAT3 pos, int playerState);
	//プレイヤーを追尾する処理(プレイヤーの座標,プレイヤーの状態)
	void PlayerAim(DirectX::XMFLOAT3 pos0, DirectX::XMFLOAT3 pos1, int playerState);

	//ゲッターセッター
	void SetTarget(XMFLOAT3 pos);
	void SetEye(XMFLOAT3 pos);
	XMFLOAT3 GetEye() { return eye_; };
	XMFLOAT3 GetTraget() { return target_; };
	XMFLOAT3 GetUp() { return up_; };
	XMMATRIX GetMatProjection() { return matProjection_; };
	XMMATRIX GetMatView() { return matView_; };
	XMMATRIX GetMatViewProjection() { return matView_ * matProjection_; };

	Mode GetMode() { return mode; }
private:
	//入力
	//キーボード
	static Input* input;
	//コントローラー
	static DXInput* dxInput;
	//射影変換
	XMMATRIX matProjection_;
	//ビュー変換行列
	XMMATRIX matView_;
	XMFLOAT3 eye_ = { 0, 20, -100 };
	XMFLOAT3 target_ = { 0, 0, 0 };
	XMFLOAT3 up_ = { 0, 1, 0 };

	//プレイヤー追尾処理用の変数
	//前のフレームのプレイヤーの状態を保存する変数
	int prePlayerState;
	//裏表切り替わっている瞬間に立てるフラグ
	bool changeMode = false;
	//裏表を切り替えるタイマー
	float changeModeTimer = 0.0f;
	//裏表を切り替えるのにかかる時間
	float changeModeTime = 60.0f;
	//プレイヤー追尾状態の際のY座標
	float changePosY = 10.0f;

	//プレイヤー追尾状態の際のY座標
	float eyeTitleY = 50.0f;
	float targetTitlePosY = 20.0f;

	//視点を変える用の変数
	//コントローラー保存用
	float keyR = 0;
	float preKeyR = 0;
	float keyL = 0;
	float preKeyL = 0;
	//後ろ
	float bKeyR = 0;
	float preBKeyR = 0;
	float bKeyL = 0;
	float preBKeyL = 0;
	//角度
	float changeRotX;
	float changeRotZ;
	float changeRot = (float)PI * 1/2;
	//カメラの入れ替えにかけるフレーム
	float changeTime = 30.0f;
	float changeTimer = 0.0f;
	bool changeModeR = false;
	bool changeModeL = false;
	//カメラの接近に使うフラグ
	bool approachFlag = false;
	bool approachMin = false;
	bool approachMax = false;
	//プレイヤーとカメラの距離の最大値
	float maxPlayerDistance = -100.0f;
	//プレイヤーとカメラの距離の最小値
	float minPlayerDistance = -40.0f;
	//プレイヤーとカメラの距離
	float playerDistance = maxPlayerDistance;
	//プレイヤーの接近にかかるフレーム
	float distanceTime = 30.0f;
	//接近に使うタイマー
	float distanceTimer = 0.0f;

	//カメラの位置
	Mode mode = forward;
	ApproachMode approachMode = Max;
};
