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
	void Initialize(Model* model, ViewProjection* viewProjection);

	/// <summary>
	/// 更新
	///  </summary>
	void Update();

	/// <summary>
	/// 描画
	///  </summary>
	void Draw();

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

	void SetCameraViewProjection(const ViewProjection* cameraViewProjection) { cameraViewProjection_ = cameraViewProjection; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

	//カメラのビュープロジェクション
	const ViewProjection* cameraViewProjection_ = nullptr;

	//目標角度
	float targetAngle_;
	//角度補完レート
	const float angleCompletionRate_ = 0.2f;
};