// �R�}�ɂ���āA���ڈȍ~�ɃN���b�N���ꂽ���W�𐧌�����

//Player.cpp�ŏq�ׂ��ʂ�A���܂������Ȃ��̂ŁA�ǂݔ�΂�(�������ĂȂ��j

#define FALSE_
#ifndef FALSE_

#include "DxLib.h"
#include "Statas.h"
#include "Stage.h"


int hohei_way(int x_1, int y_1, int x_now, int y_now) {

	//if (x_now == x_1 && (y_1 + 1) == y_now) //x���W�������A���Ay���W����O�̎��i����O�i)
		return 1;
	//else
		//return 0;

}


void ou_way(int x_1, int y_1, int x_now, int y_now) {

	Stage_Place[y_1][x_1] = 1;

	if (x_now == x_1 + 1 && y_now == y_1)

		Stage_Place[y_now][x_now] = 2;

}


int kin_way(int x_1, int y_1, int x_now, int y_now) {

	if (x_now == x_1 + 1 || x_now == x_1 || x_now == x_1 - 1)
		if (y_now == y_1 + 1 || y_now == y_1 || y_now == y_1 - 1) {//x���W��x_now�̗��ׁA���Ay���W��y_now�̗���
			if (x_now == x_1 - 1 && y_now == y_1 + 1)//�I���R�}�̍���
				return 0;
			else if (x_now == x_1 + 1 && y_now == y_1 + 1)//�I���R�}�̉E��

				return 0;
			else if (x_now == x_1 && y_now == y_1)
				return 0;
			else return 1;
		}

	return 0;

}

int gin_way(int x_1, int y_1, int x_now, int y_now) {

	if (x_now == x_1 + 1 || x_now == x_1 || x_now == x_1 - 1)
		if (y_now == y_1 + 1 || y_now == y_1 || y_now == y_1 - 1) {//x���W��x_now�̗��ׁA���Ay���W��y_now�̗���

			if (x_now == x_1 + 1 && y_now == y_1)
				return 0;
			else if (x_now == x_1 - 1 && y_now == y_1)
				return 0;
			else if (x_now == x_1 && y_now == y_1 + 1)
				return 0;

		}

		else return 0;

}

int keima_way(int x_1, int y_1, int x_now, int y_now) {

	if (x_now == x_1 - 1 || x_now == x_1 + 1) {

		if (y_now == y_1 - 2)

			return 1;

	}
	else return 0;

}

int kou_way(int x_1, int y_1, int x_now, int y_now) {

	if (x_now == x_1) {

		if (y_now < y_1 && y_now >= 0)
			return 1;

	}
	else return 0;

}

int hisha_way(int x_1, int y_1, int x_now, int y_now) {

	if (x_now == x_1) {

		return 1;

	}
	else if (y_now == y_1) {

		return 1;

	}
	else return 0;

}


#endif