#include "GameScene.h"
#include "FbxLoader.h"
#include<fstream>
#include "sstream"
#include "stdio.h"
#include "string.h"
#include "vector"

#define PI 3.1415

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	this->dxCommon_ = dxCommon;
	this->input_ = input;

#pragma region カメラ初期化

	//カメラ初期化
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 10, -50 });

#pragma endregion

#pragma region FBX読み込み
	//FBX読み込み
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("Walking", "Resources/red.png");
	stoneModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/dirt.png");
	skydome = FbxLoader::GetInstance()->LoadModelFromFile("skydome", "Resources/skydome.png");


#pragma endregion

#pragma region オブジェクトにデバイスをセット

	//デバイスをセット
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::CreateGraphicsPipeline();

	//キューブの設定
	//デバイスをセット
	CubeObject3D::SetDevice(dxCommon_->GetDevice());
	CubeObject3D::SetCamera(camera_.get());
	CubeObject3D::SetInput(input_);
	CubeObject3D::CreateGraphicsPipeline();

	//メタボール
	//デバイスをセット
	Metaball::SetDevice(dxCommon_->GetDevice());
	Metaball::SetCamera(camera_.get());
	Metaball::CreateGraphicsPipeline();

	//障害物
	Obstacle::SetDevice(dxCommon_->GetDevice());
	Obstacle::SetCamera(camera_.get());

	//床
	Floor::SetDevice(dxCommon_->GetDevice());
	Floor::SetCamera(camera_.get());


#pragma endregion

#pragma region キューブモデルの設定

	//モデルの設定
	//床のモデル
	CubeModel* newCubeModel = new CubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	cubeModel.reset(newCubeModel);
	cubeModel->SetImageData({ 1.0f, 1.0f, 1.0f,0.5f });
	//hitboxのモデル
	CubeModel* newCubeModel1 = new CubeModel();
	newCubeModel1->CreateBuffers(dxCommon_->GetDevice());
	hitBoxModel.reset(newCubeModel1);
	hitBoxModel->SetImageData({ 1.0f,0.5f,0.5f,1.0f });

#pragma endregion

#pragma region プレイヤー初期化

	//プレイヤーセット
	Player::SetDevice(dxCommon_->GetDevice());
	Player::SetCamera(camera_.get());
	Player::SetInput(input_);
	Player::SetDXInput(dxInput);
	//プレイヤー初期化
	Player* newPlayer = new Player();
	newPlayer->SetModel(model1);
	newPlayer->SetCubeModel(hitBoxModel.get());
	newPlayer->Initialize();
	player.reset(newPlayer);

#pragma endregion

#pragma region 床の設定

	//床初期化
	for (int i = 0; i < floorVol; i++)
	{
		std::unique_ptr<Floor>newFloor = std::make_unique<Floor>();
		newFloor->SetCubeModel(cubeModel.get());
		newFloor->Initialize();
		newFloor->SetScale({ 120,0.5,120 });
		newFloor->SetPosition({ 0,0,0 });
		floors.push_back(std::move(newFloor));
	}

#pragma endregion


#pragma region 障害物初期化

	//ファイル読み込み
	std::stringstream obstaclePosList;	//文字列
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/obstacleStage1.csv");
	//ファイルの内容をコピー
	obstaclePosList << file.rdbuf();
	//ファイルを閉じる
	file.close();

	std::string line;

	//ファイルから障害物の場所を読み込み
	while (getline(obstaclePosList, line, '{'))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		std::string word1;
		std::string word2;
		std::string word3;
		//カンマ区切りで先頭文字列を取得
		getline(line_stream, word1, ',');
		getline(line_stream, word2, ',');
		getline(line_stream, word3, ',');
		DirectX::XMFLOAT3 pos(atoi(word1.c_str()), atoi(word2.c_str()), atoi(word3.c_str()));
		obstaclePos.push_back(pos);
	}
	//障害物初期化
	for (int i = 1; i <= obstacleVal; i++)
	{
		std::unique_ptr<Obstacle>newObstacle = std::make_unique<Obstacle>();
		newObstacle->SetModel(stoneModel);
		newObstacle->SetCubeModel(hitBoxModel.get());
		//ファイル読み込みで得た座標を代入
		newObstacle->SetPosition({ 0,0,-10000 });
		newObstacle->SetScale({ 0.11f,0.03f,0.11f});
		newObstacle->Initialize();
		obstacles.push_back(std::move(newObstacle));
	}

