#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "Vector.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	delete debugCamera_;

	delete collisionManager_;

	delete model_;

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

	collisionManager_ = new CollisionManager();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("./Resources/mario.jpg");
	enemyTextureHandle_ = TextureManager::Load("./Resources/enemy.png");

	//3Dモデルの生成
	model_ = Model::Create();

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	//敵の生成
	enemy_ = new Enemy();
	//敵の初期化
	enemy_->Initialize(model_, enemyTextureHandle_);
	enemy_->SetPlayer(player_);
}

void GameScene::Update() {

	collisionManager_->ClearColliders();

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
		//ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}

	

	//自キャラの更新
	player_->Update();

	//敵の更新
	if (enemy_) {
		enemy_->Update();
	}

	collisionManager_->SetCollider(player_);
	collisionManager_->SetCollider(enemy_);
	// 自弾全てについて
	for (PlayerBullet* playerbullet : player_->GetBullets()) {
		collisionManager_->SetCollider(playerbullet);
	}
	//敵弾すべてについて
	for (EnemyBullet* enemyBullet : enemy_->GetBullets()) {
		collisionManager_->SetCollider(enemyBullet);
	}

	collisionManager_->CheckAllColision();

}
/*
void GameScene::CheckAllColision() {

	//コライダー
	std::list<Collider*> colliders_;
	//コライダーをリストに登録
	colliders_.push_back(player_);
	colliders_.push_back(enemy_);
	//自弾全てについて
	for (PlayerBullet* playerbullet : player_->GetBullets()) {
		colliders_.push_back(playerbullet);
	}
	for (EnemyBullet* enemyBullet : enemy_->GetBullets()) {
		colliders_.push_back(enemyBullet);
	}

	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* colliderA = *itrA;

		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			Collider* colliderB = *itrB;

			//ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}

}
*/
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

	//自キャラの描画
	player_->Draw(viewProjection_);

	// 敵の描画
	if (enemy_) {
		enemy_->Draw(viewProjection_);
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
/*
void GameScene::CheckCollisionPair(Collider* colliderA, Collider* colliderB) 
{

	//衝突フィルタリング
	if (((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0b0 || 
		((colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0b0))) {
		return;
	}

	Vector3 worldPosA = colliderA->GetWorldPosition();
	Vector3 worldPosB = colliderB->GetWorldPosition();

	float distance = Length(Subtract(worldPosA, worldPosB));
	// 球と球の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		// 自キャラ衝突時コールバックを呼び出す
		colliderA->OnCollision();
		// 敵弾の衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}

}
*/