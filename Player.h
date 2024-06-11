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

	//照準
	void Reticle(const ViewProjection& viewProjection);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//UI描画
	void DrawUI();

	//親となるワールドトランスフォームをセット
	void SetParent(const WorldTransform* parent);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

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

};