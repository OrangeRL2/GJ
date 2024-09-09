#pragma once
#include "memory"
#include "list"
#include "Input.h"
#include "DXInput.h"
#include "DirectXCommon.h"
#include "FbxObject3D.h"
#include "CubeObject3D.h"
#include "CubeModel.h"
#include "Metaball.h"
#include "Collision.h"
#include "Camera.h"
#include "Obstacle.h"
#include "Magma.h"
#include "Sprite.h"
#include "Floor.h"
#include "Player.h"
#include "MagmaBlock.h"
#include "Goal.h"
class GameScene
{
	//メンバ関数
public:
	GameScene();
	~GameScene();
	void Initialize(DirectXCommon* dxCommon, Input* input);
	//更新
	void Update();
	//描画
	void Draw();
	//タイトルシーン
	void TitleUpdate();
	void TitleDraw();
	//ゲームシーン
	void GameUpdate();
	void GameDraw();

	//シーンごとのセット関数
	//タイトルをセット
	void SetTitle();
	//チュートリアルをセット
	void SetTutorial();
	void SetStage1();
	void SetStage2();
	void SetStage3();
	void SetStage4();
	void SetStage5();

	//スペースキーでファイル読み込みを実行する関数
	void LoadCsv(const wchar_t* fileName, int obstacleVal);
	void LoadCsvMagma(const wchar_t* fileName, int magmaVal);
	void DebugLoadCsv(const wchar_t* fileName, int obstacleVal);

	void Collision();
	//メンバ変数
private:
	//デバイスとinput
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	DXInput* dxInput = new DXInput();

	//カメラ
	std::unique_ptr<Camera> camera_;

	//------------モデル関連-------------
	//fbx
	FbxModel* model1 = nullptr;
	FbxModel* model2 = nullptr;
	//岩のモデル
	FbxModel* stoneModel = nullptr;
	//ゴールのモデル
	FbxModel* goalModel = nullptr;
	//鍵のモデル
	FbxModel* keyModel = nullptr;
	FbxModel* lavaModel = nullptr;
	FbxModel* skydome = nullptr;

	//ステージとかタイトルのモデル
	//タイトル
	FbxModel* titleModel = nullptr;
	//stage1
	FbxModel* stage1Model = nullptr;
	//stage2
	FbxModel* stage2Model = nullptr;
	//stage3
	FbxModel* stage3Model = nullptr;
	//stage4
	FbxModel* stage4Model = nullptr;
	//stage5
	FbxModel* stage5Model = nullptr;
	//tutorial
	FbxModel* stageTutoModel = nullptr;
	//clear1
	FbxModel* clear1Model = nullptr;

	//キューブ
	std::unique_ptr<CubeModel> cubeModel;
	/*std::unique_ptr<CubeObject3D> cubeObject;*/

	//キューブ(hitbox用)
	std::unique_ptr<CubeModel> hitBoxModel;
	FbxObject3D* skydomeObject = nullptr;
	//----------自作クラス---------
	//プレイヤー
	std::unique_ptr<Player> player;
	
	//障害物
	std::list<std::unique_ptr<Obstacle>> obstacles;
	//まぐまのゾーン
	std::list<std::unique_ptr<Magma>> magmas;
	//マグマの障害物
	std::unique_ptr<MagmaBlock> magmaBlock;
	//障害物の数
	size_t obstacleVal = 350;
	//床
	//std::unique_ptr<Floor>floor;
	std::list<std::unique_ptr<Floor>>floors;
	size_t floorVol = 7;
	//ゴール
	std::unique_ptr<Goal>goal;

	//---------------------------

	//スプライト
	Sprite* sprite = new Sprite;
	Sprite titleSprite;	//title.png
	Sprite keySprite;	//key.png
	//スプライト共通データ生成
	SpriteCommon spriteCommon;

	//シーン
	enum class Scene
	{
		Title,	//タイトル
		Game,	//ゲーム
	};
	//シーン	最初がタイトル
	size_t scene_ = static_cast<size_t>(Scene::Title);
	//前のフレームのシーン
	size_t preScene_ = static_cast<size_t>(Scene::Title);
	//メンバ関数のポインタテーブル
	static void (GameScene::* Scene_[])();

	//シーンの描画
	enum class SceneDraw
	{
		TitleDraw,	//タイトル
		GameDraw,	//ゲーム
	};
	//シーン	最初がタイトル
	size_t sceneDraw_ = static_cast<size_t>(SceneDraw::TitleDraw);
	//メンバ関数のポインタテーブル
	static void (GameScene::* SceneDraw_[])();

	//ステージを分ける列挙型
	enum Stage
	{
		Title,
		Tutorial,
		Stage1,
		Stage2,
		Stage3,
		Stage4,
		Stage5,
	};
	//ステージ
	Stage stage = Stage::Title;
	//前のフレームのステージ
	Stage preStage = Stage::Title;

	//ステージごとの障害物の数
	//tutorial
	size_t tutorialObstacleVal = 9;
	size_t tutorialObstacleVal1 = 9;
	size_t tutorialObstacleVal2 = 20;
	size_t tutorialObstacleVal3 = 24;
	size_t tutorialObstacleVal4 = 126;
	size_t tutorialObstacleVal5 = 126;
	size_t stage1ObstacleVal = 5;

	//magma
	size_t magmaVal2 = 20;
	float num = 0;

	//クリアーフラッグ
	bool clearTutoFlag = false;
	bool clear1Flag = false;
	bool clear2Flag = false;
	bool clear3Flag = false;
	bool clear4Flag = false;
	bool clear5Flag = false;
	bool clear6Flag = false;
};