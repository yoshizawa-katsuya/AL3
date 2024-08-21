#pragma once
#include "GameScene.h"
#include "TitleScene.h"

extern GameScene* gameScene;
extern TitleScene* titleScene;

// シーン(型)
enum class Scene {
	kUnknown = 0,

	kTitle,
	kGame,
};

// 現在シーン
extern Scene scene;

void ChangeScene();

void UpdateScene();

void DrawScene();