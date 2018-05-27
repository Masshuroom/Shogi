#include"DxLib.h"
#include"Mouse.h"

Mouse_ch MP;

int Mouse_Input() {

	return GetMouseInput();

}

void MouseDraw() {

	GetMousePoint(&MP.x, &MP.y);
	//DrawBox(MP.x, MP.y, MP.x + 10, MP.y + 10, GetColor(255,255,255), true);

}