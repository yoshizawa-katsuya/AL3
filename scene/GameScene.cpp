#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Vector.h"
#include "Curve.h"
#include "PrimitiveDrawer.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete debugCamera_;

	delete model_;
	delete modelSkydome_;

	//自キャラの解放
	delete player_;

	//敵の解放
	delete enemy_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("./Resources/mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("./Resources/enemy.png");

	//3Dモデルの生成
	model_ = Model::Create();
	modelSkydome_ = Model::CreateFromOBJ("skydome02", true);

	// レールカメラの生成
	railCamera_ = std::make_unique<RailCamera>();
	// レールカメラの初期化
	railCamera_->Initialize({0.0f, 0.0f, -20.0f}, {0.0f, 0.0f, 0.0f});


	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	Vector3 playerPosition(0, 0, 20.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);
	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//敵の生成
	enemy_ = new Enemy();
	//敵の初期化
	enemy_->Initialize(model_, enemyTextureHandle_);
	enemy_->SetPlayer(player_);

	//スカイドームの生成
	skydome_ = std::make_unique<Skydome>();
	//スカイドームの初期化
	skydome_->Initialize(modelSkydome_);

	controlPoints_ = {
	    {0, 0, 0},
		{10, 10, 0},
		{10, 15, 0},
		{20, 15, 0},
		{20, 0, 0},
		{30, 0, 0},
    };

	pointsDrawing_ = GenerateCatmullRomSplinePoints(controlPoints_, segmentCount_);


}

void GameScene::Update() {

	#ifdef _DEBUG
		if (input_->TriggerKey(DIK_P)) {
			if (isDebugCameraActive_ == false) {
				isDebugCameraActive_ = true;
		    } else {
			    isDebugCameraActive_ = false;
		    }
		}
	#endif // DEBUG

	if (isDebugCameraActive_) {

		// デバッグカメラの更新
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {

		//レールカメラの更新
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}

	//スカイドームの更新
	skydome_->Update();

	//自キャラの更新
	player_->Update();

	//敵の更新
	if (enemy_) {
		enemy_->Update();
	}

	CheckAllColision();

}

void GameScene::CheckAllColision() {

	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region

	//自キャラと敵弾の当たり判定
	
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
	
		//敵弾の座標
		posB = bullet->GetWorldPosition();
		float distance = Length(Subtract(posA, posB));
		//球と球の交差判定
		if (distance <= player_->GetRadius() + bullet->GetRadius()) {
			//自キャラ衝突時コールバックを呼び出す
			player_->OnCollision();
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}

	#pragma endregion

	#pragma region

	//自弾と敵キャラの当たり判定

	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 敵キャラと自弾すべての当たり判定
	for (PlayerBullet* bullet : playerBullets) {

		// 自弾の座標
		posB = bullet->GetWorldPosition();
		float distance = Length(Subtract(posA, posB));
		// 球と球の交差判定
		if (distance <= enemy_->GetRadius() + bullet->GetRadius()) {
			// 敵キャラ衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}

	#pragma endregion

	#pragma region

	//自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();

		for (EnemyBullet* enemyBullet : enemyBullets) {
			// 敵弾の座標
			posB = enemyBullet->GetWorldPosition();
			float distance = Length(Subtract(posA, posB));
			// 球と球の交差判定
			if (distance <= playerBullet->GetRadius() + enemyBullet->GetRadius()) {
				// 敵弾の衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
			}

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

	//スカイドームの描画
	skydome_->Draw(viewProjection_);

	//自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}

	Vector4 red = {1.0f, 0.0f, 0.0f, 1.0f};

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	for (size_t i = 0; i < pointsDrawing_.size() - 1; i++) {
			PrimitiveDrawer::GetInstance()->DrawLine3d(pointsDrawing_[i], pointsDrawing_[i + 1], red);
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
