#pragma once
#include "Model.h"
#include "WorldTransform.h"

///< summary>
/// 自キャラ
///  </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	///  </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* modelBody, Model* modelHead, Model* modelL_arm, Model* modelR_arm, ViewProjection* viewProjection);

	//浮遊ギミック初期化
	void InitializeFloatingGimmick();

	//腕振りギミック初期化
	void InitializeRollArmGimmick();

	/// <summary>
	/// 更新
	///  </summary>
	void Update();

	//浮遊ギミック更新
	void UpdateFloatingGimmick();

	//腕振りギミック更新
	void UpdateRollArmGimmick();

	/// <summary>
	/// 描画
	///  </summary>
	void Draw();

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	void SetCameraViewProjection(const ViewProjection* cameraViewProjection) { cameraViewProjection_ = cameraViewProjection; }

private:
	// ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;

	// モデル
	Model* modelBody_ = nullptr;
	Model* modelHead_ = nullptr;
	Model* modelL_arm_ = nullptr;
	Model* modelR_arm_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

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
};