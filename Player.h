#pragma once
#include "Model.h"
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
	void Initialize(Model* model, uint32_t textureHandle);

	//更新
	void Update();

	//旋回
	void Rotate();

	//攻撃
	void Attack();

	//描画
	void Draw(const ViewProjection& viewProjection);

	//ワールド座標を取得
	Vector3 GetWorldPosition();

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

};