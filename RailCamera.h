#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"

//レールカメラ
class RailCamera {
public:

	//初期化
	void Initialize(const Vector3 worldPos, const Vector3 rotate);

	//更新
	void Update();

	const ViewProjection& GetViewProjection() { return viewProjecttion_; }

	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:
	
	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewProjecttion_;

};