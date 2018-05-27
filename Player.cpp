#include "DxLib.h"
#include "Keyboard.h"
#include "Player.h"
#include "Stage.h"
#include "Mouse.h"
#include "Statas.h"
#include <stdlib.h>

static int pre_x, pre_y;//�`��X�V���O�̍��W
static int nex_x, nex_y;//���̎�̍��W
static int pre_click_x, pre_click_y;//���̎��I������
static int pre_xx = 5, pre_yy = 5;//���ӁA�����ʒu�̎��ɁA��Ȃ��ʒu�ɒu��
static int M_x, M_y; //�}�E�X���N���b�N�������W
static int Click_Flag = 0;
static int Click_Statas; //�N���b�N�������W�̋�̖���
static int C_x, C_y; //�}�E�X�����ځi�X�e�[�W�N���b�N���P�̎��j�ɃN���b�N�������W
static int C2_x, C2_y; //�}�E�X�����ڈȍ~�i�X�e�[�W�N���b�N���Q�̎��j�ɃN���b�N�������W
static int Can_Place; //���}�E�X�̂�����W�ɋ�u���邩�ǂ����H�i�N���b�N�X�e�[�^�X0�ȊO�̎��j�A1�̎��u����
static int fights = 1;//���̎萔
static int teban; //���A��Ԃ݂͂����ɂ��邩�A�G�ɂ��邩�i0�c�����A1�c�G�j
static int hantei = 0; //�I�񂾋�G��������(0�͖����A1�͓G�j
static int pre_statas; //�Ƃ�����̃X�e�[�^�X��ۑ�����
static int move; //���������u�Ԃ𔻒f����
static int motigoma_statas_me, motigoma_statas_ene, motigoma_number_me, motigoma_number_ene, motigoma_flag_me = 0, motigoma_flag_ene, motigoma_x, motigoma_y; 
			//�I�񂾎�����̃X�e�[�^�X�A����ۑ�����A�������I�����Ă��邩�A�N���b�N�����������x���W

static int Stage_Place[11][11] = { 0 };// �����Ղ̃X�e�[�W�i9�~9�j�i0�͂����ɒu���Ȃ��A�A�P�͑I��������̈ʒu�A2�͒u�����Ƃ��\�j
static int hohei_place_me[9] = { 0 }, hohei_place_ene[9] = { 0,};//�����Ղ̕����̗L�����m�F����i�������u�����̔���j

									   // �������v�Z����
									   // ���̊֐��̓��́c��̃X�e�[�^�X�A�i�N���b�N���������Ղ̍��W�ɂ��̋�̃X�e�[�^�X������j

void Player_Update() {

	//�X�e�[�W�v���C�X�̒T���i2�̎������u���Ȃ��j

	int can;

	can = Stage_Place[C2_y][C2_x];

	Click_Statas = Stage[C_y][C_x];

	//���ڂ̍X�V
	if (Click_Flag == 2 && can == 2 && motigoma_flag_me == 0 && motigoma_flag_ene == 0) { //�N���b�N�t���b�O���Q���w���Ă���i���̎��I��ł���j�A���A�X�y�[�X�������ꂽ�Ƃ��A
																						  //���A�����ɒu�����Ƃ��\�ican���Q�j
		if (Keyboard_Get(KEY_INPUT_SPACE) == 1) {

			for (int i = 0; i < 2; i++) { //���̓��삪�Ȃ��ƂȂ������܂������Ȃ�

				Stage[pre_yy][pre_xx] = -1;

				Stage[C2_y][C2_x] = Click_Statas;	//���ڂ̑I���̍��W�Ɉ��ږڂ̃X�e�[�^�X����

				pre_xx = C_x;
				pre_yy = C_y;

				//���ɂȂ��邽�߂ɁA�N���b�N�t���b�O���O�ɏ��������A�X�e�[�W�v���C�X���O�ɏ���������

				Click_Flag = 0;

				for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++)
						Stage_Place[i][j] = 0;

				fights += 1;

				teban = (fights / 2) % 2;//��Ԃ̍X�V

			}
		}

	}

	//������̍X�V

	//����

	if (motigoma_flag_me == 1 && teban == 0 && C2_x < 9 && C2_y < 9) {

		if (Keyboard_Get(KEY_INPUT_SPACE) == 1) {

			pre_statas = 0;

			for (int i = 0; i < 2; i++) { //���̓��삪�Ȃ��ƂȂ������܂������Ȃ�

				Stage[C2_y][C2_x] = motigoma_statas_me * 10;	//���ڂ̑I���̍��W�Ɉ��ږڂ̃X�e�[�^�X����

																//���ɂȂ��邽�߂ɁA�N���b�N�t���b�O���O�ɏ��������A�X�e�[�W�v���C�X���O�ɏ���������

				motigoma_flag_me = 0;

				for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++)
						Stage_Place[i][j] = 0;

				fights += 1;

				teban = (fights / 2) % 2;//��Ԃ̍X�V

			}

			Motigoma_Me[motigoma_x] -= 1;

		}

	}

	//�G

	if (motigoma_flag_ene == 1 && teban == 1 && C2_x < 9 && C2_y < 9) {

		if (Keyboard_Get(KEY_INPUT_SPACE) == 1) {

			pre_statas = 0;

			for (int i = 0; i < 2; i++) { //���̓��삪�Ȃ��ƂȂ������܂������Ȃ�

				Stage[M_y][M_x] = motigoma_statas_ene * 10 + 1;	//���ڂ̑I���̍��W�Ɉ��ږڂ̃X�e�[�^�X����

																//���ɂȂ��邽�߂ɁA�N���b�N�t���b�O���O�ɏ��������A�X�e�[�W�v���C�X���O�ɏ���������

				motigoma_flag_ene = 0;

				for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++)
						Stage_Place[i][j] = 0;

				fights += 1;

				teban = (fights / 2) % 2;//��Ԃ̍X�V

			}

			Motigoma_Ene[motigoma_y] -= 1;

		}

	}


	//�@�X�e�[�W����̍X�V

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			Stage_Hantei[i][j] = (Stage[i][j] % 10);

	// ����Ƃ����u�Ԃ���Amove������������B������ƁA���Z�b�g�����@

	if (Keyboard_Get(KEY_INPUT_SPACE) >= 0 && move < 2)		move += 1;

	if (Click_Flag == 2 && M_x < 9 && M_y < 9)	move = 0;

	if (Click_Flag == 2) {

		pre_statas = Stage[C2_y][C2_x];

	}

	if (Mouse_Input() == MOUSE_INPUT_LEFT)
		if (M_x < 8 && M_y == 10 || M_x == 10 && M_y < 8)
			pre_statas = 0;

	//hohei_place�̍X�V
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {



			if (Stage[j][i] == 80) {//�����̕�������������

				hohei_place_me[i] = 1;

			}
			else if (Stage[j][i] == 81) {

				hohei_place_ene[i] = 1;

			}

		}

}

