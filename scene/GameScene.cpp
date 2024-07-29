#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete model_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;

	delete modelSkydome_;

	delete modelPlayer_;

	delete modelEnemy_;

	delete modelDeathParticle_;

	delete skydome_;


	delete player_;

	delete deathParticle_;

	//delete enemy_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete mapChipField_;

	delete cameraController_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

	// 3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	modelDeathParticle_ = Model::CreateFromOBJ("DeathParticle", true);

	//スカイドームの生成
	skydome_ = new Skydome();

	// マップチップフィールドの生成
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");


	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	Vector3 PlayerPosition = mapChipField_->GetMapChipPositionByIndex(4, 14);
	player_->Initialize(modelPlayer_, &viewProjection_, PlayerPosition);
	player_->SetMapChipField(mapChipField_);

	//敵の生成
	/*
	enemy_ = new Enemy();
	//敵の初期化
	Vector3 EnemyPosition = mapChipField_->GetMapChipPositionByIndex(10, 18);
	enemy_->Initialize(modelEnemy_, &viewProjection_, EnemyPosition);
	*/
	for (uint32_t i = 0; i < kMaxEnemyNum_; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(10 + (i * 2), 18);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	//カメラコントローラの生成
	cameraController_ = new CameraController;
	cameraController_->Initialize();
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	
	viewProjection_.Initialize();

	skydome_->Initialize(modelSkydome_, &viewProjection_);

	GeneratrBlocks();

	

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("./Resources/mario.jpg");


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

}

void GameScene::Update() {

	switch (phase_) {
	case Phase::kPlay:

		PlayPhaseUpdate();

		break;

	case Phase::kDeath:
	
		DeathPhaseUpdate();

		break;
	
	default:
		break;
	}	

	#ifdef _DEBUG

		if (input_->TriggerKey(DIK_SPACE)) {
			isDebuCameraActive_ = true;
		}

	

	#endif

	

	

}

void GameScene::PlayPhaseUpdate() {


	// スカイドームの更新
	skydome_->Update();

	// 自キャラの更新
	player_->Update();

	// 敵の更新
	// enemy_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	if (isDebuCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} 
	else {

		// カメラコントローラの更新
		cameraController_->Update();
		viewProjection_.translation_ = cameraController_->GetViewProjection().translation_;
		// ビュープロジェクションの更新と転送
		viewProjection_.UpdateMatrix();
	}

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}

	CheckAllCollisions();

	ChangePhase();

}

void GameScene::DeathPhaseUpdate() {

	// スカイドームの更新
	skydome_->Update();

	// 敵の更新
	// enemy_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}

	deathParticle_->Update();
	
	if (isDebuCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();

	} else {
		
		// ビュープロジェクションの更新と転送
		viewProjection_.UpdateMatrix();
	}

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->UpdateMatrix();
		}
	}

	if (deathParticle_ && deathParticle_->GetIsFinished()) {
		isFinished_ = true;
	}

}

void GameScene::ChangePhase() {

	switch (phase_) {
	case GameScene::Phase::kPlay:

		
		if (player_->GetIsDead()) {
			// デス演出フェーズに切り替え
			phase_ = Phase::kDeath;
			//自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticle_ = new DeathParticle;
			deathParticle_->Initialize(modelDeathParticle_, &viewProjection_, deathParticlesPosition);

		}

		break;
	case GameScene::Phase::kDeath:
		
		break;
	default:
		break;
	}

}

void GameScene::GeneratrBlocks() {

	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	//要素数を変更する
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}

}

void GameScene::CheckAllCollisions() {

	#pragma region

	//判定対象1と2の座標
	AABB aabb1, aabb2;

	//自キャラの座標
	aabb1 = player_->GetAABB();

	//自キャラと敵キャラ全ての当たり判定
	for (Enemy* enemy : enemies_) {
		//敵キャラの座標
		aabb2 = enemy->GetAABB();

		//AABB同士の衝突判定
		if (IsCollision(aabb1, aabb2)) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
			//敵キャラの衝突時コールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}

	#pragma endregion

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

	if (!player_->GetIsDead()) {
		player_->Draw();
	}
	if (deathParticle_) {
		deathParticle_->Draw();
	}

	//敵の描画
	//enemy_->Draw();
	for (Enemy* enemy : enemies_) {
	
		enemy->Draw();
	}

	

	//ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			model_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

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
