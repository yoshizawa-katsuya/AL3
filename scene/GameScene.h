#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "LockOn.h"
#include "Enemy.h"
#include "Skydome.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "CollisionManager.h"
#include <memory>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	// メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//デバッグカメラ有効 Pキーで切り替え
	bool isDebugCameraActive_ = false;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	//衝突マネージャ
	std::unique_ptr<CollisionManager> collisionManager_;

	//テクスチャハンドル
	uint32_t textureHnadle_ = 0;

	//モデル
	//std::unique_ptr<Model> model_;
	std::unique_ptr<Model> modelSkydome_;
	std::unique_ptr<Model> modelGround_;

	std::unique_ptr<Model> modelFighterBody_;
	std::unique_ptr<Model> modelFighterHead_;
	std::unique_ptr<Model> modelFighterL_arm_;
	std::unique_ptr<Model> modelFighterR_arm_;

	std::unique_ptr<Model> modelHammer_;

	std::unique_ptr<Model> modelEnemyBody_;
	std::unique_ptr<Model> modelEnemyL_arm_;
	std::unique_ptr<Model> modelEnemyR_arm_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;

	//地面
	std::unique_ptr<Ground> ground_;

	//自キャラ
	std::unique_ptr<Player> player_;

	//ロックオン
	std::unique_ptr<LockOn> lockOn_;

	//敵
	std::list<std::unique_ptr<Enemy>> enemies_;
	
};
