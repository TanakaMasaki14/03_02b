#include "DxLib.h"
#include"Vector2.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "LE2B_14_タナカマサキ: タイトル";

// ウィンドウ横幅
const int WIN_WIDTH = 600;

// ウィンドウ縦幅
const int WIN_HEIGHT = 400;


/// <summary>
/// 線と円の当たり判定
/// </summary>
/// <param name="lineStart"></param>
/// <param name="lineEnd"></param>
/// <param name="circlePos"></param>
/// <param name="circleRadius"></param>
/// <returns></returns>
bool CollisonLineToCircle(Vector2 lineStart, Vector2 lineEnd, Vector2 circlePos, float circleRadius);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine,
	_In_ int nCmdShow) {
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0x00);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み


	// ゲームループで使う変数の宣言

	Vector2 circle{ WIN_WIDTH / 2,230 };
	float circleR = 16.0f;

	Vector2 lineStart{ 100,250 };
	Vector2 lineEnd{ 500,250 };

	int objColor = 0xffffff;

	// 最新のキーボード情報用
	char keys[256] = { 0 };

	// 1ループ(フレーム)前のキーボード情報
	char oldkeys[256] = { 0 };

	// ゲームループ
	while (true) {
		// 最新のキーボード情報だったものは1フレーム前のキーボード情報として保存
		// 最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		//サークル座標
		circle.x += (keys[KEY_INPUT_RIGHT] - keys[KEY_INPUT_LEFT]) * 1.0f;
		circle.y += (keys[KEY_INPUT_DOWN] - keys[KEY_INPUT_UP]) * 1.0f;

		//線の終点
		lineEnd.y += (keys[KEY_INPUT_S] - keys[KEY_INPUT_W]) * 1.0f;

		//当たり判定
		objColor = 0xffffff;

		if (CollisonLineToCircle(lineStart, lineEnd, circle, circleR)) {
			//色変える
			objColor = 0xff;
			DrawFormatString(50, 50, objColor, "あたった");
		}

		// 描画処理

		DrawCircle(circle.x, circle.y, circleR, objColor, true);
		DrawLine(lineStart.x, lineStart.y, lineEnd.x, lineEnd.y, objColor, true);
		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1) {
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) {
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}

bool CollisonLineToCircle(Vector2 lineStart, Vector2 lineEnd, Vector2 circlePos, float circleRadius)
{
	//線のベクトル作成
	Vector2 vecLine, vecCtoLS, vecCtoLHorizontal, vecCtoLVertical;
	vecLine = lineEnd - lineStart;
	//線のベクトル正規化
//	vecLine = vecLine.normalize();
	//線の始点から円のベクトル作成
	vecCtoLS = circlePos - lineStart;
	float length = vecCtoLS.dot(vecLine.normalize());

	vecCtoLHorizontal = vecLine * length;
	vecCtoLVertical = vecCtoLS - vecCtoLHorizontal;

	float len = vecCtoLVertical.length();
	if (len <= circleRadius) {
		return true;
	}

	return false;
}