// ���I�����A���̎��I������

void Player_Select() {

	int i, j;

	//�}�E�X�̍��W�ʒu��ǂݎ��
	for (i = 0; i < 11; i++)
		for (j = 0; j < 11; j++) {

			if (Stage_Mouse[i][j] == 1) { //�}�E�X�̍��W������

				if (Mouse_Input() == MOUSE_INPUT_LEFT) {

					M_x = j;
					M_y = i;

				}

				if (M_x < 9 && M_y < 9) {

					hantei = Stage[i][j] % 10;
					Stage_Hantei[i][j] = Stage[i][j] % 10;

					if (Stage[i][j] != 0) { //����݂��Ă�����

						if (Click_Flag == 0 && Mouse_Input() == MOUSE_INPUT_LEFT) { //���N���b�N���ꂽ��A�N���b�N�������W��ۑ�����

																					//��Ԃ�������������A���A�N���b�N���Ă���������������
							if (hantei == 0 && teban == 0) {

								M_x = j;
								M_y = i;

								if (Click_Flag == 0) {//�����A�N���b�N�t���b�O���O��������i�N���b�N����ĂȂ�������j

									Stage_Click[M_y][M_x] = 1;

									Click_Flag = 1;

								}
							}



							//�����A��l�v���C���鎞�A�������J������



							//��Ԃ����肾������A���A�N���b�N���Ă����G��������
							else if (hantei == 1 && teban == 1) {

								M_x = j;
								M_y = i;

								if (Click_Flag == 0) {//�����A�N���b�N�t���b�O���O��������i�N���b�N����ĂȂ�������j

									Stage_Click[M_y][M_x] = 1;

									Click_Flag = 1;

								}

							}
							//*/

						}

					}

				}

			}

			if (M_x >= 9 || M_y >= 9)
				Stage_Place[M_y][M_x] = -1;

		}


	//���I��������ԂŁA�E�N���b�N�i�I��������j���ꂽ��A�X�e�[�W�v���C�X���O�ɏ���������
	
	if (Click_Flag != 0 && Mouse_Input() == MOUSE_INPUT_RIGHT) {

		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++)
				Stage_Place[i][j] = 0;
		
		motigoma_statas_me = 0;
		motigoma_statas_ene = 0;
		/*
		motigoma_number_me = 0;
		motigoma_number_ene = 0;
		motigoma_flag_me = 0;
		motigoma_flag_ene = 0;
		*/

	}
	
	if (Mouse_Input() == MOUSE_INPUT_LEFT) { //���N���b�N���ꂽ��A���A���N���b�N����Ă�����A���A�N���b�N�����ꏊ�łȂ�������

		if (Click_Flag != 0 && Stage_Click[M_y][M_x] == 0) {

			Stage_Click[pre_click_y][pre_click_x] = 0;

			//���ڈȍ~�ɃN���b�N�������W�i���̎�j��ۑ�����
			C2_x = M_x;
			C2_y = M_y;

			Stage_Click[M_y][M_x] = 2;

			Click_Flag = 2;

			pre_click_x = M_x;
			pre_click_y = M_y;

		}

		if (Stage_Place[M_y][M_x] == 2)
			Can_Place = 1;
		else	Can_Place = 0;

		// ������̑I�����s���i�����j

		if (M_x < 8 && M_y == 10 && teban == 0) {
			motigoma_number_me = Motigoma_Me[M_x];

			if (motigoma_number_me > 0) {
				motigoma_statas_me = M_x + 1;

				motigoma_x = M_x;

				Click_Flag = 1;
				motigoma_flag_me = 1;

				if (motigoma_statas_me != 8) {

					for (int i = 1; i < 9; i++)
						for (int j = 0; j < 9; j++) {

							if (Stage[i][j] == -1)
								Stage_Place[i][j] = 2;

						}
				}

				//�����̎�����̒u����ꏊ�̏���

				else if (motigoma_statas_me == 8) {//�����̏ꍇ�A������̂�����͈͂�������

					for (int i = 1; i < 9; i++)
						for (int j = 0; j < 9; j++) {

							if (Stage[j][i] != -1);
							else if (hohei_place_me[j] == 0) {

								Stage_Place[i][j] = 2;

							}

							else if (hohei_place_me[j] == 1) {

								Stage_Place[i][j] = 0;

							}

						}

				}

			}

		}

		//�G

		if (M_x == 10 && M_y < 8 && teban == 1) {
			motigoma_number_ene = Motigoma_Ene[M_y];

			if (motigoma_number_ene > 0) {
				motigoma_statas_ene = M_y + 1;

				motigoma_y = M_y;

				Click_Flag = 1;
				motigoma_flag_ene = 1;

				if (motigoma_statas_ene != 8) {

					for (int i = 1; i < 9; i++)
						for (int j = 0; j < 9; j++) {

							if (Stage[i][j] == -1)
								Stage_Place[i][j] = 2;

						}
				}
				else if (motigoma_statas_ene == 8) {//�����̏ꍇ�A������̂�����͈͂�������

					for (int i = 1; i < 9; i++)
						for (int j = 0; j < 9; j++) {

							if (Stage[j][i] == -1) {
							
								if (hohei_place_ene[j] == 0) {

									Stage_Place[i][j] = 2;

								}

							}
							else {

									Stage_Place[i][j] = 0;

							}

						}

				}

			}

		}

		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++) {

				if (Stage[i][j] != -1)
					Stage_Place[i][j] = 0;

			}
	}
	

	if (Mouse_Input() == MOUSE_INPUT_RIGHT) { //�E�N���b�N������A�N���b�N�t���b�O�ƃX�e�[�W�N���b�N��������

		Click_Flag = 0;
		/*
		motigoma_statas_me = 0;
		motigoma_statas_ene = 0;
		motigoma_number_me = 0;
		motigoma_number_ene = 0;
		motigoma_flag_me = 0;
		motigoma_flag_ene = 0;
		*/
		for (i = 0; i < 9+2; i++)
			for (j = 0; j < 9+2; j++)
				Stage_Click[j][i] = 0;

				

	}
	

	if (Keyboard_Get(KEY_INPUT_SPACE) == 1) {

		motigoma_statas_me = 0;
		motigoma_statas_ene = 0;
		motigoma_number_me = 0;
		motigoma_number_ene = 0;
		motigoma_flag_me = 0;
		motigoma_flag_ene = 0;

	}

	//�N���b�N�X�e�[�W���P�̍��W��ǂݎ��,���̎���,��̃X�e�[�^�X��ۑ�����

	if (Click_Flag == 1) {
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++) {

				if (Stage_Click[i][j] == 1) { //���N���b�N���ꂽ���W��

					Click_Statas = Stage[i][j]; //�N���b�N�X�e�[�^�X�Ɉ��N���b�N���ꂽ���̋������ۑ�����
					C_x = j;
					C_y = i;

				}

			}

	}

	//����̂��܂̑I���������_���ɍs��//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (hantei == 1) {

	}

}

