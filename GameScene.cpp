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
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/red.png");
	stoneModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/dirt.png");
	lavaModel = FbxLoader::GetInstance()->LoadModelFromFile("stone", "Resources/lava.png");
	skydome = FbxLoader::GetInstance()->LoadModelFromFile("skydome", "Resources/skydome.png");
	//������@
	tutorialText1 = FbxLoader::GetInstance()->LoadModelFromFile("TutorialText1", "Resources/dirt.png"); //����
	tutorialText2 = FbxLoader::GetInstance()->LoadModelFromFile("TutorialText2", "Resources/dirt.png"); //�W�����v
	tutorialText3 = FbxLoader::GetInstance()->LoadModelFromFile("TutorialText4", "Resources/dirt.png"); //�}�O�}�̎��
	tutorialText4 = FbxLoader::GetInstance()->LoadModelFromFile("TutorialText3", "Resources/dirt.png"); //�������͈��S
	tutorialText5 = FbxLoader::GetInstance()->LoadModelFromFile("TutorialText5", "Resources/dirt.png"); //�������͈��S

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

	//�}�O�}
	Magma::SetDevice(dxCommon_->GetDevice());
	Magma::SetCamera(camera_.get());

	//��
	Floor::SetDevice(dxCommon_->GetDevice());
	Floor::SetCamera(camera_.get());

	//�e�L�X�g�̃I�u�W�F�N�g
	TextObject::SetDevice(dxCommon_->GetDevice());
	TextObject::SetCamera(camera_.get());
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
	newPlayer->SetModel(stoneModel);
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
		newFloor->SetScale({ 10,0.5,10 });
		newFloor->SetPosition({ -5,0,20 });
		floors.push_back(std::move(newFloor));
	}

#pragma endregion


#pragma region ��Q��������

	//�t�@�C���ǂݍ���
	std::stringstream obstaclePosList;	//������
	std::vector<DirectX::XMFLOAT3>obstaclePos;
	//�t�@�C�����J��
	std::ifstream file;
	file.open("Resources/obstacleStage1.csv");
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
		newObstacle->SetScale({ 0.11f,0.03f,0.11f});
		newObstacle->Initialize();
		obstacles.push_back(std::move(newObstacle));
	}

#pragma endregion

#pragma region �}�O�}������

	//�t�@�C���ǂݍ���
	std::stringstream magmaPosList;	//������
	std::vector<DirectX::XMFLOAT3>magmaPos;
	//�t�@�C�����J��
	std::ifstream file2;
	file2.open("Resources/obstacleStage1.csv");
	//�t�@�C���̓��e���R�s�[
	magmaPosList << file2.rdbuf();
	//�t�@�C�������
	file2.close();

	std::string line2;

	//�t�@�C�������Q���̏ꏊ��ǂݍ���
	while (getline(magmaPosList, line2, '{'))
	{
		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line2);
		std::string word1;
		std::string word2;
		std::string word3;
		//�J���}��؂�Ő擪��������擾
		getline(line_stream, word1, ',');
		getline(line_stream, word2, ',');
		getline(line_stream, word3, ',');
		DirectX::XMFLOAT3 pos(atoi(word1.c_str()), atoi(word2.c_str()), atoi(word3.c_str()));
		magmaPos.push_back(pos);
	}
	//�}�O�}������
	for (int i = 1; i <= obstacleVal; i++)
	{
		std::unique_ptr<Magma>newMagma = std::make_unique<Magma>();
		newMagma->SetModel(stoneModel);
		newMagma->SetCubeModel(hitBoxModel.get());
		//�t�@�C���ǂݍ��݂œ������W����
		newMagma->SetPosition({ 0,0,-10000 });
		newMagma->SetScale({ 0.11f,0.03f,0.11f });
		newMagma->Initialize();
		magmas.push_back(std::move(newMagma));
	}

#pragma endregion
#pragma region �}�O�}�u���b�N������

	//�}�O�}�[�Z�b�g
	MagmaBlock::SetDevice(dxCommon_->GetDevice());
	MagmaBlock::SetCamera(camera_.get());
	MagmaBlock::SetInput(input_);
	//�v���C���[������
	MagmaBlock* newMagma = new MagmaBlock();
	newMagma->SetModel(lavaModel);
	newMagma->Initialize();
	magmaBlock.reset(newMagma);

