#include "Description.h"

Description::Description() {}

Description::~Description() {
	delete modelDescription_;
	delete modelSpace_;
}

void Description::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクション
	camera_.Initialize();

	modelDescription_ = Model::CreateFromOBJ("Description");
	modelSpace_ = Model::CreateFromOBJ("push_Enter");

	worldTransformDescription_.Initialize();
	worldTransformSpace_.Initialize();

	worldTransformDescription_.translation_.y = 7;
	worldTransformSpace_.translation_.y = -10;
	worldTransformDescription_.scale_ = {5, 5, 5};
	worldTransformSpace_.scale_ = {2, 2, 2};
	// サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("Title.wav");
	// 音声再生
	audio_->PauseWave(soundDataHandle_);
	// 第2引数でループ再生を指定
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
}

void Description::Update() {
	if (Input::GetInstance()->ReleseKey(DIK_RETURN)) {
		// 音声停止
		audio_->StopWave(voiceHandle_);
		finished_ = true;
	}
	// タイマーを加算
	timer_ += 1.0f / 60.0f;

	// 行列を更新
	worldTransformDescription_.UpdateMatrix();
	worldTransformSpace_.UpdateMatrix();
}

void Description::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	modelDescription_->Draw(worldTransformDescription_, camera_);
	modelSpace_->Draw(worldTransformSpace_, camera_);

	// 3Dオブジェクト描画処理後
	Model::PostDraw();
}
