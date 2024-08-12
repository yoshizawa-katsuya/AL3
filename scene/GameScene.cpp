#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Vector.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete debugCamera_;

	delete model_;
	delete modelSkydome_;

	//自キャラの解放
	delete player_;

	//敵の解放
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}

	//敵の弾の解放
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

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

	//待機
	isWait_ = false;
	waitTimer_ = 0;

	//敵発生データの読み込み
	LoadEnemyPopData();

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
	railCamera_->Initialize({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f});

	//レティクルのテクスチャ
	TextureManager::Load("./Resources/reticle.png");

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	Vector3 playerPosition(0, 0, 20.0f);
	player_->Initialize(model_, textureHandle_, playerPosition);
	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//スカイドームの生成
	skydome_ = std::make_unique<Skydome>();
	//スカイドームの初期化
	skydome_->Initialize(modelSkydome_);
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
	player_->Update(viewProjection_);

	UpdateEnemyPopCommands();
	
	//デスフラグの立った敵を削除
	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});
	// 敵の更新
	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	// デスフラグの立った弾を削除
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	// 弾更新
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	CheckAllColision();

}

void GameScene::CheckAllColision() {

	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	//敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region

	//自キャラと敵弾の当たり判定
	
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets_) {
	
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

	for (Enemy* enemy : enemys_) {

		// 敵キャラの座標
		posA = enemy->GetWorldPosition();

		// 敵キャラと自弾すべての当たり判定
		for (PlayerBullet* bullet : playerBullets) {

			// 自弾の座標
			posB = bullet->GetWorldPosition();
			float distance = Length(Subtract(posA, posB));
			// 球と球の交差判定
			if (distance <= enemy->GetRadius() + bullet->GetRadius()) {
				// 敵キャラ衝突時コールバックを呼び出す
				enemy->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
			}
		}
	}
	#pragma endregion

	#pragma region

	//自弾と敵弾の当たり判定
	for (PlayerBullet* playerBullet : playerBullets) {
		// 自弾の座標
		posA = playerBullet->GetWorldPosition();

		for (EnemyBullet* enemyBullet : enemyBullets_) {
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

	#pragma region

	Vector2 ScreenPosA = player_->Get2DReticlePosition();
	Vector2 SizeA = player_->Get2DReticleSize();

	//player_->SetIsLockOn(false);

	for (Enemy* enemy : enemys_) {
		Vector2 ScreenPosB = {enemy->GetScreenPosition(viewProjection_).x, enemy->GetScreenPosition(viewProjection_).y};

		if (ScreenPosB.x >= ScreenPosA.x - SizeA.x && ScreenPosB.x <= ScreenPosA.x + SizeA.x &&
			ScreenPosB.y >= ScreenPosA.y - SizeA.y && ScreenPosB.y <= ScreenPosA.y + SizeA.y) {

			player_->LockOn(enemy, viewProjection_);
		}

	}

	#pragma endregion
}

void GameScene::EnemyPop(const Vector3& position) {

	// 敵の生成
	Enemy* newEnemy_ = new Enemy();
	// 敵の初期化
	newEnemy_->Initialize(model_, enemyTextureHandle_, position);
	newEnemy_->SetPlayer(player_);
	// 敵キャラにゲームシーンを渡す
	newEnemy_->SetGameScene(this);
	// リストに登録する
	enemys_.push_back(newEnemy_);

}

void GameScene::AddEnemybullet(EnemyBullet* enemyBullet) {

	//リストに登録する
	enemyBullets_.push_back(enemyBullet);

}

void GameScene::LoadEnemyPopData() {

	//ファイルを開く
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();

}

void GameScene::UpdateEnemyPopCommands() {

	//待機処理
	if (isWait_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			//待機完了
			isWait_ = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			EnemyPop(Vector3(x, y, z));

		}

		//WAITコマンドの実行
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait_ = true;
			waitTimer_ = waitTime;

			//コマンドループを抜ける
			break;
		}

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

	//スカイドームの描画
	skydome_->Draw(viewProjection_);

	//自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	/*
	if (enemy_) {
		enemy_->Draw(viewProjection_);
	}
	*/
	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	// 弾描画
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
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
	
	player_->DrawUI(viewProjection_);


	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
