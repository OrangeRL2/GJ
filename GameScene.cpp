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

#pragma region �J����������

	//�J����������
	Camera::SetInput(input_);
	Camera::SetDXInput(dxInput);
	Camera* newCamera = new Camera();
	newCamera->Initialize();
	camera_.reset(newCamera);
	camera_->SetTarget({ 0,0,0 });
	camera_->SetEye({ 0, 10, -50 });

#pragma endregion

#pragma region FBX�ǂݍ���
	//FBX�ǂݍ���
	FbxLoader::GetInstance()->Initialize(dxCommon_->GetDevice());
	//���f�������w�肵�ăt�@�C���ǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/white1x1.png");


#pragma endregion

#pragma region �I�u�W�F�N�g�Ƀf�o�C�X���Z�b�g

	//�f�o�C�X���Z�b�g
	FbxObject3D::SetDevice(dxCommon_->GetDevice());
	FbxObject3D::SetCamera(camera_.get());
	FbxObject3D::CreateGraphicsPipeline();

	//�L���[�u�̐ݒ�
	//�f�o�C�X���Z�b�g
	CubeObject3D::SetDevice(dxCommon_->GetDevice());
	CubeObject3D::SetCamera(camera_.get());
	CubeObject3D::SetInput(input_);
	CubeObject3D::CreateGraphicsPipeline();

	//���^�{�[��
	//�f�o�C�X���Z�b�g
	Metaball::SetDevice(dxCommon_->GetDevice());
	Metaball::SetCamera(camera_.get());
	Metaball::CreateGraphicsPipeline();

	//��Q��
	Obstacle::SetDevice(dxCommon_->GetDevice());
	Obstacle::SetCamera(camera_.get());

	//��
	Floor::SetDevice(dxCommon_->GetDevice());
	Floor::SetCamera(camera_.get());


#pragma endregion

#pragma region �L���[�u���f���̐ݒ�

	//���f���̐ݒ�
	//���̃��f��
	CubeModel* newCubeModel = new CubeModel();
	newCubeModel->CreateBuffers(dxCommon_->GetDevice());
	cubeModel.reset(newCubeModel);
	cubeModel->SetImageData({ 1.0f, 1.0f, 1.0f,0.5f });
	//hitbox�̃��f��
	CubeModel* newCubeModel1 = new CubeModel();
	newCubeModel1->CreateBuffers(dxCommon_->GetDevice());
	hitBoxModel.reset(newCubeModel1);
	hitBoxModel->SetImageData({ 1.0f,0.5f,0.5f,1.0f });

#pragma endregion

#pragma region �v���C���[������

	//�v���C���[�Z�b�g
	Player::SetDevice(dxCommon_->GetDevice());
	Player::SetCamera(camera_.get());
	Player::SetInput(input_);
	Player::SetDXInput(dxInput);
	//�v���C���[������
	Player* newPlayer = new Player();
	newPlayer->SetModel(model1);
	newPlayer->SetCubeModel(hitBoxModel.get());
	newPlayer->Initialize();
	player.reset(newPlayer);

#pragma endregion

#pragma region ���̐ݒ�

	//��������
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


#pragma region ��Q��������

	//�t�@�C���ǂݍ���
	std::stringstream obstaclePosList;	//������
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/obstacleTutorial.csv");
	//�t�@�C���̓��e���R�s�[
	obstaclePosList << file.rdbuf();
	//�t�@�C�������
	file.close();

	std::string line;

	//�t�@�C�������Q���̏ꏊ��ǂݍ���
	while (getline(obstaclePosList, line, '{'))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);
		std::string word1;
		std::string word2;
		std::string word3;
		//�J���}��؂�Ő擪��������擾
		getline(line_stream, word1, ',');
		getline(line_stream, word2, ',');
		getline(line_stream, word3, ',');
		DirectX::XMFLOAT3 pos(atoi(word1.c_str()), atoi(word2.c_str()), atoi(word3.c_str()));
		obstaclePos.push_back(pos);
	}
	//��Q��������
	for (int i = 1; i <= obstacleVal; i++)
	{
		std::unique_ptr<Obstacle>newObstacle = std::make_unique<Obstacle>();
		newObstacle->SetModel(stoneModel);
		newObstacle->SetCubeModel(hitBoxModel.get());
		//�t�@�C���ǂݍ��݂œ������W����
		newObstacle->SetPosition({ 0,0,-10000 });
		newObstacle->Initialize();
		obstacles.push_back(std::move(newObstacle));
	}