#pragma endregion

#pragma region スプライト
	//スプライト初期化処理
	spriteCommon = sprite->SpriteCommonCreate(dxCommon_->GetDevice(), 1280, 720);
	sprite->SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/title.png", dxCommon_->GetDevice());
	titleSprite.SpriteCreate(dxCommon_->GetDevice(), 1280, 720);
	titleSprite.SetTexNumber(0);
	titleSprite.SetPosition(XMFLOAT3(200, 100, 0));
	titleSprite.SetScale(XMFLOAT2(414 * 2.2, 54 * 2.2));
	keySprite.SpriteCreate(dxCommon_->GetDevice(), 1280, 720);
	keySprite.SetTexNumber(1);
	keySprite.SetPosition(XMFLOAT3(1200, 650, 0));
	keySprite.SetScale(XMFLOAT2(64, 64));
#pragma endregion

#pragma region skydome

	//オブジェクト初期化
	skydomeObject = new FbxObject3D;
	skydomeObject->Initialize();
	skydomeObject->SetModel(skydome);

#pragma endregion

	SetTitle();

}

void GameScene::Update()
{
	//コントローラー更新
	dxInput->InputProcess();


	//シーンごとの処理
	(this->*Scene_[scene_])();

	//デバッグ用 キー入力でステージ変更
	if (input_->TriggerKey(DIK_0))stage = Stage::Tutorial;
	if (input_->TriggerKey(DIK_1))stage = Stage::Stage1;
	if (input_->TriggerKey(DIK_2))stage = Stage::Stage2;
	if (input_->TriggerKey(DIK_3))stage = Stage::Stage3;
	if (input_->TriggerKey(DIK_4))stage = Stage::Stage4;
	if (input_->TriggerKey(DIK_5))stage = Stage::Stage5;

	//前のシーンと今のシーンが違かったらリセット処理
	if (scene_ != preScene_)
	{
		//タイトルをセット
		if (scene_ == static_cast<size_t>(Scene::Title))SetTitle();
	}

	//前のステージと現在のステージが違かったらリセット処理
	if (stage != preStage)
	{
		//チュートリアルに移動した場合
		if (stage == Stage::Tutorial)	SetTitle();
		if (stage == Stage::Stage1)		SetStage1();
		if (stage == Stage::Stage2)		SetStage2();
		if (stage == Stage::Stage3)		SetStage3();
		if (stage == Stage::Stage4)		SetStage4();
		if (stage == Stage::Stage5)		SetStage5();
	}

	//前のフレームのシーン取得
	preScene_ = scene_;
	preStage = stage;
}

void GameScene::Draw()
{
	(this->*SceneDraw_[sceneDraw_])();
}

void GameScene::TitleUpdate()
{

	//床更新
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Update();
	}

	//障害物更新
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Update();
	}



	//プレイヤー更新
	player->Update();

	//オブジェクト更新
	skydomeObject->SetPosition(player->GetPosition0());
	//skydomeObject->SetPosition({0.0f,0.0f,0.0f});
	skydomeObject->SetScale({900.0f,900.0f,900.0f});
	skydomeObject->SetRotation({0.0f,0.0f,0.0f});
	skydomeObject->Update();
	//camera_->SetTarget(player->GetPosition0());
	camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
	//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
	camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y+10.0f,camera_->GetEye().z });
	//カメラ更新
	camera_->Update();
}