//���̍��W�ɋ�u���邩�H

void Player_Place() {

	//C2���W����ɂ���Đ�������

	if (Click_Flag != 0) {//��I�����ꂽ��ԂŁA�A�A

						  //����
		//if (C_y < 3 && C_y > -1 && teban == 0) {
		/*
			if (Click_Statas == 10)			ou_way(C_x, C_y);
			else if (Click_Statas == 20)	kin_way(C_x, C_y);
			else if (Click_Statas == 30)	kin_way(C_x, C_y);
			else if (Click_Statas == 40)	hisha_way(C_x, C_y);
			else if (Click_Statas == 50)	kaku_way(C_x, C_y);
			else if (Click_Statas == 60)	kin_way(C_x, C_y);
			else if (Click_Statas == 70)	kin_way(C_x, C_y);
			else if (Click_Statas == 80)	kin_way(C_x, C_y);
		*/
		//}
		
		//else {

			if (Click_Statas == 10)			ou_way(C_x, C_y);
			else if (Click_Statas == 20)	kin_way(C_x, C_y);
			else if (Click_Statas == 30)	gin_way(C_x, C_y);
			else if (Click_Statas == 40)	hisha_way(C_x, C_y);
			else if (Click_Statas == 50)	kaku_way(C_x, C_y);
			else if (Click_Statas == 60)	keima_way(C_x, C_y);
			else if (Click_Statas == 70)	kou_way(C_x, C_y);
			else if (Click_Statas == 80)	hohei_way(C_x, C_y);

		//}

		//�G

		//if (C_y >= 3 && 8 <= C_y && teban == 1) {
		/*
			if (Click_Statas == 11)			ou_way_Enemy(C_x, C_y);
			else if (Click_Statas == 21)	kin_way_Enemy(C_x, C_y);
			else if (Click_Statas == 31)	kin_way_Enemy(C_x, C_y);
			else if (Click_Statas == 41)	hisha_way_Enemy(C_x, C_y);
			else if (Click_Statas == 51)	kaku_way_Enemy(C_x, C_y);
			else if (Click_Statas == 61)	kin_way_Enemy(C_x, C_y);
			else if (Click_Statas == 71)	kin_way_Enemy(C_x, C_y);
			else if (Click_Statas == 81)	kin_way_Enemy(C_x, C_y);
		*/
		//}
	
		//else {

			if (Click_Statas == 11)			ou_way_Enemy(C_x, C_y);
			else if (Click_Statas == 21)	kin_way_Enemy(C_x, C_y);
			else if (Click_Statas == 31)	gin_way_Enemy(C_x, C_y);
			else if (Click_Statas == 41)	hisha_way_Enemy(C_x, C_y);
			else if (Click_Statas == 51)	kaku_way_Enemy(C_x, C_y);
			else if (Click_Statas == 61)	keima_way_Enemy(C_x, C_y);
			else if (Click_Statas == 71)	kou_way_Enemy(C_x, C_y);
			else if (Click_Statas == 81)	hohei_way_Enemy(C_x, C_y);
		//}

	}

}

// ������i�����������j���L�^���A�u����悤�ɂ���

void Player_Motigoma() {

	// ��������L�^����

	int i, j;
	int X, Y;

	for (i = 0; i < 11; i++)
		for (j = 0; j < 11; j++)
			if (Stage_Mouse[i][j] == 1) {

				X = j;
				Y = i;

			}

	if (move == 1) {//����������ǂ����𒲂ׂ�


		//����
		if (pre_statas == 11)		Motigoma_Me[0] += 1;
		else if (pre_statas == 21)	Motigoma_Me[1] += 1;
		else if (pre_statas == 31)	Motigoma_Me[2] += 1;
		else if (pre_statas == 41)	Motigoma_Me[3] += 1;
		else if (pre_statas == 51)	Motigoma_Me[4] += 1;
		else if (pre_statas == 61)	Motigoma_Me[5] += 1;
		else if (pre_statas == 71)	Motigoma_Me[6] += 1;
		else if (pre_statas == 81)	Motigoma_Me[7] += 1;
		
		//�G
		if (pre_statas == 10)		Motigoma_Ene[0] += 1;
		else if (pre_statas == 20)	Motigoma_Ene[1] += 1;
		else if (pre_statas == 30)	Motigoma_Ene[2] += 1;
		else if (pre_statas == 40)	Motigoma_Ene[3] += 1;
		else if (pre_statas == 50)	Motigoma_Ene[4] += 1;
		else if (pre_statas == 60)	Motigoma_Ene[5] += 1;
		else if (pre_statas == 70)	Motigoma_Ene[6] += 1;
		else if (pre_statas == 80)	Motigoma_Ene[7] += 1;

		for (int i = 0; i < 9; i++) {

			hohei_place_me[i] = 0;
			hohei_place_ene[i] = 0;

		}
		
	}

}


// �}�E�X�̍��W���X�V����

void Player_MouseUpdate() {

	MouseDraw();

	int M_x, M_y, x, y; //�}�E�X��x�Ay���W�AStage��̍��W

	int i, j;

	GetMousePoint(&M_x, &M_y);//�}�E�X�̍��W�X�V

							  //32���݂ŁAStage�ɑΉ�������W�ɕϊ�
	x = M_x / 32;
	y = M_y / 32;

	if ((x < 9 && x > -1) && (y < 9 && y > -1)) {//�����Ղ̏�Ƀ}�E�X�̍��W�����邱��

			Stage_Mouse[y][x] = 1;						//��������W��1��������@�����}�E�X�̍��W�ɂ���R�}


	}

	if ((x < 8) && (y == 10)) {//�����̎�����̏�Ƀ}�E�X�̍��W�����邱��

		Stage_Mouse[y][x] = 1;						//��������W��1��������@�����}�E�X�̍��W�ɂ���R�}


	}

	if ((y < 8) && (x == 10)) {//����̎�����̏�Ƀ}�E�X�̍��W�����邱��

		Stage_Mouse[y][x] = 1;						//��������W��1��������@�����}�E�X�̍��W�ɂ���R�}


	}

	//����ȉ��A�}�E�X�̉e����������



	//������ꏊ�ȊO�͂O�ɏ���������(��������������j

	for (i = 0; i < 9 + 2 && i != y; i++)//�}�E�X�����W�ȊO
		for (j = 0; j < 9 + 2 && j != x; j++)//�}�E�Xx���W�ȊO
			Stage_Mouse[i][j] = 0;

	//������ꏊ�ȊO�͂O�ɏ���������i�E������������j

	for (i = 0; i < 9 + 2 && i != y; i++)//�}�E�X�����W�ȊO
		for (j = 8 + 2; j > -1 && j != x; j--)//�}�E�Xx���W�ȊO
			Stage_Mouse[i][j] = 0;

	//������ꏊ�ȊO�͂O�ɏ���������(��������������j

	for (i = 8 + 2; i > -1 && i != y; i--)//�}�E�X�����W�ȊO
		for (j = 0; j < 9 + 2 && j != x; j++)//�}�E�Xx���W�ȊO
			Stage_Mouse[i][j] = 0;

	//������ꏊ�ȊO�͂O�ɏ���������i�E������������j

	for (i = 8 + 2; i > -1 && i != y; i--)//�}�E�X�����W�ȊO
		for (j = 8 + 2; j > -1 && j != x; j--)//�}�E�Xx���W�ȊO
			Stage_Mouse[i][j] = 0;

	//������ꏊ�ȊO�͂O�ɏ���������i�ォ������j

	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][0] = 0;
	for (i = 0; i < 9 * 2 && i != y; i++)
		Stage_Mouse[i][1] = 0;
	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][2] = 0;
	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][3] = 0;
	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][4] = 0;
	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][5] = 0;
	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][6] = 0;
	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][7] = 0;
	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][8] = 0;
	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][9] = 0;
	for (i = 0; i < 9 + 2 && i != y; i++)
		Stage_Mouse[i][10] = 0;

	//������ꏊ�ȊO�͂O�ɏ���������i����������j
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][0] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][1] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][2] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][3] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][4] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][5] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][6] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][7] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][8] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][9] = 0;
	for (i = 8 + 2; i > -1 && i != y; i--)
		Stage_Mouse[i][10] = 0;


	//������ꏊ�ȊO�͂O������������i�E��������j

	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[0][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[1][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[2][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[3][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[4][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[5][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[6][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[7][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[8][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[9][i] = 0;
	for (i = 8 + 2; i > -1 && i != x; i--)
		Stage_Mouse[10][i] = 0;

	//������ꏊ�ȊO�͂O������������i����������j

	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[0][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[1][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[2][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[3][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[4][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[5][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[6][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[7][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[8][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[9][i] = 0;
	for (i = 0; i < 9 + 2 && i != x; i++)
		Stage_Mouse[10][i] = 0;



}

// �`�悷��
void Player_Draw() {

	//�X�e�[�W�̐}�`�\��

	//DrawFormatString(11 * 32 + 40, 50, GetColor(255, 255, 255), "move %d", move);

	
	//����

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {

			if (Stage[j][i] == 1) //�����Ȃ��i�O�j����������{�b�N�X��\��
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(200, 220, 50), true);

			else if (Stage[j][i] == 10)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(255, 0, 0), true);

			else if (Stage[j][i] == 20)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 225, 0), true);

			else if (Stage[j][i] == 30)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 0, 255), true);

			else if (Stage[j][i] == 40)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(100, 100, 0), true);

			else if (Stage[j][i] == 50)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 100, 100), true);

			else if (Stage[j][i] == 60)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(50, 0, 100), true);

			else if (Stage[j][i] == 70)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 50, 100), true);

			else if (Stage[j][i] == 80)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(100, 50, 0), true);

		}

	//�G

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {

			if (Stage[j][i] == -1) //�����Ȃ��i�O�j����������{�b�N�X��\��
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(200, 220, 50), true);

			else if (Stage[j][i] == 11)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(255, 0, 0), true);

			else if (Stage[j][i] == 21)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 225, 0), true);

			else if (Stage[j][i] == 31)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 0, 255), true);

			else if (Stage[j][i] == 41)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(100, 100, 0), true);

			else if (Stage[j][i] == 51)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 100, 100), true);

			else if (Stage[j][i] == 61)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(50, 0, 100), true);

			else if (Stage[j][i] == 71)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(0, 50, 100), true);

			else if (Stage[j][i] == 81)
				DrawBox(i * 32, j * 32, (i + 1) * 32, (j + 1) * 32, GetColor(100, 50, 0), true);

		}
		

	//������@����

	int i = 10, j = 0;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(255, 0, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	j = 1;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 225, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	j = 2;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 0, 255), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	j = 3;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(100, 100, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	j = 4;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 100, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "�p");

	j = 5;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(50, 0, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "�j");

	j = 6;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(50, 0, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	j = 7;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(100, 50, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	//�G

	j = 10, i = 0;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(255, 0, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	i = 1;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 225, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	i = 2;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 0, 255), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	i = 3;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(100, 100, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	i = 4;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 100, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "�p");

	i = 5;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(50, 0, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "�j");

	i = 6;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(50, 0, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

	i = 7;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(100, 50, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");


	//������̕\��
	for (i = 0; i < 8; i++)
		DrawFormatString(i * 32, 11 * 32, GetColor(255, 255, 255), "%d", Motigoma_Me[i]);

	//������̕\��
	for (i = 0; i < 8; i++)
		DrawFormatString(11 * 32, i * 32, GetColor(255, 255, 255), "%d", Motigoma_Ene[i]);


	//�`��̉e������(����Ȃ����ۂ��A������J������ƁA�P�E�P���W���O�ɂȂ�
	/*
	if (Click_Flag != 2) {
	Stage[pre_y][pre_x] = 0;
	DrawBox(pre_x * 32, pre_y * 32, (pre_x + 1) * 32, (pre_y + 1) * 32,
	GetColor(0, 255, 0), true);

	}
	*/


	//�X�e�[�W�i��̖��j��\������

	
	//����

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {

			if (Stage[i][j] == 0)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "");
			else if (Stage[i][j] == 10)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");
			else if (Stage[i][j] == 20)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");
			else if (Stage[i][j] == 30)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");
			else if (Stage[i][j] == 40)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");
			else if (Stage[i][j] == 50)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "�p");
			else if (Stage[i][j] == 60)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");
			else if (Stage[i][j] == 70)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");
			else if (Stage[i][j] == 80)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "��");

		}

	//�G

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {

			if (Stage[i][j] == 0)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "");
			else if (Stage[i][j] == 11)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "��");
			else if (Stage[i][j] == 21)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "��");
			else if (Stage[i][j] == 31)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "��");
			else if (Stage[i][j] == 41)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "��");
			else if (Stage[i][j] == 51)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "�p");
			else if (Stage[i][j] == 61)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "��");
			else if (Stage[i][j] == 71)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "��");
			else if (Stage[i][j] == 81)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "��");

		}

	//�I�𒆂̋�Ɉ������

	if (Click_Statas == 0)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "");
	else if (Click_Statas == 10 || Click_Statas == 11)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "��");
	else if (Click_Statas == 20 || Click_Statas == 21)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "��");
	else if (Click_Statas == 30 || Click_Statas == 31)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "��");
	else if (Click_Statas == 40 || Click_Statas == 41)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "��");
	else if (Click_Statas == 50 || Click_Statas == 51)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "�p");
	else if (Click_Statas == 60 || Click_Statas == 61)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "��");
	else if (Click_Statas == 70 || Click_Statas == 71)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "��");
	else if (Click_Statas == 80 || Click_Statas == 81)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "��");


	//�X�e�[�W�}�E�X��\������
	/*
	
	for (int i = 0; i < 11; i++)
	for (int j = 0; j < 11; j++) {

	if (Stage_Mouse[j][i] == 1) //1��������h�P�h�ƕ\��
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "�P");
	else if (Stage_Mouse[j][i] == 0) //�O��������h�O�h�ƕ\��
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "�O");

	}
	*/
	

	/*
	//�X�e�[�W�N���b�N��\������
	for (int i = 0; i < 9; i++)
	for (int j = 0; j < 9; j++) {

	DrawFormatString(32 * j, 32 * i, GetColor(255, 255, 255), "%d", Stage_Click[i][j]);

	}

	*/

	//�X�e�[�W�����\������
	/*

	for (int i = 0; i < 9; i++)
	for (int j = 0; j < 9; j++) {

	if (Stage_Hantei[j][i] == -1)
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 0, 0), "-1");
	else if (Stage_Hantei[j][i] == 1) //1��������h�P�h�ƕ\��
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "1");
	else if (Stage_Hantei[j][i] == 0) //�O��������h�O�h�ƕ\��
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "0");

	}


	//�X�e�[�W�v���C�X��\������
	*/
	
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 11; j++) {

			if (Stage_Place[j][i] == 2)
				DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 0, 0), "�E");
			else if (Stage_Place[j][i] == 1) //1��������h�P�h�ƕ\��
				DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(0, 255, 0), "");
			else if (Stage_Place[j][i] == 0) //�O��������h�O�h�ƕ\��
				DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(0, 0, 255), "");
			else 
				DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "");
		}

		
	//�X�e�[�W�̐��l���o�͂���

	/*
	for (int i = 0; i < 9; i++)
	for (int j = 0; j < 9; j++)
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "%d", Stage[j][i]);
	
	
	//�������\������

	for (int i = 0; i < 8; i++) {

	DrawFormatString(i * 32 + 10, 9 * 32 + 10, GetColor(255, 0, 0), "%d", Motigoma_Me[i]);
	DrawFormatString(i * 32 + 10, 9 * 32 + 30, GetColor(0, 0, 255), "%d", Motigoma_Ene[i]);

	}
	*/

	//�����v���C�X��\������
	/*
	for (int i = 0; i < 9; i++) {

		DrawFormatString(i * 32 + 10, 11 * 32 + 30, GetColor(255, 255, 255), "%d", hohei_place_me[i]);
		DrawFormatString(i * 32 + 10, 11 * 32 + 50, GetColor(255, 255, 255), "%d", hohei_place_ene[i]);

	}
	*/
	//��Ղ̋�؂��`�悷��

	//�c��
	for (int i = 0; i < 11; i++) {

		DrawBox(i * 32, 0, i * 32 + 1, 32 * 9, GetColor(0, 0, 0), true);

	}

	//����
	for (int i = 0; i < 11; i++) {

		DrawBox(0, i * 32 + 1, 32 * 9, i * 32, GetColor(0, 0, 0), true);

	}

	//��N���b�N���ꂽ�Ƃ��A��������A�ǂ��ɓ�������
	/*
	if (Click_Flag == 1 || Click_Flag == 2) {

	DrawFormatString(32 * 9 + 10, 8, GetColor(255, 255, 255), "���ڂɃN���b�N�������W(��������)");
	DrawFormatString(32 * 9 + 10, 25, GetColor(255, 255, 255), "��(%d, %d)�ł��B", C_x + 1, C_y + 1);

	DrawFormatString(32 * 9 + 10, 51, GetColor(255, 255, 255), "���ڈȍ~�ɃN���b�N�������W(���̎�)");
	DrawFormatString(32 * 9 + 10, 68, GetColor(255, 255, 255), "��(%d, %d)�ł��B", C2_x + 1, C2_y + 1);

	}
	*/



	//�N���b�N�t���b�O��\������
	//DrawFormatString(32 * 11, 100, GetColor(255, 255, 255), "�N���b�N�t���b�O��%d�ł��B", Click_Flag);


	//�N���b�N�������W��\������
	//DrawFormatString(32 * 11, 10, GetColor(255, 255, 255), "�N���b�N�������W��x�c%d, y�c%d�ł��B", M_x + 1, M_y + 1);

	//�N���b�N���ꂽ�Ƃ���ɂ����X�e�[�^�X�̕\�����s��

	/*
	if (Click_Flag != 0) {

	if (Click_Statas == 1)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "���������\"���@\"�ł��B");
	if (Click_Statas == 2)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "���������\"���@\"�ł��B");
	if (Click_Statas == 3)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "���������\"��@\"�ł��B");
	if (Click_Statas == 4)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "���������\"���\"�ł��B");
	if (Click_Statas == 5)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "���������\"�p�s\"�ł��B");
	if (Click_Statas == 6)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "���������\"�j�n\"�ł��B");
	if (Click_Statas == 7)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "���������\"���s\"�ł��B");
	if (Click_Statas == 8)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "���������\"����\"�ł��B");

	}

	*/
	//�N���b�N�t���b�O�O����Ȃ����A�����ɒu���邩�ǂ����H
	/*
	if (Click_Flag != 0) {

		if (Can_Place == 1)
			DrawFormatString(32 * 9 + 20, 260, GetColor(255, 255, 255), "�u����I");
		else
			DrawFormatString(32 * 9 + 20, 260, GetColor(255, 255, 255), "�u���Ȃ�");

	}
	*/

	//���̎萔��\������

	DrawFormatString(32 * 9 + 20, 280, GetColor(255, 255, 255), "���A%d ��ڂł��B", fights / 2);

	//���̎�Ԃ�\������
	if (teban == 0)
		DrawFormatString(32 * 9 + 20, 300, GetColor(255, 255, 255), "���̎�Ԃ�\"����\"�ł��B");
	else if (teban == 1)
		DrawFormatString(32 * 9 + 20, 300, GetColor(255, 255, 255), "���̎�Ԃ�\"����\"�ł��B");

	//���N���b�N���Ă���͓̂G��������
	if (hantei == 0)
		DrawFormatString(32 * 9 + 20, 320, GetColor(255, 255, 255), "���̋��\"����\"�ł��B");
	else if (hantei == 1)
		DrawFormatString(32 * 9 + 20, 320, GetColor(255, 255, 255), "���̋��\"����\"�ł��B");

	//�����������Ƃ�m�点��
	if (Motigoma_Me[0] == 1)
		DrawFormatString(32 * 11 + 50, 200, GetColor(255, 0, 0), "���Ȃ��̏����ł��B");

	else if (Motigoma_Ene[0] == 1)
		DrawFormatString(32 * 11 + 50, 200, GetColor(0, 0, 255), "���Ȃ��̕����ł��B");

	//DrawFormatString(20 ,32 * 11 + 20, GetColor(255,255, 255), "���������̎�����A�E������̎�����");
	DrawFormatString(20, 32 * 11 + 40, GetColor(255, 255, 255), "���N���b�N�c��I���@�E�N���b�N�c�I�����Z�b�g�@�X�y�[�X�c���u��");
	DrawFormatString(70, 32 * 9 + 10, GetColor(255, 255, 255), "�����̎�����");
	DrawFormatString(32 * 9 + 10, 70, GetColor(255, 255, 255), "��");
	DrawFormatString(32 * 9 + 10, 90, GetColor(255, 255, 255), "��");
	DrawFormatString(32 * 9 + 10, 110, GetColor(255, 255, 255), "��");
	DrawFormatString(32 * 9 + 10, 130, GetColor(255, 255, 255), "��");
	DrawFormatString(32 * 9 + 10, 150, GetColor(255, 255, 255), "��");
	DrawFormatString(32 * 9 + 10, 170, GetColor(255, 255, 255), "��");


	//DrawFormatString(32 * 11, 17, GetColor(255, 255, 255), "mitigoma_statas_ene = %d", motigoma_statas_ene);
	//DrawFormatString(32 * 11, 34, GetColor(255, 255, 255), "motigoma_number_ene = %d", motigoma_number_ene);
	//DrawFormatString(32 * 11, 34 + 17, GetColor(255, 255, 255), "mitigoma_statas_me = %d", motigoma_statas_me);
	//DrawFormatString(32 * 11, 34 * 2, GetColor(255, 255, 255), "motigoma_number_me = %d", motigoma_number_me);
	//DrawFormatString(32 * 11, 32 * 11 + 51, GetColor(255, 255, 255), "motigoma_flag_me = %d", motigoma_flag_me);
	//DrawFormatString(32 * 11, 32 * 11 + 51 + 17, GetColor(255, 255, 255), "motigoma_flag_ene = %d", motigoma_flag_ene);
	//DrawFormatString(32 * 11, 32 * 11 + 68 + 17, GetColor(255, 255, 255), "teban = %d", teban);
	//DrawFormatString(32 * 11, 32 * 11 + 68 + 17, GetColor(255, 255, 255), "hantei = %d", hantei);
	//DrawFormatString(32 * 11, 32 * 11 + 85 + 17, GetColor(255, 255, 255), "fights = %d", fights);
	//DrawFormatString(32 * 7, 32 * 11 + 85, GetColor(255, 255, 255), "C2_x = %d", C2_x);
	//DrawFormatString(32 * 7, 32 * 11 + 85 + 17, GetColor(255, 255, 255), "C_y = %d", C_y);
	//DrawFormatString(32 * 7, 32 * 11 + 17 + 17, GetColor(255, 255, 255), "pre_statas = %d", pre_statas);
	//DrawFormatString(32 * 11, 32 * 11 + 51 + 17, GetColor(255, 255, 255), "Click_Flag = %d", Click_Flag);

}