#pragma endregion

#pragma region �S�[��������

	//�f�o�C�X�ƃJ�����Z�b�g
	Goal::SetDevice(dxCommon_->GetDevice());
	Goal::SetCamera(camera_.get());
	//�S�[��������
	Goal* newGoal = new Goal();
	newGoal->SetModel(model1);
	newGoal->SetCubeModel(hitBoxModel.get());
	newGoal->Initialize();
	newGoal->SetScale({ 5.0f,5.0f ,5.0f });
	goal.reset(newGoal);

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

#pragma region skydome

	//�I�u�W�F�N�g������
	skydomeObject = new FbxObject3D;
	skydomeObject->Initialize();
	skydomeObject->SetModel(skydome);

#pragma endregion

#pragma region lavaFloor

	//�I�u�W�F�N�g������
	lavaFloor = new FbxObject3D;
	lavaFloor->Initialize();
	lavaFloor->SetModel(lavaModel);

#pragma endregion

#pragma region �e�L�X�g�̃I�u�W�F�N�g
	for (int i = 0; i < textObjectVol; i++)
	{
		std::unique_ptr<TextObject>newTextObject = std::make_unique<TextObject>();
		if (i == 0)    //WASD
		{
			newTextObject->SetModel(tutorialText1);
			newTextObject->SetPosition({ -300,20,30 });
			newTextObject->SetScale({ 0.1,0.1,0.1 });
			newTextObject->SetRotation({ 1.0f + textrot.x,-0.0f + textrot.y,-0.0f + textrot.z });
		}
		if (i == 1)    //SPACE
		{
			newTextObject->SetModel(tutorialText2);
			newTextObject->SetPosition({ -250,20,30 });
			newTextObject->SetScale({ 0.1,0.1,0.1 });
			newTextObject->SetRotation({ 1.0f + textrot.x,-0.0f + textrot.y,-0.0f + textrot.z });
		}
		if (i == 2)    //SPACE
		{
			newTextObject->SetModel(tutorialText3);
			newTextObject->SetPosition({ -160,20,30 });
			newTextObject->SetScale({ 0.09,0.09,0.09 });
			newTextObject->SetRotation({ 1.0f + textrot.x,-0.0f + textrot.y,-0.0f + textrot.z });
		}	if (i == 3)    //SPACE
		{
			newTextObject->SetModel(tutorialText4);
			newTextObject->SetPosition({ -100,20,30 });
			newTextObject->SetScale({ 0.1,0.1,0.1 });
			newTextObject->SetRotation({ 1.0f + textrot.x,-0.0f + textrot.y,-0.0f + textrot.z });
		}	if (i == 4)    //SPACE
		{
			newTextObject->SetModel(tutorialText5);
			newTextObject->SetPosition({ 300,40,30 });
			newTextObject->SetScale({ 0.1,0.1,0.1 });
			newTextObject->SetRotation({ 1.0f + textrot.x,-0.0f + textrot.y,-0.0f + textrot.z });
		}
		newTextObject->Initialize();
		textObjects.push_back(std::move(newTextObject));
	}

#pragma endregion

	SetTitle();

}

