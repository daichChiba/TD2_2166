
#include "GameScene.h"
#include <cassert>

using namespace KamataEngine;
// インストラクタ
GameScene::GameScene() {}
// デストラクタ
GameScene::~GameScene() {
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	// 3Dモデルデータの解放
	delete model_;

	delete player_;

	delete ModelPlayer_;

	delete botton_;

	delete ModelBotton_;

	delete mirror_;

	delete modelSkydome_;

	delete skydome_;

	// delete modelGoalParticle_;

	// for (GoalParticles* GoalParticles : goalPartiy_) {
	//	delete GoalParticles;
	// }
}

void GameScene::Initialize() {

	//// ファイル名を指定してテクスチャを読み込む
	// textureHandle_ = TextureManager::Load("mario.jpg");

	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// スカイドームの生成
	skydome_ = new Skydome();

	// 3Dモデルデータの生成
	model_ = Model::CreateFromOBJ("block", true);
	// playerModelの作成
	ModelPlayer_ = Model::CreateFromOBJ("player", true);
	// bottonModelの作成
	ModelBotton_ = Model::CreateFromOBJ("switch", true);
	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// カメラ初期化
	// カメラの位置を変える
	camera_.translation_.y = 17.6f;
	camera_.rotation_.x = 1.28f;
	camera_.translation_.z = 1.0f;
	camera_.translation_.x = 5.5f;
	camera_.farZ;
	camera_.Initialize();

	// スカイドームの初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	worldTransform_.Initialize();

	mapChipField_ = new MapChipField;

	mapChipField_->LoadMapChipCsv("Resources/blocks_csv/1stStage.csv");

	// modelGoalParticle_ = Model::CreateFromOBJ("DeathParticle", true);

	player_ = new Player; //
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = {static_cast<float>(1), 0, static_cast<float>(1)};
	player_->Initialize(ModelPlayer_, &camera_, playerPosition);
	player_->SetMapChipField(mapChipField_);
	botton_ = new Botton;
	Vector3 bottonPos = {static_cast<float>(7), 0, static_cast<float>(6)};
	botton_->Initialize(ModelBotton_, &camera_, bottonPos);
	botton2_ = new Botton;
	Vector3 botton2Pos = {static_cast<float>(4), 0, static_cast<float>(8)};
	botton2_->Initialize(ModelBotton_, &camera_, botton2Pos);
	mirror_ = new Mirror;
	Vector3 mirrorPos = {static_cast<float>(13), 0, static_cast<float>(13)};
	mirror_->Initialize(ModelPlayer_, &camera_, mirrorPos);
	mirror_->SetMapChipField(mapChipField_);
	GenerateBlocks();
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	//// ブロック１個分の横幅
	// const float kBlockWidth = 1.0f;
	// const float kBlockHeight = 1.0f;

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				// worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				worldTransformBlocks_[i][j]->translation_.x = static_cast<float>(j);
				worldTransformBlocks_[i][j]->translation_.z = static_cast<float>(i);
			}
		}
	}
}

void GameScene::Update() {

	 ChangePhase();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}

			model_->Draw(*worldTransformBlock, camera_);
			player_->Draw();
			botton_->Draw();
			botton2_->Draw();
			mirror_->Draw();
			// スカイドームの描画
			skydome_->Draw();
		}
	}
	// model_->Draw(worldTransform_, camera_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
#pragma region 自キャラと敵キャラの当たり判定
	{
		// 判定対象1と2の座標
		AABB aabb1, aabb2;
		AABB aabb3, aabb4;

		// 自キャラの座標
		aabb1 = player_->GetAABB();
		aabb2 = botton_->GetAABB();

		if (IsCollision(aabb1, aabb2)) {
			player_->OnCollision(botton_, mirror_->GetIsHit());
			botton_->OnCollision(player_);
		}

		// 自キャラの座標
		aabb3 = mirror_->GetAABB();
		aabb4 = botton2_->GetAABB();

		if (IsCollision(aabb3, aabb4)) {
			mirror_->OnCollision(botton2_, player_->GetIsHit());
			botton2_->OnCollision(mirror_);
		}
	}
#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case GameScene::Phase::kPlay:
		player_->Update();
		botton_->Update();
		botton2_->Update();
		mirror_->Update();
		skydome_->Update();

		CheckAllCollisions();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				// 04/24 02_02の更新から始める

				if (!worldTransformBlock) {
					continue;
				}

				worldTransformBlock->UpdateMatrix();
			}
		}
#ifdef _DEBUG
		if (input_->TriggerKey(DIK_0)) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif // DEBUG
       // カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();

			// デバックカメラのビュー行列
			camera_.matView = debugCamera_->GetCamera().matView;
			// デバックカメラのプロジェクション行列
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
		} else {
			//  ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		}
		worldTransform_.TransferMatrix();

		if (player_->IsAlive() == false) {
			phase_ = Phase::kDeath;
			//
			// const Vector3 deathParticlePosition = player_->GetWorldPosition();

			// goalParticles_ = new GoalParticles;
			// Vector3 position = player_->GetWorldPosition();
			// goalParticles_->Initialize(modelGoalParticle_, &camera_, position);
		}
		break;
	case GameScene::Phase::kDeath:
		botton_->Update();
		botton2_->Update();
		skydome_->Update();

		CheckAllCollisions();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				// 04/24 02_02の更新から始める

				if (!worldTransformBlock) {
					continue;
				}

				worldTransformBlock->UpdateMatrix();
			}
		}
#ifdef _DEBUG
		if (input_->TriggerKey(DIK_0)) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif // DEBUG
       // カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();

			// デバックカメラのビュー行列
			camera_.matView = debugCamera_->GetCamera().matView;
			// デバックカメラのプロジェクション行列
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
		} else {
			//  ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		}
		worldTransform_.TransferMatrix();

		//if (player_->IsAlive()==false && mirror_->IsAlive()==false) {
			finished_ = true;
		//}

		break;
	}
}