void GameScene::TitleDraw()
{
	if (input_->TriggerKey(DIK_R)) {
		clearTutoFlag = false;
		clear1Flag = false;
		clear2Flag = false;
		clear3Flag = false;
		clear4Flag = false;
		clear5Flag = false;
		SetTitle();
	}

	//障害物描画
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}

	player->Draw(dxCommon_->GetCommandList());
	skydomeObject->Draw(dxCommon_->GetCommandList());
	//床描画
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i == 0)floor->Draw(dxCommon_->GetCommandList());
		i++;
	}

}

void GameScene::GameUpdate()
{

	//床更新
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Update();
	}

	//障害物更新
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Update();
	}



	//プレイヤー更新
	player->Update();

	//オブジェクト更新
	skydomeObject->SetPosition(player->GetPosition0());
	//skydomeObject->SetPosition({0.0f,0.0f,0.0f});
	skydomeObject->SetScale({ 900.0f,900.0f,900.0f });
	skydomeObject->SetRotation({ 0.0f,0.0f,0.0f });
	skydomeObject->Update();
	//camera_->SetTarget(player->GetPosition0());
	camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
	//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
	camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	//カメラ更新
	camera_->Update();
}

void GameScene::GameDraw()
{
	if (input_->TriggerKey(DIK_R)) {
		clearTutoFlag = false;
		clear1Flag = false;
		clear2Flag = false;
		clear3Flag = false;
		clear4Flag = false;
		clear5Flag = false;
		SetTitle();
	}

	//障害物描画
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}

	player->Draw(dxCommon_->GetCommandList());
	skydomeObject->Draw(dxCommon_->GetCommandList());
	//床描画
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i == 0)floor->Draw(dxCommon_->GetCommandList());
		i++;
	}

}

void GameScene::SetTitle()
{
	player->ClearCollision();
	//camera_->SetCamera();
	
	//床セット
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 100,0.5,100 });
			floor->SetPosition({ 0,0,0 });
		}
		//if (j == 1)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 0,10,0 });
		//}
		//if (j == 2)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		//if (j == 3)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		j++;
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player->SetPosition0({0.0f,5.0f,0.0f});
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial2.csv", tutorialObstacleVal2);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}


}


void GameScene::LoadCsv(const wchar_t* fileName, int obstacleVal)
{
	//ファイル読み込み
	std::stringstream obstaclePosList;	//文字列
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//ファイルを開く
	std::ifstream file;
	file.open(fileName);
	//ファイルの内容をコピー
	obstaclePosList << file.rdbuf();
	//ファイルを閉じる
	file.close();

	std::string line;

	//ファイルから障害物の場所を読み込み
	while (getline(obstaclePosList, line, '{'))
	{
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		std::string word1;
		std::string word2;
		std::string word3;
		//カンマ区切りで先頭文字列を取得
		getline(line_stream, word1, ',');
		getline(line_stream, word2, ',');
		getline(line_stream, word3, ',');
		DirectX::XMFLOAT3 pos(atoi(word1.c_str()), atoi(word2.c_str()), atoi(word3.c_str()));
		obstaclePos.push_back(pos);
	}

	int i = 1;
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		if (i <= obstacleVal)
		{
			obstacle->SetPosition({ obstaclePos[i].x,obstaclePos[i].y,obstaclePos[i].z });
			obstacle->SetHitbox();
		}
		else
		{
			obstacle->SetPosition({ 0,0,-10000 });
			obstacle->SetHitbox();
		}
		i++;
	}
}