void GameScene::Update()
{
	//�R���g���[���[�X�V
	dxInput->InputProcess();

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		//player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}
	//�V�[�����Ƃ̏���
	(this->*Scene_[scene_])();

	//�f�o�b�O�p �L�[���͂ŃX�e�[�W�ύX
	if (input_->TriggerKey(DIK_0))stage = Stage::Tutorial;
	if (input_->TriggerKey(DIK_1))stage = Stage::Stage1;
	if (input_->TriggerKey(DIK_2))stage = Stage::Stage2;
	if (input_->TriggerKey(DIK_3))stage = Stage::Stage3;
	if (input_->TriggerKey(DIK_4))stage = Stage::Stage4;
	//if (input_->TriggerKey(DIK_5))stage = Stage::Stage5;

	//�O�̃V�[���ƍ��̃V�[�����Ⴉ�����烊�Z�b�g����
	if (scene_ != preScene_)
	{
		//�^�C�g�����Z�b�g
		if (scene_ == static_cast<size_t>(Scene::Title))SetTitle();
	}

	//�O�̃X�e�[�W�ƌ��݂̃X�e�[�W���Ⴉ�����烊�Z�b�g����
	if (stage != preStage)
	{
		//�`���[�g���A���Ɉړ������ꍇ
		if (stage == Stage::Tutorial)	SetTitle();
		if (stage == Stage::Stage1)		SetStage1();
		if (stage == Stage::Stage2)		SetStage2();
		if (stage == Stage::Stage3)		SetStage3();
		if (stage == Stage::Stage4)		SetStage4();
		//if (stage == Stage::Stage5)		SetStage5();
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
	if (player->GetGoalFlag() == true && stage == Stage::Title)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Stage1;

		clearTutoFlag = true;
	}

	else if (player->GetGoalFlag() == true && stage == Stage::Stage1)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Stage2;

		clear1Flag = true;
	}

	else if (player->GetGoalFlag() == true && stage == Stage::Stage2)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Stage3;

		clear2Flag = true;

	}
	else if (player->GetGoalFlag() == true && stage == Stage::Stage3)
	{
		scene_ = static_cast<size_t>(Scene::Title);
		sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
		stage = Stage::Stage4;

		clear3Flag = true;

	}

	//if (player->GetGoalFlag() == true && stage == Stage::Stage4)
	//{
	//	scene_ = static_cast<size_t>(Scene::Title);
	//	sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
	//	stage = Stage::Tutorial;

	//	clear1Flag = true;

	//}
	//���X�V
	for (std::unique_ptr<Floor>& floor : floors)
	{
		floor->Update();
	}

	//��Q���X�V
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Update();
	}
	//�}�O�}
	for (std::unique_ptr<Magma>& magma : magmas)
	{
		magma->Update();
	}
	for (std::unique_ptr<MagmaBlock>& magma : magmaBlocks)
	{
		magma->Update();
	}
	Collision();
	//�v���C���[�X�V
	player->Update();

	magmaBlock->Update();
	goal->Update();
	//�I�u�W�F�N�g�X�V
	skydomeObject->SetPosition(player->GetPosition0());
	skydomeObject->SetScale({900.0f,900.0f,900.0f});
	skydomeObject->SetRotation({0.0f,0.0f,0.0f});
	skydomeObject->Update();
	
	lavaRot += 0.006f;
	lavaFloor->SetPosition({0.0f,-150.0f,0.0f});
	lavaFloor->SetScale({ 50.0f,1.0f,50.0f });
	lavaFloor->SetRotation({0.0f,lavaRot,0.0f });
	lavaFloor->Update();
	//camera_->SetTarget(player->GetPosition0());
	//if (input_->PushKey(DIK_UP))
	//{
	//	cameraOffsetZ += 0.55;
	//}
	//if (input_->PushKey(DIK_DOWN))
	//{
	//	cameraOffsetZ -= 0.55;
	//}

	//�e�L�X�g�̃I�u�W�F�N�g�X�V
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		textObject->Update();
	}

	camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
	//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
	camera_->SetEye({ camera_->GetEye().x - 10.0f,player->GetPosition0().y+100.0f,camera_->GetEye().z+cameraOffsetZ });
	//�J�����X�V
	camera_->Update();

	//int j = 0;
	//for (std::unique_ptr<TextObject>& floor : textObjects)
	//{
	//	if (j == 0)
	//	{
	//		floor->SetScale({ 300,0.5,40 });
	//		floor->SetRotation({ 1.0f + textrot.x,-0.0f + textrot.y,-0.0f + textrot.z });
	//	}
	//}

	//if (input_->PushKey(DIK_Z)) {
	//	textrot.x += 0.1f;
	//}
	//if (input_->PushKey(DIK_X)) {
	//	textrot.y += 0.1f;
	//}
	//if (input_->PushKey(DIK_C)) {
	//	textrot.z += 0.1f;
	//}
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
		//SetTitle();
	}

	//��Q���`��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}

	//�}�O�}
	for (std::unique_ptr<Magma>& magma : magmas)
	{
		magma->Draw(dxCommon_->GetCommandList());
	}
	goal->Draw(dxCommon_->GetCommandList());
	player->Draw(dxCommon_->GetCommandList());
	magmaBlock->Draw(dxCommon_->GetCommandList());
	for (std::unique_ptr<MagmaBlock>& magma : magmaBlocks)
	{
		magma->Draw(dxCommon_->GetCommandList());
	}
	skydomeObject->Draw(dxCommon_->GetCommandList());
	lavaFloor->Draw(dxCommon_->GetCommandList());
	//���`��
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i >= 0)floor->Draw(dxCommon_->GetCommandList());
		i++;
	}
	
	//�e�L�X�g�̃I�u�W�F�N�g�`��
	int j = 0;
	for (std::unique_ptr<TextObject>& textObject : textObjects)
	{
		if (j == 0)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 1)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 2)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 3)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 4)textObject->Draw(dxCommon_->GetCommandList());
		if (j == 5)textObject->Draw(dxCommon_->GetCommandList());
		
		j++;
	}

}