#pragma endregion

#pragma region �X�v���C�g
	//�X�v���C�g����������
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

	SetTitle();

}

void GameScene::Update()
{
	//�R���g���[���[�X�V
	dxInput->InputProcess();


	//�V�[�����Ƃ̏���
	(this->*Scene_[scene_])();

	//�f�o�b�O�p �L�[���͂ŃX�e�[�W�ύX
	if (input_->TriggerKey(DIK_0))stage = Stage::Tutorial;


	//�O�̃V�[���ƍ��̃V�[�����Ⴉ�����烊�Z�b�g����
	if (scene_ != preScene_)
	{
		//�^�C�g�����Z�b�g
		if (scene_ == static_cast<size_t>(Scene::Title))SetTitle();
	}

	//�O�̃t���[���̃V�[���擾
	preScene_ = scene_;
	preStage = stage;
}

void GameScene::Draw()
{
	(this->*SceneDraw_[sceneDraw_])();
}

void GameScene::TitleUpdate()
{

	//���X�V
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Update();
	}

	//�v���C���[�X�V
	player->Update();
	//camera_->SetTarget(player->GetPosition0());
	camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
	//�J�����X�V
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
	}


	player->Draw(dxCommon_->GetCommandList());
	//���`��
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i == 0)floor->Draw(dxCommon_->GetCommandList());
		i++;
	}

}

void GameScene::SetTitle()
{
	camera_->SetCamera();
	
	//���Z�b�g
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 1100,0.5,60 });
			floor->SetPosition({ 0,0,0 });
		}
		if (j == 1)
		{
			floor->SetScale({ 0.5f,80.0,60 });
			floor->SetPosition({ -320,0,0 });
		}
		if (j == 2)
		{
			floor->SetScale({ 0.5f,80.0,60 });
			floor->SetPosition({ 520,0,0 });
		}
		if (j == 3)
		{
			floor->SetScale({ 1100,0.5,60 });
			floor->SetPosition({ 0,40,0 });
		}
		if (j == 4)
		{
			floor->SetScale({ 1100,0.5,60 });
			floor->SetPosition({ 0,-40,0 });
		}
		if (j == 5)
		{
			floor->SetScale({ 1100,80.0f,0.5 });
			floor->SetPosition({ 0,0,30 });
		}
		if (j == 6)
		{
			floor->SetScale({ 1100,80.0f,0.5 });
			floor->SetPosition({ 0,0,-30 });
		}
		j++;
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
	}

}


void GameScene::LoadCsv(const wchar_t* fileName, int obstacleVal)
{
	//�t�@�C���ǂݍ���
	std::stringstream obstaclePosList;	//������
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//�t�@�C�����J��
	std::ifstream file;
	file.open(fileName);
	//�t�@�C���̓��e���R�s�[
	obstaclePosList << file.rdbuf();
	//�t�@�C�������
	file.close();

	std::string line;

	//�t�@�C�������Q���̏ꏊ��ǂݍ���
	while (getline(obstaclePosList, line, '{'))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);
		std::string word1;
		std::string word2;
		std::string word3;
		//�J���}��؂�Ő擪��������擾
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
		//�t�@�C���ǂݍ���
		std::stringstream obstaclePosList;	//������
		std::vector<DirectX::XMFLOAT3>obstaclePos;
		//�t�@�C�����J��
		std::ifstream file;
		file.open(fileName);
		//�t�@�C���̓��e���R�s�[
		obstaclePosList << file.rdbuf();
		//�t�@�C�������
		file.close();

		std::string line;

		//�t�@�C�������Q���̏ꏊ��ǂݍ���
		while (getline(obstaclePosList, line, '{'))
		{
			//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
			std::istringstream line_stream(line);
			std::string word1;
			std::string word2;
			std::string word3;
			//�J���}��؂�Ő擪��������擾
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



void (GameScene::* GameScene::Scene_[])() =
{
	&GameScene::TitleUpdate,

};

void (GameScene::* GameScene::SceneDraw_[])() =
{
	&GameScene::TitleDraw,

};