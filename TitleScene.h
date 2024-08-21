#pragma once
#include "DirectXCommon.h"
#include "Fade.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleScene {
public:
	~TitleScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// デスフラグのgetter
	bool GetIsFinished() const { return isFinished_; }

private:
	// シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadoOut, // フェードアウト
	};

	DirectXCommon* dxCommon_ = nullptr;

	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransform_;

	Model* modelTitle_ = nullptr;
	WorldTransform worldTransformTitle_;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	// 終了フラグ
	bool isFinished_ = false;

	Fade* fade_ = nullptr;

	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;
};