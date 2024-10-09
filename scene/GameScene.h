#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include <vector>
#include "DebugCamera.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "DeathParticles.h"

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

	void GenerateBlocks();

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

	//デスフラグのgetter
	bool IsFinished() const { return finished_; }


private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>

	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;

	// ビュープロジェクション
	ViewProjection viewProjection_;

	//自キャラ
	Player* player_ = nullptr;

	//playerモデル
	Model* modelPlayer_ = nullptr;

	//3Dモデルデータ
	Model* model3d_ = nullptr;

	//天球の生成
	Skydome* skydome_ = nullptr;

	std::vector <std::vector< WorldTransform* >> worldTransformBlocks_;


	// エネミーモデル
	Model* enemyModel_;

	//スカイドーム3Dモデル
	Model* modelSkydome_ = nullptr;

	//デバックカメラ有効
	bool isDebugCameraActive_ = false;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_;

	//カメラコントローラ
	CameraController* cameraController_;

	//デスパーティクル
	DeathParticles* deathParticles_ = nullptr;

	// エネミーモデル
	Model* deathParticleModel_;

	bool isDeathParticle=true;


	//すべての当たり判定を行う
	void CheckAllCollisions();

	//ゲームのフェーズ(型)
	enum class Phase {
		kPlay,	//ゲームプレイ
		kDeath,	//デス演出
	};

	//ゲームの現在フェーズ(変数)
	Phase phase_;

	void ChangePhase();

	// 終了フラグ
	bool finished_ = false;

};
