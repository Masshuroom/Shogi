#include "DxLib.h"
#include "Keyboard.h"
#include "Player.h"

static const int NUM = 2;        //�v���C���[�̐�

Player_t m_Player[NUM];   //�v���C���[�̎���

// �������v�Z����
void PlayerMgr_Update() {

	Player_MouseUpdate();

	Player_Update();

	Player_Select();

	Player_Place();

	Player_Motigoma();

}

//�`�悷��
void PlayerMgr_Draw() {

	Player_Draw();

}