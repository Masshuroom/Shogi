#include "DxLib.h"
#include "Keyboard.h"
#include "Player.h"

static const int NUM = 2;        //プレイヤーの数

Player_t m_Player[NUM];   //プレイヤーの実体

// 動きを計算する
void PlayerMgr_Update() {

	Player_MouseUpdate();

	Player_Update();

	Player_Select();

	Player_Place();

	Player_Motigoma();

}

//描画する
void PlayerMgr_Draw() {

	Player_Draw();

}