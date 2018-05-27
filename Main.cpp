#include "DxLib.h"
#include "PlayerMgr.h"
#include "Keyboard.h"
#include "Mouse.h"

//今何手目かをしめす
int fights = 0;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {

		Keyboard_Update();    //キーボードの更新

		MouseDraw();			//マウスの表示

		PlayerMgr_Update();   //プレイヤー管理モジュールの更新

		PlayerMgr_Draw();    //プレイヤー管理モジュールの描画


	}

	DxLib_End();
	return 0;
}