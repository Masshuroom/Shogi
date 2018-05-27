// コマによって、二回目以降にクリックされた座標を制限する

//Player.cppで述べた通り、うまくいかないので、読み飛ばす(完成してない）

#define FALSE_
#ifndef FALSE_

#include "DxLib.h"
#include "Statas.h"
#include "Stage.h"


int hohei_way(int x_1, int y_1, int x_now, int y_now) {

	//if (x_now == x_1 && (y_1 + 1) == y_now) //x座標が同じ、かつ、y座標が一つ前の時（一歩前進)
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
		if (y_now == y_1 + 1 || y_now == y_1 || y_now == y_1 - 1) {//x座標がx_nowの両隣、かつ、y座標がy_nowの両隣
			if (x_now == x_1 - 1 && y_now == y_1 + 1)//選択コマの左下
				return 0;
			else if (x_now == x_1 + 1 && y_now == y_1 + 1)//選択コマの右下

				return 0;
			else if (x_now == x_1 && y_now == y_1)
				return 0;
			else return 1;
		}

	return 0;

}

int gin_way(int x_1, int y_1, int x_now, int y_now) {

	if (x_now == x_1 + 1 || x_now == x_1 || x_now == x_1 - 1)
		if (y_now == y_1 + 1 || y_now == y_1 || y_now == y_1 - 1) {//x座標がx_nowの両隣、かつ、y座標がy_nowの両隣

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