void GameScene::GameUpdate()
{
	////���X�V
	//for (std::unique_ptr<Floor>& floor : floors)
	//{
	//	floor->Update();
	//}

	////��Q���X�V
	//for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	//{
	//	obstacle->Update();
	//}

	////�}�O�}�X�V
	//for (std::unique_ptr<Magma>& magma : magmas)
	//{
	//	magma->Update();
	//}

	////�v���C���[�X�V
	//player->Update();
	//magmaBlock->Update();
	//for (std::unique_ptr<MagmaBlock>& magma : magmaBlocks)
	//{
	//	magma->Update();
	//}
	//goal->Update();
	////�I�u�W�F�N�g�X�V
	//skydomeObject->SetPosition(player->GetPosition0());
	////skydomeObject->SetPosition({0.0f,0.0f,0.0f});
	//skydomeObject->SetScale({ 900.0f,900.0f,900.0f });
	//skydomeObject->SetRotation({ 0.0f,0.0f,0.0f });
	//skydomeObject->Update();

	//lavaFloor->SetPosition({0.0f,20.0f,0.0f});
	//lavaFloor->SetScale({ 900.0f,0.5f,900.0f });
	//lavaFloor->SetRotation({ 0.0f,0.0f,0.0f });
	//lavaFloor->Update();
	////camera_->SetTarget(player->GetPosition0());
	//camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
	////camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
	//camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 100.0f,camera_->GetEye().z });
	////�J�����X�V
	//camera_->Update();
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
		//SetTitle();
	}

	//��Q���`��
	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		obstacle->Draw(dxCommon_->GetCommandList());
	}
	//�}�O�}�`��
	for (std::unique_ptr<Magma>& magma : magmas)
	{
		magma->Draw(dxCommon_->GetCommandList());
	}
	player->Draw(dxCommon_->GetCommandList());
	for (std::unique_ptr<MagmaBlock>& magma : magmaBlocks)
	{
		magma->Draw(dxCommon_->GetCommandList());
	}
	skydomeObject->Draw(dxCommon_->GetCommandList());
	lavaFloor->Draw(dxCommon_->GetCommandList());
	//���`��
	int i = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (i <= 1)floor->Draw(dxCommon_->GetCommandList());
		i++;
	}
	goal->Draw(dxCommon_->GetCommandList());
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

void GameScene::LoadCsvMagma(const wchar_t* fileName, int obstacleVal)
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
	for (std::unique_ptr<Magma>& magma : magmas)
	{
		if (i <= obstacleVal)
		{
			magma->SetPosition({ obstaclePos[i].x,obstaclePos[i].y,obstaclePos[i].z });
			magma->SetHitbox();
		}
		else
		{
			magma->SetPosition({ 0,0,-10000 });
			magma->SetHitbox();
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

void GameScene::SetTitle()
{
	player->ClearCollision();
	//player->SetGoalFlag();
	//camera_->SetCamera();
	for (std::unique_ptr<MagmaBlock>& magma : magmaBlocks)
	{
		magma->RemoveErupt();
	}

	//���Z�b�g
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 300,0.5,40 });
			floor->SetPosition({ -170.0f,5.0f,30.0f });
		}
		if (j == 1)
		{
			floor->SetScale({ 100,0.5,100 });
			floor->SetPosition({ 300.0	,20.0	,40.0 });
		}
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
		//�S�[���Z�b�g
		goal->SetTutorial();
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//�S�[��
		player->SetPosition0({ -310.0f,5.0f,30.0f });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 80.0f,camera_->GetEye().z });
	}
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial2.csv", tutorialObstacleVal2);
	LoadCsvMagma(L"Resources/magmaTutorial.csv", magmaVal2);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}

	for (std::unique_ptr<Magma>& magma : magmas)
	{
		//player->SetCollisionObstacle(magma->GetHitboxPosition(), magma->GetHitboxScale());	//�}�O�}
	}

}

