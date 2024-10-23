#pragma once
#include <KamataEngine.h>
using namespace KamataEngine;
#include "Fade.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include <vector>

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene {
public:
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFedeOut, // フェードアウト
	};

	TitleScene();

	~TitleScene();

	/// <summary>
	/// 初期化
	/// <summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw();

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

	bool GetPushSpace() { return pushSpace; }

private:
	// 終了フラグ
	bool finished_ = false;

	Model* modelFont_ = nullptr;
	Model* modelSpace_ = nullptr;
	Model* modelEnter_ = nullptr;
	Model* modelDescription_ = nullptr;

	// ビュープロジェクション
	Camera camera_;

	WorldTransform worldTransformFont_;
	WorldTransform worldTransformSpace_;
	WorldTransform worldTransformEnter_;
	WorldTransform worldTransformDescription_;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	float timer_ = 0.0f;
	static inline const float kMotionTime = 1.0f;

	// 画面フェード
	Fade* fade_ = nullptr;
	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	// サウドデータハンドル
	uint32_t soundDataHandle_ = 0;
	// 音声再生ハンドル
	uint32_t voiceHandle_ = 0;

	bool pushSpace = false;
};