//������ȉ��AStatas.cpp �ŃC���N���[�h����"Statas.h"���A�ǂ����Ă���d�C���N���[�h����Ă��܂��i�C���N���[�h�K�[�h����Ȃ��A�����炭�A�����J�̖��j�̂ŁA
//�R�}�ɂ���āA���ڈȍ~�ɃN���b�N���ꂽ���W�𐧌�����i�G���[�R�[�hLNK2005�@

//������ȉ��A�����̓���

void ou_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 0)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//�E��
	if (Stage_Hantei[y_1 + 1][x_1] != 0)		Stage_Place[y_1 + 1][x_1] = 2;		//��
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 0)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//����
	if (Stage_Hantei[y_1][x_1 + 1] != 0)		Stage_Place[y_1][x_1 + 1] = 2;		//�E
	if (Stage_Hantei[y_1][x_1 - 1] != 0)		Stage_Place[y_1][x_1 - 1] = 2;		//��
	if (Stage_Hantei[y_1 - 1][x_1 + 1] != 0)		Stage_Place[y_1 - 1][x_1 + 1] = 2;	//�E��
	if (Stage_Hantei[y_1 - 1][x_1] != 0)		Stage_Place[y_1 - 1][x_1] = 2;		//��
	if (Stage_Hantei[y_1 - 1][x_1 - 1] != 0)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//����

}

