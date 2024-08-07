#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "BaseEnemyState.h"

//敵
class Enemy {
public:
	// 初期化
	void Initialize(Model* model, uint32_t textureHandle);

	// 更新
	void Update();

	//void ApproachUpdate();
	//void LeaveUpdate();

	// 描画
	void Draw(const ViewProjection& viewProjection);

	//状態変更
	void ChangeState(std::unique_ptr<BaseEnemyState> state);

	const Vector3&  GetTranslation() { return worldTransform_.translation_; }

	void SetTranslation(const Vector3& translation) { worldTransform_.translation_ = translation; }

private:

	//状態
	std::unique_ptr<BaseEnemyState> state_;

	//メンバ関数ポインタ
	//void (Enemy::*pFunk_)();

	//メンバ関数のポインタのテーブル
	//static void (Enemy::*spFuncTable[])();

	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	/*
	enum class Phase {
		Approach, // 接近する
		Leave,    // 離脱する
	};
	//フェーズ
	Phase phase_ = Phase::Approach;
	*/

	// キャラクターの移動速さ
	
	
};