void GameScene::SetStage1()
{
	player->ClearCollision();
	//camera_->SetCamera();
	for (std::unique_ptr<MagmaBlock>& magma : magmaBlocks)
	{
		magma->RemoveErupt();
	}

	//���Z�b�g
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 50,0.5,40 });
			floor->SetPosition({ -10.0f,5.0f,30.0f });
		}
		if (j == 1)
		{
			floor->SetScale({ 100,0.5,100 });
			floor->SetPosition({ -20.0	,190.0	,100.0 });
		}
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
		//�S�[���Z�b�g
		goal->SetStage1();
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player->SetCollisionGoal(goal->GetPosition(), goal->GetScale());	//�S�[��
		player->SetPosition0({ 0.0	,2.0	,20.0 });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 80.0f,camera_->GetEye().z });
	}
	//��Q���ǂݍ���
	LoadCsv(L"Resources/gameStage1.csv", tutorialObstacleVal2);
	LoadCsvMagma(L"Resources/magmaStage1.csv", magmaVal2);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}

	for (std::unique_ptr<Magma>& magma : magmas)
	{
		//player->SetCollisionObstacle(magma->GetHitboxPosition(), magma->GetHitboxScale());	//�}�O�}
	}

}

void GameScene::SetStage2()
{
	player->ClearCollision();
	//camera_->SetCamera();

	//���Z�b�g
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
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player->SetPosition0({ 0.0f,5.0f,0.0f });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//��Q���ǂݍ���
	LoadCsv(L"Resources/gameStage1.csv", tutorialObstacleVal2);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}


}

void GameScene::SetStage3()
{
	player->ClearCollision();
	//camera_->SetCamera();

	//���Z�b�g
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
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player->SetPosition0({ 0.0f,5.0f,0.0f });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial3.csv", tutorialObstacleVal3);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}


}

void GameScene::SetStage4()
{
	player->ClearCollision();
	//camera_->SetCamera();

	//���Z�b�g
	int j = 0;
	for (std::unique_ptr<Floor>& floor : floors)
	{
		if (j == 0)
		{
			floor->SetScale({ 10,0.5,10 });
			floor->SetPosition({ 5.0	,6.0	,40.0 });
		}
		if (j == 1)
		{
			floor->SetScale({ 100,0.5,100 });
			floor->SetPosition({ 30.0,190.0,100.0 });
		}
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
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player->SetPosition0({ 5.0	,6.0	,40.0 });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//��Q���ǂݍ���
	LoadCsv(L"Resources/gameStage4.csv", tutorialObstacleVal2 );

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}


}

void GameScene::SetStage5()
{
	player->ClearCollision();
	//camera_->SetCamera();

	//���Z�b�g
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
		player->SetCollisionFloor(floor->GetPosition(), floor->GetScale());	//��
		player->SetPosition0({ 0.0f,5.0f,0.0f });
		camera_->PlayerAim(player->GetPosition0(), player->GetPosition0(), player->GetPlayerState());
		//camera_->SetEye({ player->GetPosition0().x,player->GetPosition0().y+10.0f,player->GetPosition0().z});
		camera_->SetEye({ camera_->GetEye().x,player->GetPosition0().y + 10.0f,camera_->GetEye().z });
	}
	//��Q���ǂݍ���
	LoadCsv(L"Resources/obstacleTutorial5.csv", tutorialObstacleVal5);

	for (std::unique_ptr<Obstacle>& obstacle : obstacles)
	{
		player->SetCollisionObstacle(obstacle->GetHitboxPosition(), obstacle->GetHitboxScale());	//�I�u�W�F�N�g
	}


}
void (GameScene::* GameScene::Scene_[])() =
{
	&GameScene::TitleUpdate,
	//&GameScene::GameUpdate,

};

void (GameScene::* GameScene::SceneDraw_[])() =
{
	&GameScene::TitleDraw,
	//&GameScene::GameDraw,

};

