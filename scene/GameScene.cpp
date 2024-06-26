#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//追従カメラの生成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	//ファイル名を指定してテクスチャを読み込む
	textureHnadle_ = TextureManager::Load("./Resources/mario.jpg");

	//3Dモデルの生成
	//model_.reset(Model::Create());
	modelSkydome_.reset(Model::CreateFromOBJ("Skydome03", true));
	modelGround_.reset(Model::CreateFromOBJ("Ground01", true));

	//自キャラモデル
	modelFighterBody_.reset(Model::CreateFromOBJ("float_Body", true));
	modelFighterHead_.reset(Model::CreateFromOBJ("float_Head", true));
	modelFighterL_arm_.reset(Model::CreateFromOBJ("float_L_arm", true));
	modelFighterR_arm_.reset(Model::CreateFromOBJ("float_R_arm", true));
	modelHammer_.reset(Model::CreateFromOBJ("Hammer01", true));
	std::vector<Model*> playerModels = 
	{modelFighterBody_.get(), modelFighterHead_.get(), modelFighterL_arm_.get(),
	 modelFighterR_arm_.get(), modelHammer_.get()};

	//敵モデル
	modelEnemyBody_.reset(Model::CreateFromOBJ("Enemy02_Body", true));
	modelEnemyL_arm_.reset(Model::CreateFromOBJ("Enemy02_L_arm", true));
	modelEnemyR_arm_.reset(Model::CreateFromOBJ("Enemy02_R_arm", true));
	std::vector<Model*> enemyModels = {modelEnemyBody_.get(), modelEnemyL_arm_.get(), modelEnemyR_arm_.get()};

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//スカイドームの生成
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(modelSkydome_.get(), &viewProjection_);

	//地面の生成
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(modelGround_.get(), &viewProjection_);

	//自キャラの生成
	player_ = std::make_unique<Player>();
	//自キャラの初期化
	player_->Initialize(playerModels, &viewProjection_);
	player_->SetCameraViewProjection(&followCamera_->GetViewProjection());

	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(&player_->GetWorldTransform());

	//敵の生成
	enemy_ = std::make_unique<Enemy>();
	//敵の初期化
	enemy_->Initialize(enemyModels, &viewProjection_);

}

void GameScene::Update() {

#ifdef  _DEBUG

	if (input_->TriggerKey(DIK_P)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

#endif //  _DEBUG

	// スカイドームの更新
	skydome_->Update();

	//地面の更新
	ground_->Update();

	//自キャラの更新
	player_->Update();

	//敵の更新
	enemy_->Update();

	// カメラの処理
	if (isDebugCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();

		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {

		// 追従カメラの更新
		followCamera_->Update();

		viewProjection_.matView = followCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = followCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	}

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	// スカイドームの描画
	skydome_->Draw();

	//地面の描画
	ground_->Draw();

	//自キャラの描画
	player_->Draw();

	//敵の描画
	enemy_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
