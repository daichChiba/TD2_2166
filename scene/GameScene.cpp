#include "GameScene.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>


// インストラクタ
GameScene::GameScene() {}
// デストラクタ
GameScene::~GameScene() {
	delete model_;
	// ここからAL3の02_01の22をやる
	delete player_;

	delete model3d_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete modelSkydome_;

	delete skydome_;

	delete mapChipField_;

	delete debugCamera_;

	delete cameraController_;

	for (Enemy* enemy:enemies_) {
		delete enemy;
	}

	if(isDeathParticle){
		delete deathParticles_;
	}

}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	//// ブロック１個分の横幅
	//const float kBlockWidth = 1.0f;
	//const float kBlockHeight = 1.0f;

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
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Initialize() {

	//// ファイル名を指定してテクスチャを読み込む
	// textureHandle_ = TextureManager::Load("chopper.png");

	//ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

	// 3Dモデルの作成
	model_ = Model::Create();

	// ビュープロジェクション
	viewProjection_.farZ = 600;
	viewProjection_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルデータの生成
	model3d_ = Model::CreateFromOBJ("block",true);

	// 3Dモデルの生成(スカイドーム)
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);



	// 　3Dplayerモデルデータの作成
	modelPlayer_ = Model::CreateFromOBJ("player",true);

	skydome_ = new Skydome;

	enemyModel_ = Model::CreateFromOBJ("enemy", true);

	deathParticleModel_ = Model::CreateFromOBJ("deathParticle", true);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks_csv/blocks.csv");

	GenerateBlocks();

	player_ = new Player; //
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(7, 7);
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);

	player_->SetMapChipField(mapChipField_);


	//enemy_ = new Enemy;
	//enemy_->Initialize(enemyModel_, &viewProjection_, enemyPosition);

	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = {10 + i * 4.0f, 1, 0};
		newEnemy->Initialize(enemyModel_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	//	仮の生成処理。後で消す。
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(deathParticleModel_, &viewProjection_, playerPosition);

	// 　天球の生成
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// カメラコントローラの初期化
	// カメラコントローラの生成
	cameraController_ = new CameraController();
	// 初期化
	cameraController_->Initialize();
	// 追従対象をセット
	cameraController_->SetTarget(player_);
	// リセット(瞬間合わせ)
	cameraController_->Reset();

	//移動範囲の指定
	Rect cameraArea = {12.0, 100.0f - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);



	// デバックカメラの生成
	debugCamera_ = new DebugCamera(640, 360);
	debugCamera_->SetFarZ(5000000);
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
			skydome_->Draw();
			// プレイヤーの描画処理
			if (phase_ == Phase::kPlay) {
				player_->Draw();
			}
			model3d_->Draw(*worldTransformBlock, viewProjection_);
		}
	}
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (isDeathParticle) {
		deathParticles_->Draw();
	}


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
		//判定対象1と2の座標
		AABB aabb1, aabb2;

		//自キャラの座標
		aabb1 = player_->GetAABB();

		//自キャラと敵弾すべての当たり判定
		for (Enemy*enemy:enemies_) {
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				// 敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}

		}
	}
	#pragma endregion

}

void GameScene::ChangePhase() {


		switch (phase_) {
	case GameScene::Phase::kPlay:
		// 自キャラの更新
		player_->Update();
		// 敵キャラの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
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
		skydome_->Update();

#ifdef _DEBUG
		if (input_->TriggerKey(DIK_0)) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif // DEBUG
       // カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();

			// デバックカメラのビュー行列
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			// デバックカメラのプロジェクション行列
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		} else {
			cameraController_->Update();

			// ビュープロジェクション行列の更新と転送
			viewProjection_.matView = cameraController_->GetViewProjection().matView;
			viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		}

		if (player_->IsDead() == true) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();



			deathParticleModel_ = Model::CreateFromOBJ("deathParticle", true);

			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticleModel_, &viewProjection_, deathParticlesPosition);
		}


		break;
	case GameScene::Phase::kDeath:
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

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

		if (isDeathParticle) {
			deathParticles_->Update();
		}

		skydome_->Update();

#ifdef _DEBUG
		if (input_->TriggerKey(DIK_0)) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif // DEBUG
       // カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();

			// デバックカメラのビュー行列
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			// デバックカメラのプロジェクション行列
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		} else {
			cameraController_->Update();

			// ビュープロジェクション行列の更新と転送
			viewProjection_.matView = cameraController_->GetViewProjection().matView;
			viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		}

		if (deathParticles_ && deathParticles_->IsFinished()) {

			finished_ = true;
		}

		break;
	}
}
