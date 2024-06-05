#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include <memory>
#include <sstream>

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
	/// 衝突判定と応答
	/// </summary>
	void CheckAllColision();

	/// <summary>
	/// 敵発生
	/// </summary>
	void EnemyPop(const Vector3& position);

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	void AddEnemybullet(EnemyBullet* enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemyPopCommands();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t enemyTextureHandle_ = 0;

	//3Dモデル
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;

	//待機
	bool isWait_;
	int32_t waitTimer_;

	//自キャラ
	Player* player_ = nullptr;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//敵
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemys_;

	// 敵の弾
	std::list<EnemyBullet*> enemyBullets_;

	//天球
	std::unique_ptr<Skydome> skydome_;

	//レールカメラ
	std::unique_ptr<RailCamera> railCamera_;

};
