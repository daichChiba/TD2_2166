#include "TitleScene.h"
#include <numbers>

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	delete modelFont_;
	delete modelSpace_;
	delete modelEnter_;
	delete modelDescription_;
	delete fade_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// ビュープロジェクション
	camera_.Initialize();

	modelFont_ = Model::CreateFromOBJ("titleFont");
	modelSpace_ = Model::CreateFromOBJ("push_Space");
	modelEnter_ = Model::CreateFromOBJ("push_Enter");
	modelDescription_ = Model::CreateFromOBJ("DescriptionName");

	worldTransformFont_.Initialize();
	worldTransformSpace_.Initialize();
	worldTransformEnter_.Initialize();
	worldTransformDescription_.Initialize();

	worldTransformFont_.translation_.y = 10;
	worldTransformSpace_.translation_.y = -8;
	worldTransformEnter_.translation_.y = -14;
	worldTransformDescription_.translation_.y = -14;
	worldTransformDescription_.translation_.x = -15;
	worldTransformFont_.scale_ = {2, 2, 2};
	worldTransformSpace_.scale_ = {2, 2, 2};
	worldTransformEnter_.scale_ = {2, 2, 2};
	worldTransformDescription_.scale_ = {2, 2, 2};

	// フェード
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1);

		// サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("Title.wav");
	// 音声再生
	audio_->PauseWave(soundDataHandle_);
	// 第2引数でループ再生を指定
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);

}

void TitleScene::Update() {
	if (Input::GetInstance()->ReleseKey(DIK_SPACE)) {
		fade_->Start(Fade::Status::FadeOut, 1);
		pushSpace = true;
	}
	if (Input::GetInstance()->ReleseKey(DIK_RETURN)) {
		fade_->Start(Fade::Status::FadeOut, 1);
		pushSpace = false;
	}

	// フェードアウトが終了したらゲームシーンに行く
	if (fade_->IsFadeOutFinished() == true) {
		// 音声停止
		 audio_->StopWave(voiceHandle_);
		finished_ = true;
	}

	// タイマーを加算
	timer_ += 1.0f / 60.0f;

	// worldTransformPlayer_.rotation_.y =
	//	std::numbers::pi_v<float> + std::sin(std::numbers::pi_v<float> * 2.0f * timer_ / kMotionTime);

	// 行列を更新
	worldTransformFont_.UpdateMatrix();
	worldTransformSpace_.UpdateMatrix();
	worldTransformEnter_.UpdateMatrix();
	worldTransformDescription_.UpdateMatrix();

	// フェード
	fade_->Update();
}

void TitleScene::Draw() {
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// ここに3Dオブジェクトの描画処理を追加できる
	modelFont_->Draw(worldTransformFont_, camera_);
	modelSpace_->Draw(worldTransformSpace_, camera_);
	modelEnter_->Draw(worldTransformEnter_, camera_);
	modelDescription_->Draw(worldTransformDescription_, camera_);

	// 3Dオブジェクト描画処理後
	Model::PostDraw();

	// フェード
	fade_->Draw(commandList);
}