void GameScene::DebugLoadCsv(const wchar_t* fileName, int obstacleVal)
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//ファイル読み込み
		std::stringstream obstaclePosList;	//文字列
		std::vector<DirectX::XMFLOAT3>obstaclePos;
		//ファイルを開く
		std::ifstream file;
		file.open(fileName);
		//ファイルの内容をコピー
		obstaclePosList << file.rdbuf();
		//ファイルを閉じる
		file.close();

		std::string line;

		//ファイルから障害物の場所を読み込み
		while (getline(obstaclePosList, line, '{'))
		{
			//1行分の文字列をストリームに変換して解析しやすくする
			std::istringstream line_stream(line);
			std::string word1;
			std::string word2;
			std::string word3;
			//カンマ区切りで先頭文字列を取得
			getline(line_stream, word1, ',');
			getline(line_stream, word2, ',');
			getline(line_stream, word3, ',');
			DirectX::XMFLOAT3 pos(atoi(word1.c_str()), atoi(word2.c_str()), atoi(word3.c_str()));
			obstaclePos.push_back(pos);
		}

		int i = 1;
		for (std::unique_ptr<Obstacle>& obstacle : obstacles)
		{
			if (i <= obstacleVal)
			{
				obstacle->SetPosition({ obstaclePos[i].x,obstaclePos[i].y,obstaclePos[i].z });
			}
			else
			{
				obstacle->SetPosition({ 0,0,-10000 });
			}
			i++;
		}
	}
}

void GameScene::SetStage1()
{
	player->ClearCollision();
	//camera_->SetCamera();

	//床セット
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 100,0.5,100 });
			floor->SetPosition({ 0,0,0 });
		}
		//if (j == 1)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 0,10,0 });
		//}
		//if (j == 2)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		//if (j == 3)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		j++;
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player->SetPosition0({ 0.0f,5.0f,0.0f });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial1.csv", tutorialObstacleVal1);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}


}

void GameScene::SetStage2()
{
	player->ClearCollision();
	//camera_->SetCamera();

	//床セット
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 100,0.5,100 });
			floor->SetPosition({ 0,0,0 });
		}
		//if (j == 1)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 0,10,0 });
		//}
		//if (j == 2)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		//if (j == 3)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		j++;
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player->SetPosition0({ 0.0f,5.0f,0.0f });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial2.csv", tutorialObstacleVal2);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}


}

void GameScene::SetStage3()
{
	player->ClearCollision();
	//camera_->SetCamera();

	//床セット
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 100,0.5,100 });
			floor->SetPosition({ 0,0,0 });
		}
		//if (j == 1)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 0,10,0 });
		//}
		//if (j == 2)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		//if (j == 3)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		j++;
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player->SetPosition0({ 0.0f,5.0f,0.0f });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial3.csv", tutorialObstacleVal3);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}


}

void GameScene::SetStage4()
{
	player->ClearCollision();
	//camera_->SetCamera();

	//床セット
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 100,0.5,100 });
			floor->SetPosition({ 0,0,0 });
		}
		//if (j == 1)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 0,10,0 });
		//}
		//if (j == 2)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		//if (j == 3)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		j++;
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player->SetPosition0({ 0.0f,5.0f,0.0f });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial4.csv", tutorialObstacleVal4);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}


}

void GameScene::SetStage5()
{
	player->ClearCollision();
	//camera_->SetCamera();

	//床セット
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 100,0.5,100 });
			floor->SetPosition({ 0,0,0 });
		}
		//if (j == 1)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 0,10,0 });
		//}
		//if (j == 2)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		//if (j == 3)
		//{
		//	floor->SetScale({ 10,0.5,10 });
		//	floor->SetPosition({ 10,20,0 });
		//}
		j++;
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//床
		player->SetPosition0({ 0.0f,5.0f,0.0f });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//障害物読み込み
	LoadCsv(L"Resources/obstacleTutorial5.csv", tutorialObstacleVal5);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//オブジェクト
	}


}
void (GameScene::* GameScene::Scene_[])() =
{
	&GameScene::TitleUpdate,
	&GameScene::GameUpdate,

};

void (GameScene::* GameScene::SceneDraw_[])() =
{
	&GameScene::GameDraw,

};