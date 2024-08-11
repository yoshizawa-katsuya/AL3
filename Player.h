#pragma once
#include "Model.h"
#include "Sprite.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

//自キャラ
class Player {

public:
	
	//デストラクタ
	~Player();

	//初期化
	void Initialize(Model* model, uint32_t textureHandle, const Vector3& position);

	//更新
	void Update(const ViewProjection& viewProjection);

	//旋回
	void Rotate();

	//攻撃
	void Attack();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//UI描画
	void DrawUI();

	void LockOn(const Vector2& position, const Vector3& targetPosition);

	//親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent);

	void SetIsLockOn(bool isLockOn) { isLockOn_ = isLockOn; } 

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	//2Dレティクルの座標を取得
	Vector2 Get2DReticlePosition() { return sprite2DReticle_->GetPosition(); }

	Vector2 Get2DReticleSize() { return sprite2DReticle_->GetSize(); }

	//弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	const float GetRadius() const { return radius_; }

private:
	
	//キーボード入力
	Input* input_ = nullptr;

	//弾
	std::list<PlayerBullet*> bullets_;

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;

	// 半径
	const float radius_ = 1.0f; 

	bool isLockOn_ = false;

	Vector3 target_;

};