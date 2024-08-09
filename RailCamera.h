#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <vector>

//レールカメラ
class RailCamera {
public:

	//初期化
	void Initialize(const Vector3 worldPos, const Vector3 rotate);

	//更新
	void Update();

	//描画
	void Draw(const ViewProjection& viewProjection);

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	
	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	
	// スプライン曲線制御点(通過点)
	std::vector<Vector3> controlPoints_;

	// 線分で描画する用の頂点リスト
	std::vector<Vector3> pointsDrawing_;
	// 線分の数
	const uint32_t segmentCount_ = 100;

	Vector3 eye_;
	Vector3 target_;
	Vector3 forward_;
	Vector3 offset_ = {0.0f, 1.0f, 0.0f};

	uint32_t difference_ = 5;
	uint32_t moveCount_ = 0;
};