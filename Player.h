#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "BaseCharacter.h"
#include <optional>

///< summary>
/// 自キャラ
///  </summary>
class Player : public BaseCharacter{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models_">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) override;

	//通常行動初期化
	void BehaviorRootInitialize();

	//攻撃行動初期化
	void BehaviorAttackInitialize();

	//ダッシュ行動初期化
	void BehaviorDashInitialize();

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();

	//腕振りギミック初期化
	void InitializeRollArmGimmick();

	/// <summary>
	/// 更新
	///  </summary>
	void Update() override;

	// 通常行動更新
	void BehaviorRootUpdate();

	//攻撃行動更新
	void BehaviorAttackUpdate();

	//ダッシュ行動初期化
	void BehaviorDashUpdate();

	//浮遊ギミック更新
	void UpdateFloatingGimmick();

	//腕振りギミック更新
	void UpdateRollArmGimmick();

	/// <summary>
	/// 描画
	///  </summary>
	void Draw() override;

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetCameraViewProjection(const ViewProjection* cameraViewProjection) { cameraViewProjection_ = cameraViewProjection; }

private:

	//振るまい
	enum class Behavior {
		kRoot,	//通常状態
		kAttack,	//攻撃中
		kDash,	//ダッシュ中
	};

	Behavior behavior_ = Behavior::kRoot;

	//次の振るまいリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	WorldTransform worldTransformHammer_;

	const uint16_t kModelIndexBody = 0;
	const uint16_t kModelIndexHead = 1;
	const uint16_t kModelIndexL_arm = 2;
	const uint16_t kModelIndexR_arm = 3;
	const uint16_t kModelIndexHammer = 4;

	//カメラのビュープロジェクション
	const ViewProjection* cameraViewProjection_ = nullptr;

	//目標角度
	float targetAngle_;
	//角度補完レート
	const float angleCompletionRate_ = 0.2f;

	//浮遊ギミック用の媒介変数
	float floatingParameter_ = 0.0f;

	//腕振りギミック用の媒介変数
	float rollArmParameter_ = 0.0f;

	//攻撃用ワーク
	struct WorkAttack {
		// 攻撃用の媒介変数
		uint16_t attackParameter_ = 0;
	};

	WorkAttack workAttack_;

	
	

	// ダッシュ用ワーク
	struct  WorkDash {
		//ダッシュ用の媒介変数
		uint32_t dashParameter_ = 0;
	};

	WorkDash workDash_;
	
};