void kin_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 0)	Stage_Place[y_1 + 1][x_1] = 2;		//��
	if (Stage_Hantei[y_1][x_1 + 1] != 0)			Stage_Place[y_1][x_1 + 1] = 2;		//�E
	if (Stage_Hantei[y_1][x_1 - 1] != 0)		Stage_Place[y_1][x_1 - 1] = 2;		//��
	if (Stage_Hantei[y_1 - 1][x_1 + 1] != 0)	Stage_Place[y_1 - 1][x_1 + 1] = 2;	//�E��
	if (Stage_Hantei[y_1 - 1][x_1] != 0)		Stage_Place[y_1 - 1][x_1] = 2;		//��
	if (Stage_Hantei[y_1 - 1][x_1 - 1] != 0)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//����

}

void gin_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 0)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//�E��
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 0)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//����
	if (Stage_Hantei[y_1 - 1][x_1 + 1] != 0)	Stage_Place[y_1 - 1][x_1 + 1] = 2;	//�E��
	if (Stage_Hantei[y_1 - 1][x_1] != 0)		Stage_Place[y_1 - 1][x_1] = 2;		//��
	if (Stage_Hantei[y_1 - 1][x_1 - 1] != 0)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//����

}

void keima_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 - 2][x_1 + 1] != 0)	Stage_Place[y_1 - 2][x_1 + 1] = 2;	//�E���
	if (Stage_Hantei[y_1 - 2][x_1 - 1] != 0)	Stage_Place[y_1 - 2][x_1 - 1] = 2;	//�����

}

void hohei_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 - 1][x_1] != 0)	Stage_Place[y_1 - 1][x_1] = 2;		//��

}

void kou_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	for (int i = (y_1 - 1); i > -1; i--) {//�O����
		if (Stage_Hantei[i][x_1] == 0) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[i + 1][x_1] == 1) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[i][x_1] = 2;
	}
}

void hisha_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	int i;

	for (int i = (y_1 - 1); i > -1; i--) {//�O����
		if (Stage_Hantei[i][x_1] == 0) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[i + 1][x_1] == 1) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[i][x_1] = 2;
	}
	for (i = (x_1 - 1); i > -1; i--) { //������
		if (Stage_Hantei[y_1][i] == 0) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[y_1][i + 1] == 1) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[y_1][i] = 2;
	}
	for (i = (y_1 + 1); i < 9; i++) { //�����]

		if (Stage_Hantei[i][x_1] == 0) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[i - 1][x_1] == 1) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[i][x_1] = 2;
	}
	for (i = (x_1 + 1); i < 9; i++) {//�E����
		if (Stage_Hantei[y_1][i] == 0) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[y_1][i - 1] == 1) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[y_1][i] = 2;
	}

}

