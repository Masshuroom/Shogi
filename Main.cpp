#include "DxLib.h"
#include "PlayerMgr.h"
#include "Keyboard.h"
#include "Mouse.h"

//������ڂ������߂�
int fights = 0;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK);

	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {

		Keyboard_Update();    //�L�[�{�[�h�̍X�V

		MouseDraw();			//�}�E�X�̕\��

		PlayerMgr_Update();   //�v���C���[�Ǘ����W���[���̍X�V

		PlayerMgr_Draw();    //�v���C���[�Ǘ����W���[���̕`��


	}

	DxLib_End();
	return 0;
}