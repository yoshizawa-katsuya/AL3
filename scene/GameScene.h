#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>
#include "Matrix.h"
#include "DebugCamera.h"
#include "Skydome.h"
#include "Player.h"
#include "Enemy.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "DeathParticle.h"

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

	void GeneratrBlocks();

	//全ての当たり判定を行う
	void CheckAllCollisions();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	

	//3Dモデルデータ
	Model* model_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelPlayer_ = nullptr;
	Model* modelEnemy_ = nullptr;
	Model* modelDeathParticle_ = nullptr;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//デバッグカメラ有効
	bool isDebuCameraActive_ = false;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//スカイドーム
	Skydome* skydome_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;
	
	//カメラコントローラ
	CameraController* cameraController_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;

	DeathParticle* deathParticle_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	Player* player_ = nullptr;

	//敵
	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;

	const uint32_t kMaxEnemyNum_ = 3;

};
