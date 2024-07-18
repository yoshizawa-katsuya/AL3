#pragma once
#include "BaseCharacter.h"

class Enemy : public BaseCharacter{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models_">モデルデータ配列</param>
	void Initialize(const std::vector<Model*>& models, ViewProjection* viewProjection) override;

	// 腕振りギミック初期化
	void InitializeRollArmGimmick();

	/// <summary>
	/// 更新
	///  </summary>
	void Update() override;

	// 腕振りギミック更新
	void UpdateRollArmGimmick();

	/// <summary>
	/// 描画
	///  </summary>
	void Draw() override;

	//中心座標を取得
	Vector3 GetCenterPosition() const override;

private:

	// ワールド変換データ
	WorldTransform worldTransformBody_;
	WorldTransform worldTransformL_arm_;
	WorldTransform worldTransformR_arm_;
	
	const uint16_t kModelIndexBody = 0;
	const uint16_t kModelIndexL_arm = 1;
	const uint16_t kModelIndexR_arm = 2;

	// 腕振りギミック用の媒介変数
	float rollArmParameter_ = 0.0f;

};