void GameScene::Collision() {
	for (std::unique_ptr<Magma>& magma : magmas)
	{
	if (magma->GetPosition().x - player->GetPosition0().x < 5 &&
		-5 < magma->GetPosition().x - player->GetPosition0().x) {
		if (magma->GetPosition().y - player->GetPosition0().y < 5 &&
			-5 < magma->GetPosition().y - player->GetPosition0().y) {
			if (magma->GetPosition().z - player->GetPosition0().z < 5 &&
				-5 < magma->GetPosition().z - player->GetPosition0().z) {
				CreateEruption({ magma->GetPosition().x,magma->GetPosition().y - 400.0f, magma->GetPosition().z });
				magma->Teleport();
			}
		}
	}
	for (std::unique_ptr<MagmaBlock>& _magmaBlocks : magmaBlocks)
	{
		if				(_magmaBlocks->GetPosition().x - player->GetPosition0().x < 5 &&
			-5 <		 _magmaBlocks->GetPosition().x - player->GetPosition0().x) {
			if			(_magmaBlocks->GetPosition().y - player->GetPosition0().y < 100 &&
				-300 <	 _magmaBlocks->GetPosition().y - player->GetPosition0().y) {
				if		(_magmaBlocks->GetPosition().z - player->GetPosition0().z < 5 &&
					-5 < _magmaBlocks->GetPosition().z - player->GetPosition0().z) {
					
					if (player->GetFloorFlag() != true) {
						if (clearTutoFlag == false) {
			SetTitle();
		}
		else if (clearTutoFlag == true) {
			SetStage1();
		}
		else if (clearTutoFlag == true && clear1Flag == true) {
			SetStage2();
		}
		else if (clearTutoFlag == true && clear2Flag == true) {
			SetStage3();
		}
		else if (clearTutoFlag == true && clear3Flag == true) {
			SetStage4();
		}
					}
				}
			}
		}
		if (_magmaBlocks->GetPosition().y > player->GetPosition0().y) {
			_magmaBlocks->RemoveErupt();
		}

	}
	if (player->GetPosition0().y < -100) {
		if (clearTutoFlag == false) {
			SetTitle();
		}
		else if (clearTutoFlag == true) {
			SetStage1();
		}
		else if (clearTutoFlag == true && clear1Flag == true) {
			SetStage2();
		}
		else if (clearTutoFlag == true && clear2Flag == true) {
			SetStage3();
		}
		else if (clearTutoFlag == true && clear3Flag == true) {
			SetStage4();
		}
	}
	}

	diceTime++;
	if (diceTime >= 500) {
		diceFlag = true;
		//diceTime = 0;
	}
	if (diceFlag == true) {
		rollDice();
		
		diceRoll = rollDice();
		diceTime = 0;
		DiceResult();
		diceFlag = false;
	}
	// Perform the action based on the dice roll
	for (auto it = magmaBlocks.begin(); it != magmaBlocks.end();)
	{
		// Check if the block's position meets the condition relative to the player's position
		if ((*it)->GetPosition().y >= player->GetPosition0().y)
		{
			// The condition is met, so remove this block from the vector
			it = magmaBlocks.erase(it);  // Erase and get the next iterator
		}
		else
		{
			++it;  // Only increment if no removal occurs
		}
	}
}

void GameScene::CreateEruption(XMFLOAT3 pos) {
	std::unique_ptr<MagmaBlock>newObject = std::make_unique<MagmaBlock>();
	newObject->SetModel(lavaModel);
	newObject->SetPosition({pos.x,pos.y,pos.z });
	//newObject->SetScale({pos.x,pos.y,pos.z });
	newObject->Initialize();
	newObject->SetEruptFlag();
	magmaBlocks.push_back(std::move(newObject));
	diceRoll = 0;
}

void GameScene::DiceResult() {
	switch (diceRoll) {
	case ACTION_ONE:
		//if(diceFlag==true)
		CreateEruption({ player->GetPosition0().x +15,player->GetPosition0().y - 400.0f, player->GetPosition0().z });
		//diceRoll = 0;
		break;
	case ACTION_TWO:
		CreateEruption({ player->GetPosition0().x + 0.0f,player->GetPosition0().y - 400.0f, player->GetPosition0().z + 15.0f });
		//diceRoll = 0;
		break;
	case ACTION_THREE:
		CreateEruption({ player->GetPosition0().x,player->GetPosition0().y - 400.0f, player->GetPosition0().z - 15.0f });
		//diceRoll = 0;
		break;
	default:
		break;
	}
}