void kaku_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	for (int i = 1; i < 9; i++) {	//�E�΂߉�
		if (Stage_Hantei[y_1 + i][x_1 + i] == 0) break;
		if (Stage_Hantei[y_1 + i - 1][x_1 + i - 1] == 1) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 + i][x_1 + i] = 2;
	}
	for (int i = 1; i < 9; i++) {//�E�΂ߏ�
		if (Stage_Hantei[y_1 - i][x_1 + i] == 0) break;
		if (Stage_Hantei[y_1 - i + 1][x_1 + i - 1] == 1) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 - i][x_1 + i] = 2;
	}
	for (int i = 1; i < 9; i++) {
		if (Stage_Hantei[y_1 + i][x_1 - i] == 0) break;
		if (Stage_Hantei[y_1 + i - 1][x_1 - i + 1] == 1) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 + i][x_1 - i] = 2;	//���΂߉�
	}
	for (int i = 1; i < 9; i++) {
		if (Stage_Hantei[y_1 - i][x_1 - i] == 0) break;
		if (Stage_Hantei[y_1 - i + 1][x_1 - i + 1] == 1) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 - i][x_1 - i] = 2;	//���΂ߏ�
	}

}

//�@�������A�G

void ou_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 1)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//�E��
	if (Stage_Hantei[y_1 + 1][x_1] != 1)		Stage_Place[y_1 + 1][x_1] = 2;		//��
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 1)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//����
	if (Stage_Hantei[y_1][x_1 + 1] != 1)		Stage_Place[y_1][x_1 + 1] = 2;		//�E
	if (Stage_Hantei[y_1][x_1 - 1] != 1)		Stage_Place[y_1][x_1 - 1] = 2;		//��

	if (C_y != 0) {
		if (Stage_Hantei[y_1 - 1][x_1 + 1] != 1)		Stage_Place[y_1 - 1][x_1 + 1] = 2;	//�E��
		if (Stage_Hantei[y_1 - 1][x_1] != 1)		Stage_Place[y_1 - 1][x_1] = 2;		//��
		if (Stage_Hantei[y_1 - 1][x_1 - 1] != 1)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//����
	}

}

void kin_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1] != 1)		Stage_Place[y_1 + 1][x_1] = 2;		//��
	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 1)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//�E��
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 1)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//����
	if (Stage_Hantei[y_1][x_1 + 1] != 1)		Stage_Place[y_1][x_1 + 1] = 2;		//�E
	if (Stage_Hantei[y_1][x_1 - 1] != 1)		Stage_Place[y_1][x_1 - 1] = 2;		//��
	if (C_y != 0) {
		if (Stage_Hantei[y_1 - 1][x_1] != 1)		Stage_Place[y_1 - 1][x_1] = 2;		//��
	}
}

void gin_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1] != 1)		Stage_Place[y_1 + 1][x_1] = 2;		//��
	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 1)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//�E��
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 1)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//����
	if (C_y != 0) {
		if (Stage_Hantei[y_1 - 1][x_1 + 1] != 1)	Stage_Place[y_1 - 1][x_1 + 1] = 2;	//�E��
		if (Stage_Hantei[y_1 - 1][x_1 - 1] != 1)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//����
	}

}

void keima_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 2][x_1 + 1] != 1)	Stage_Place[y_1 + 2][x_1 + 1] = 2;	//�E���
	if (Stage_Hantei[y_1 + 2][x_1 - 1] != 1)	Stage_Place[y_1 + 2][x_1 - 1] = 2;	//�����

}

void hohei_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1] != 1)	Stage_Place[y_1 + 1][x_1] = 2;		//��



}

void kou_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	int i;

	for (i = (y_1 + 1); i < 9; i++) { //�O����

		if (Stage_Hantei[i][x_1] == 1) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[i - 1][x_1] == 0) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[i][x_1] = 2;
	}
}

void hisha_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	int i;

	for (int i = (y_1 - 1); i > -1; i--) {//�O����
		if (Stage_Hantei[i][x_1] == 1) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[i + 1][x_1] == 0) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[i][x_1] = 2;
	}
	for (i = (x_1 - 1); i > -1; i--) { //������
		if (Stage_Hantei[y_1][i] == 1) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[y_1][i + 1] == 0) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[y_1][i] = 2;
	}
	for (i = (y_1 + 1); i < 9; i++) { //�����]

		if (Stage_Hantei[i][x_1] == 1) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[i - 1][x_1] == 0) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[i][x_1] = 2;
	}
	for (i = (x_1 + 1); i < 9; i++) {//�E����

		if (Stage_Hantei[y_1][i] == 1) break;//����������Ƃ��납���ɂ͒u���Ȃ�
		if (Stage_Hantei[y_1][i - 1] == 0) break;//�G���Ƃ�����z������u���Ȃ�
		Stage_Place[y_1][i] = 2;
	}


}

void kaku_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	for (int i = 1; i < 9; i++) {	//�E�΂߉�
		if (Stage_Hantei[y_1 + i][x_1 + i] == 1) break;
		if (Stage_Hantei[y_1 + i - 1][x_1 + i - 1] == 0) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 + i][x_1 + i] = 2;
	}
	for (int i = 1; i < 9; i++) {//�E�΂ߏ�
		if (Stage_Hantei[y_1 - i][x_1 + i] == 1) break;
		if (Stage_Hantei[y_1 - i + 1][x_1 + i - 1] == 0) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 - i][x_1 + i] = 2;
	}
	for (int i = 1; i < 9; i++) {
		if (Stage_Hantei[y_1 + i][x_1 - i] == 1) break;
		if (Stage_Hantei[y_1 + i - 1][x_1 - i + 1] == 0) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 + i][x_1 - i] = 2;	//���΂߉�
	}
	for (int i = 1; i < 9; i++) {
		if (Stage_Hantei[y_1 - i][x_1 - i] == 1) break;
		if (Stage_Hantei[y_1 - i + 1][x_1 - i + 1] == 0) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 - i][x_1 - i] = 2;	//���΂ߏ�
	}
	
}