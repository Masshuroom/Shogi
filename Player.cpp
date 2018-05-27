#include "DxLib.h"
#include "Keyboard.h"
#include "Player.h"
#include "Stage.h"
#include "Mouse.h"
#include "Statas.h"
#include <stdlib.h>

static int pre_x, pre_y;//描画更新直前の座標
static int nex_x, nex_y;//次の手の座標
static int pre_click_x, pre_click_y;//次の手を選択する
static int pre_xx = 5, pre_yy = 5;//注意、初期位置の時に、駒がない位置に置く
static int M_x, M_y; //マウスがクリックした座標
static int Click_Flag = 0;
static int Click_Statas; //クリックした座標の駒の役割
static int C_x, C_y; //マウスが一回目（ステージクリックが１の時）にクリックした座標
static int C2_x, C2_y; //マウスが二回目以降（ステージクリックが２の時）にクリックした座標
static int Can_Place; //今マウスのある座標に駒が置けるかどうか？（クリックステータス0以外の時）、1の時置ける
static int fights = 1;//今の手数
static int teban; //今、手番はみかたにあるか、敵にあるか（0…自分、1…敵）
static int hantei = 0; //選んだ駒が敵か味方か(0は味方、1は敵）
static int pre_statas; //とった駒のステータスを保存する
static int move; //駒を取った瞬間を判断する
static int motigoma_statas_me, motigoma_statas_ene, motigoma_number_me, motigoma_number_ene, motigoma_flag_me = 0, motigoma_flag_ene, motigoma_x, motigoma_y; 
			//選んだ持ち駒のステータス、数を保存する、持ち駒を選択しているか、クリックした持ち駒のx座標

static int Stage_Place[11][11] = { 0 };// 将棋盤のステージ（9×9）（0はそこに置けない、、１は選択した駒の位置、2は置くことが可能）
static int hohei_place_me[9] = { 0 }, hohei_place_ene[9] = { 0,};//将棋盤の歩兵の有無を確認する（持ち駒を置く時の判定）

									   // 動きを計算する
									   // この関数の入力…駒のステータス、（クリックした将棋盤の座標にその駒のステータスを入れる）

void Player_Update() {

	//ステージプレイスの探索（2の時しか置けない）

	int can;

	can = Stage_Place[C2_y][C2_x];

	Click_Statas = Stage[C_y][C_x];

	//二手目の更新
	if (Click_Flag == 2 && can == 2 && motigoma_flag_me == 0 && motigoma_flag_ene == 0) { //クリックフラッグが２を指している（次の手を選んでいる）、かつ、スペースが押されたとき、
																						  //かつ、そこに置くことが可能（canが２）
		if (Keyboard_Get(KEY_INPUT_SPACE) == 1) {

			for (int i = 0; i < 2; i++) { //この動作がないとなぜかうまくいかない

				Stage[pre_yy][pre_xx] = -1;

				Stage[C2_y][C2_x] = Click_Statas;	//二手目の選択先の座標に一手目目のステータスを代入

				pre_xx = C_x;
				pre_yy = C_y;

				//次につなげるために、クリックフラッグを０に初期化し、ステージプレイスを０に初期化する

				Click_Flag = 0;

				for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++)
						Stage_Place[i][j] = 0;

				fights += 1;

				teban = (fights / 2) % 2;//手番の更新

			}
		}

	}

	//持ち駒の更新

	//味方

	if (motigoma_flag_me == 1 && teban == 0 && C2_x < 9 && C2_y < 9) {

		if (Keyboard_Get(KEY_INPUT_SPACE) == 1) {

			pre_statas = 0;

			for (int i = 0; i < 2; i++) { //この動作がないとなぜかうまくいかない

				Stage[C2_y][C2_x] = motigoma_statas_me * 10;	//二手目の選択先の座標に一手目目のステータスを代入

																//次につなげるために、クリックフラッグを０に初期化し、ステージプレイスを０に初期化する

				motigoma_flag_me = 0;

				for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++)
						Stage_Place[i][j] = 0;

				fights += 1;

				teban = (fights / 2) % 2;//手番の更新

			}

			Motigoma_Me[motigoma_x] -= 1;

		}

	}

	//敵

	if (motigoma_flag_ene == 1 && teban == 1 && C2_x < 9 && C2_y < 9) {

		if (Keyboard_Get(KEY_INPUT_SPACE) == 1) {

			pre_statas = 0;

			for (int i = 0; i < 2; i++) { //この動作がないとなぜかうまくいかない

				Stage[M_y][M_x] = motigoma_statas_ene * 10 + 1;	//二手目の選択先の座標に一手目目のステータスを代入

																//次につなげるために、クリックフラッグを０に初期化し、ステージプレイスを０に初期化する

				motigoma_flag_ene = 0;

				for (int i = 0; i < 9; i++)
					for (int j = 0; j < 9; j++)
						Stage_Place[i][j] = 0;

				fights += 1;

				teban = (fights / 2) % 2;//手番の更新

			}

			Motigoma_Ene[motigoma_y] -= 1;

		}

	}


	//　ステージ判定の更新

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++)
			Stage_Hantei[i][j] = (Stage[i][j] % 10);

	// 駒をとった瞬間から、moveが増え続ける。駒を取ると、リセットされる　

	if (Keyboard_Get(KEY_INPUT_SPACE) >= 0 && move < 2)		move += 1;

	if (Click_Flag == 2 && M_x < 9 && M_y < 9)	move = 0;

	if (Click_Flag == 2) {

		pre_statas = Stage[C2_y][C2_x];

	}

	if (Mouse_Input() == MOUSE_INPUT_LEFT)
		if (M_x < 8 && M_y == 10 || M_x == 10 && M_y < 8)
			pre_statas = 0;

	//hohei_placeの更新
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {



			if (Stage[j][i] == 80) {//味方の歩兵があったら

				hohei_place_me[i] = 1;

			}
			else if (Stage[j][i] == 81) {

				hohei_place_ene[i] = 1;

			}

		}

}

// 駒を選択し、次の手を選択する

void Player_Select() {

	int i, j;

	//マウスの座標位置を読み取る
	for (i = 0; i < 11; i++)
		for (j = 0; j < 11; j++) {

			if (Stage_Mouse[i][j] == 1) { //マウスの座標を示す

				if (Mouse_Input() == MOUSE_INPUT_LEFT) {

					M_x = j;
					M_y = i;

				}

				if (M_x < 9 && M_y < 9) {

					hantei = Stage[i][j] % 10;
					Stage_Hantei[i][j] = Stage[i][j] % 10;

					if (Stage[i][j] != 0) { //駒が存在していたら

						if (Click_Flag == 0 && Mouse_Input() == MOUSE_INPUT_LEFT) { //左クリックされたら、クリックした座標を保存する

																					//手番が自分だったら、かつ、クリックしている駒が味方だったら
							if (hantei == 0 && teban == 0) {

								M_x = j;
								M_y = i;

								if (Click_Flag == 0) {//もし、クリックフラッグが０だったら（クリックされてなかったら）

									Stage_Click[M_y][M_x] = 1;

									Click_Flag = 1;

								}
							}



							//もし、二人プレイする時、ここを開放する



							//手番が相手だったら、かつ、クリックしている駒が敵だったら
							else if (hantei == 1 && teban == 1) {

								M_x = j;
								M_y = i;

								if (Click_Flag == 0) {//もし、クリックフラッグが０だったら（クリックされてなかったら）

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


	//駒を選択した状態で、右クリック（選択駒解除）されたら、ステージプレイスを０に初期化する
	
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
	
	if (Mouse_Input() == MOUSE_INPUT_LEFT) { //左クリックされたら、かつ、一回クリックされていたら、かつ、クリックした場所でなかったら

		if (Click_Flag != 0 && Stage_Click[M_y][M_x] == 0) {

			Stage_Click[pre_click_y][pre_click_x] = 0;

			//二回目以降にクリックした座標（次の手）を保存する
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

		// 持ち駒の選択を行う（味方）

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

				//歩兵の持ち駒の置ける場所の処理

				else if (motigoma_statas_me == 8) {//歩兵の場合、持ち駒のおける範囲が限られる

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

		//敵

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
				else if (motigoma_statas_ene == 8) {//歩兵の場合、持ち駒のおける範囲が限られる

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
	

	if (Mouse_Input() == MOUSE_INPUT_RIGHT) { //右クリックしたら、クリックフラッグとステージクリックを初期化

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

	//クリックステージが１の座標を読み取る,その時の,駒のステータスを保存する

	if (Click_Flag == 1) {
		for (int i = 0; i < 9; i++)
			for (int j = 0; j < 9; j++) {

				if (Stage_Click[i][j] == 1) { //一回クリックされた座標で

					Click_Statas = Stage[i][j]; //クリックステータスに一回クリックされたその駒役割を保存する
					C_x = j;
					C_y = i;

				}

			}

	}

	//相手のこまの選択をランダムに行う//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (hantei == 1) {

	}

}

//その座標に駒が置けるか？

void Player_Place() {

	//C2座標を駒によって制限する

	if (Click_Flag != 0) {//駒が選択された状態で、、、

						  //味方
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

		//敵

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

// 持ち駒（勝ち取った駒）を記録し、置けるようにする

void Player_Motigoma() {

	// 持ち駒を記録する

	int i, j;
	int X, Y;

	for (i = 0; i < 11; i++)
		for (j = 0; j < 11; j++)
			if (Stage_Mouse[i][j] == 1) {

				X = j;
				Y = i;

			}

	if (move == 1) {//駒が動いたかどうかを調べる


		//味方
		if (pre_statas == 11)		Motigoma_Me[0] += 1;
		else if (pre_statas == 21)	Motigoma_Me[1] += 1;
		else if (pre_statas == 31)	Motigoma_Me[2] += 1;
		else if (pre_statas == 41)	Motigoma_Me[3] += 1;
		else if (pre_statas == 51)	Motigoma_Me[4] += 1;
		else if (pre_statas == 61)	Motigoma_Me[5] += 1;
		else if (pre_statas == 71)	Motigoma_Me[6] += 1;
		else if (pre_statas == 81)	Motigoma_Me[7] += 1;
		
		//敵
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


// マウスの座標を更新する

void Player_MouseUpdate() {

	MouseDraw();

	int M_x, M_y, x, y; //マウスのx、y座標、Stage上の座標

	int i, j;

	GetMousePoint(&M_x, &M_y);//マウスの座標更新

							  //32刻みで、Stageに対応する座標に変換
	x = M_x / 32;
	y = M_y / 32;

	if ((x < 9 && x > -1) && (y < 9 && y > -1)) {//将棋盤の上にマウスの座標があること

			Stage_Mouse[y][x] = 1;						//今いる座標に1を代入する　★今マウスの座標にいるコマ


	}

	if ((x < 8) && (y == 10)) {//自分の持ち駒の上にマウスの座標があること

		Stage_Mouse[y][x] = 1;						//今いる座標に1を代入する　★今マウスの座標にいるコマ


	}

	if ((y < 8) && (x == 10)) {//相手の持ち駒の上にマウスの座標があること

		Stage_Mouse[y][x] = 1;						//今いる座標に1を代入する　★今マウスの座標にいるコマ


	}

	//これ以下、マウスの影を消す動作



	//今いる場所以外は０に初期化する(左うえから消す）

	for (i = 0; i < 9 + 2 && i != y; i++)//マウスｙ座標以外
		for (j = 0; j < 9 + 2 && j != x; j++)//マウスx座標以外
			Stage_Mouse[i][j] = 0;

	//今いる場所以外は０に初期化する（右うえから消す）

	for (i = 0; i < 9 + 2 && i != y; i++)//マウスｙ座標以外
		for (j = 8 + 2; j > -1 && j != x; j--)//マウスx座標以外
			Stage_Mouse[i][j] = 0;

	//今いる場所以外は０に初期化する(左したから消す）

	for (i = 8 + 2; i > -1 && i != y; i--)//マウスｙ座標以外
		for (j = 0; j < 9 + 2 && j != x; j++)//マウスx座標以外
			Stage_Mouse[i][j] = 0;

	//今いる場所以外は０に初期化する（右したから消す）

	for (i = 8 + 2; i > -1 && i != y; i--)//マウスｙ座標以外
		for (j = 8 + 2; j > -1 && j != x; j--)//マウスx座標以外
			Stage_Mouse[i][j] = 0;

	//今いる場所以外は０に初期化する（上から消す）

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

	//今いる場所以外は０に初期化する（下から消す）
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


	//今いる場所以外は０い初期化する（右から消す）

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

	//今いる場所以外は０い初期化する（左から消す）

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

// 描画する
void Player_Draw() {

	//ステージの図形表示

	//DrawFormatString(11 * 32 + 40, 50, GetColor(255, 255, 255), "move %d", move);

	
	//味方

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {

			if (Stage[j][i] == 1) //何もなし（０）だったら青いボックスを表示
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

	//敵

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {

			if (Stage[j][i] == -1) //何もなし（０）だったら青いボックスを表示
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
		

	//持ち駒　味方

	int i = 10, j = 0;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(255, 0, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "王");

	j = 1;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 225, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "金");

	j = 2;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 0, 255), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "銀");

	j = 3;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(100, 100, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "飛");

	j = 4;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 100, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "角");

	j = 5;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(50, 0, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "桂");

	j = 6;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(50, 0, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "香");

	j = 7;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(100, 50, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "歩");

	//敵

	j = 10, i = 0;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(255, 0, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "王");

	i = 1;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 225, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "金");

	i = 2;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 0, 255), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "銀");

	i = 3;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(100, 100, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "飛");

	i = 4;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(0, 100, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "角");

	i = 5;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(50, 0, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "桂");

	i = 6;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(50, 0, 100), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "香");

	i = 7;
	DrawBox(j * 32, i * 32, (j + 1) * 32, (i + 1) * 32, GetColor(100, 50, 0), true);
	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "歩");


	//持ち駒の表示
	for (i = 0; i < 8; i++)
		DrawFormatString(i * 32, 11 * 32, GetColor(255, 255, 255), "%d", Motigoma_Me[i]);

	//持ち駒の表示
	for (i = 0; i < 8; i++)
		DrawFormatString(11 * 32, i * 32, GetColor(255, 255, 255), "%d", Motigoma_Ene[i]);


	//描画の影を消す(いらないっぽい、これを開放すると、１・１座標が０になる
	/*
	if (Click_Flag != 2) {
	Stage[pre_y][pre_x] = 0;
	DrawBox(pre_x * 32, pre_y * 32, (pre_x + 1) * 32, (pre_y + 1) * 32,
	GetColor(0, 255, 0), true);

	}
	*/


	//ステージ（駒の役）を表示する

	
	//味方

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {

			if (Stage[i][j] == 0)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "");
			else if (Stage[i][j] == 10)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "王");
			else if (Stage[i][j] == 20)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "金");
			else if (Stage[i][j] == 30)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "銀");
			else if (Stage[i][j] == 40)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "飛");
			else if (Stage[i][j] == 50)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "角");
			else if (Stage[i][j] == 60)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "佳");
			else if (Stage[i][j] == 70)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "香");
			else if (Stage[i][j] == 80)	DrawFormatString(j * 32, i * 32, GetColor(255, 255, 255), "歩");

		}

	//敵

	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 9; j++) {

			if (Stage[i][j] == 0)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "");
			else if (Stage[i][j] == 11)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "王");
			else if (Stage[i][j] == 21)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "金");
			else if (Stage[i][j] == 31)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "銀");
			else if (Stage[i][j] == 41)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "飛");
			else if (Stage[i][j] == 51)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "角");
			else if (Stage[i][j] == 61)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "佳");
			else if (Stage[i][j] == 71)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "香");
			else if (Stage[i][j] == 81)	DrawFormatString(j * 32, i * 32, GetColor(0, 255, 255), "歩");

		}

	//選択中の駒に印をつける

	if (Click_Statas == 0)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "");
	else if (Click_Statas == 10 || Click_Statas == 11)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "王");
	else if (Click_Statas == 20 || Click_Statas == 21)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "金");
	else if (Click_Statas == 30 || Click_Statas == 31)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "銀");
	else if (Click_Statas == 40 || Click_Statas == 41)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "飛");
	else if (Click_Statas == 50 || Click_Statas == 51)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "角");
	else if (Click_Statas == 60 || Click_Statas == 61)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "佳");
	else if (Click_Statas == 70 || Click_Statas == 71)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "香");
	else if (Click_Statas == 80 || Click_Statas == 81)	DrawFormatString(C_x * 32, C_y * 32, GetColor(255, 0, 0), "歩");


	//ステージマウスを表示する
	/*
	
	for (int i = 0; i < 11; i++)
	for (int j = 0; j < 11; j++) {

	if (Stage_Mouse[j][i] == 1) //1だったら”１”と表示
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "１");
	else if (Stage_Mouse[j][i] == 0) //０だったら”０”と表示
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "０");

	}
	*/
	

	/*
	//ステージクリックを表示する
	for (int i = 0; i < 9; i++)
	for (int j = 0; j < 9; j++) {

	DrawFormatString(32 * j, 32 * i, GetColor(255, 255, 255), "%d", Stage_Click[i][j]);

	}

	*/

	//ステージ判定を表示する
	/*

	for (int i = 0; i < 9; i++)
	for (int j = 0; j < 9; j++) {

	if (Stage_Hantei[j][i] == -1)
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 0, 0), "-1");
	else if (Stage_Hantei[j][i] == 1) //1だったら”１”と表示
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "1");
	else if (Stage_Hantei[j][i] == 0) //０だったら”０”と表示
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "0");

	}


	//ステージプレイスを表示する
	*/
	
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 11; j++) {

			if (Stage_Place[j][i] == 2)
				DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 0, 0), "・");
			else if (Stage_Place[j][i] == 1) //1だったら”１”と表示
				DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(0, 255, 0), "");
			else if (Stage_Place[j][i] == 0) //０だったら”０”と表示
				DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(0, 0, 255), "");
			else 
				DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "");
		}

		
	//ステージの数値を出力する

	/*
	for (int i = 0; i < 9; i++)
	for (int j = 0; j < 9; j++)
	DrawFormatString(i * 32 + 10, j * 32 + 10, GetColor(255, 255, 255), "%d", Stage[j][i]);
	
	
	//持ち駒を表示する

	for (int i = 0; i < 8; i++) {

	DrawFormatString(i * 32 + 10, 9 * 32 + 10, GetColor(255, 0, 0), "%d", Motigoma_Me[i]);
	DrawFormatString(i * 32 + 10, 9 * 32 + 30, GetColor(0, 0, 255), "%d", Motigoma_Ene[i]);

	}
	*/

	//歩兵プレイスを表示する
	/*
	for (int i = 0; i < 9; i++) {

		DrawFormatString(i * 32 + 10, 11 * 32 + 30, GetColor(255, 255, 255), "%d", hohei_place_me[i]);
		DrawFormatString(i * 32 + 10, 11 * 32 + 50, GetColor(255, 255, 255), "%d", hohei_place_ene[i]);

	}
	*/
	//碁盤の区切りを描画する

	//縦線
	for (int i = 0; i < 11; i++) {

		DrawBox(i * 32, 0, i * 32 + 1, 32 * 9, GetColor(0, 0, 0), true);

	}

	//横線
	for (int i = 0; i < 11; i++) {

		DrawBox(0, i * 32 + 1, 32 * 9, i * 32, GetColor(0, 0, 0), true);

	}

	//駒がクリックされたとき、動かす駒、どこに動かすか
	/*
	if (Click_Flag == 1 || Click_Flag == 2) {

	DrawFormatString(32 * 9 + 10, 8, GetColor(255, 255, 255), "一回目にクリックした座標(動かす駒)");
	DrawFormatString(32 * 9 + 10, 25, GetColor(255, 255, 255), "は(%d, %d)です。", C_x + 1, C_y + 1);

	DrawFormatString(32 * 9 + 10, 51, GetColor(255, 255, 255), "二回目以降にクリックした座標(次の手)");
	DrawFormatString(32 * 9 + 10, 68, GetColor(255, 255, 255), "は(%d, %d)です。", C2_x + 1, C2_y + 1);

	}
	*/



	//クリックフラッグを表示する
	//DrawFormatString(32 * 11, 100, GetColor(255, 255, 255), "クリックフラッグは%dです。", Click_Flag);


	//クリックした座標を表示する
	//DrawFormatString(32 * 11, 10, GetColor(255, 255, 255), "クリックした座標はx…%d, y…%dです。", M_x + 1, M_y + 1);

	//クリックされたところにある駒ステータスの表示を行う

	/*
	if (Click_Flag != 0) {

	if (Click_Statas == 1)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "動かす駒は\"王　\"です。");
	if (Click_Statas == 2)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "動かす駒は\"金　\"です。");
	if (Click_Statas == 3)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "動かす駒は\"銀　\"です。");
	if (Click_Statas == 4)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "動かす駒は\"飛車\"です。");
	if (Click_Statas == 5)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "動かす駒は\"角行\"です。");
	if (Click_Statas == 6)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "動かす駒は\"桂馬\"です。");
	if (Click_Statas == 7)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "動かす駒は\"香行\"です。");
	if (Click_Statas == 8)
	DrawFormatString(32 * 9, 32 * 9 + 17, GetColor(255, 255, 255), "動かす駒は\"歩兵\"です。");

	}

	*/
	//クリックフラッグ０じゃない時、そこに置けるかどうか？
	/*
	if (Click_Flag != 0) {

		if (Can_Place == 1)
			DrawFormatString(32 * 9 + 20, 260, GetColor(255, 255, 255), "置ける！");
		else
			DrawFormatString(32 * 9 + 20, 260, GetColor(255, 255, 255), "置けない");

	}
	*/

	//今の手数を表示する

	DrawFormatString(32 * 9 + 20, 280, GetColor(255, 255, 255), "今、%d 手目です。", fights / 2);

	//今の手番を表示する
	if (teban == 0)
		DrawFormatString(32 * 9 + 20, 300, GetColor(255, 255, 255), "今の手番は\"自分\"です。");
	else if (teban == 1)
		DrawFormatString(32 * 9 + 20, 300, GetColor(255, 255, 255), "今の手番は\"相手\"です。");

	//今クリックしているのは敵か味方か
	if (hantei == 0)
		DrawFormatString(32 * 9 + 20, 320, GetColor(255, 255, 255), "その駒は\"自分\"です。");
	else if (hantei == 1)
		DrawFormatString(32 * 9 + 20, 320, GetColor(255, 255, 255), "その駒は\"相手\"です。");

	//勝利したことを知らせる
	if (Motigoma_Me[0] == 1)
		DrawFormatString(32 * 11 + 50, 200, GetColor(255, 0, 0), "あなたの勝利です。");

	else if (Motigoma_Ene[0] == 1)
		DrawFormatString(32 * 11 + 50, 200, GetColor(0, 0, 255), "あなたの負けです。");

	//DrawFormatString(20 ,32 * 11 + 20, GetColor(255,255, 255), "下が自分の持ち駒、右が相手の持ち駒");
	DrawFormatString(20, 32 * 11 + 40, GetColor(255, 255, 255), "左クリック…駒選択　右クリック…選択リセット　スペース…駒を置く");
	DrawFormatString(70, 32 * 9 + 10, GetColor(255, 255, 255), "自分の持ち駒");
	DrawFormatString(32 * 9 + 10, 70, GetColor(255, 255, 255), "相");
	DrawFormatString(32 * 9 + 10, 90, GetColor(255, 255, 255), "手");
	DrawFormatString(32 * 9 + 10, 110, GetColor(255, 255, 255), "の");
	DrawFormatString(32 * 9 + 10, 130, GetColor(255, 255, 255), "持");
	DrawFormatString(32 * 9 + 10, 150, GetColor(255, 255, 255), "ち");
	DrawFormatString(32 * 9 + 10, 170, GetColor(255, 255, 255), "駒");


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

//これより以下、Statas.cpp でインクルードした"Statas.h"が、どうしても二重インクルードされてしまう（インクルードガードされない、おそらく、リンカの問題）ので、
//コマによって、二回目以降にクリックされた座標を制限する（エラーコードLNK2005　

//これより以下、味方の動き

void ou_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 0)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//右下
	if (Stage_Hantei[y_1 + 1][x_1] != 0)		Stage_Place[y_1 + 1][x_1] = 2;		//下
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 0)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//左下
	if (Stage_Hantei[y_1][x_1 + 1] != 0)		Stage_Place[y_1][x_1 + 1] = 2;		//右
	if (Stage_Hantei[y_1][x_1 - 1] != 0)		Stage_Place[y_1][x_1 - 1] = 2;		//左
	if (Stage_Hantei[y_1 - 1][x_1 + 1] != 0)		Stage_Place[y_1 - 1][x_1 + 1] = 2;	//右上
	if (Stage_Hantei[y_1 - 1][x_1] != 0)		Stage_Place[y_1 - 1][x_1] = 2;		//上
	if (Stage_Hantei[y_1 - 1][x_1 - 1] != 0)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//左上

}

void kin_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 0)	Stage_Place[y_1 + 1][x_1] = 2;		//下
	if (Stage_Hantei[y_1][x_1 + 1] != 0)			Stage_Place[y_1][x_1 + 1] = 2;		//右
	if (Stage_Hantei[y_1][x_1 - 1] != 0)		Stage_Place[y_1][x_1 - 1] = 2;		//左
	if (Stage_Hantei[y_1 - 1][x_1 + 1] != 0)	Stage_Place[y_1 - 1][x_1 + 1] = 2;	//右上
	if (Stage_Hantei[y_1 - 1][x_1] != 0)		Stage_Place[y_1 - 1][x_1] = 2;		//上
	if (Stage_Hantei[y_1 - 1][x_1 - 1] != 0)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//左上

}

void gin_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 0)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//右下
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 0)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//左下
	if (Stage_Hantei[y_1 - 1][x_1 + 1] != 0)	Stage_Place[y_1 - 1][x_1 + 1] = 2;	//右上
	if (Stage_Hantei[y_1 - 1][x_1] != 0)		Stage_Place[y_1 - 1][x_1] = 2;		//上
	if (Stage_Hantei[y_1 - 1][x_1 - 1] != 0)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//左上

}

void keima_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 - 2][x_1 + 1] != 0)	Stage_Place[y_1 - 2][x_1 + 1] = 2;	//右二個上
	if (Stage_Hantei[y_1 - 2][x_1 - 1] != 0)	Stage_Place[y_1 - 2][x_1 - 1] = 2;	//左二個上

}

void hohei_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 - 1][x_1] != 0)	Stage_Place[y_1 - 1][x_1] = 2;		//上

}

void kou_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	for (int i = (y_1 - 1); i > -1; i--) {//前方向
		if (Stage_Hantei[i][x_1] == 0) break;//味方がいるところから先には置けない
		if (Stage_Hantei[i + 1][x_1] == 1) break;//敵がところを越したら置けない
		Stage_Place[i][x_1] = 2;
	}
}

void hisha_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	int i;

	for (int i = (y_1 - 1); i > -1; i--) {//前方向
		if (Stage_Hantei[i][x_1] == 0) break;//味方がいるところから先には置けない
		if (Stage_Hantei[i + 1][x_1] == 1) break;//敵がところを越したら置けない
		Stage_Place[i][x_1] = 2;
	}
	for (i = (x_1 - 1); i > -1; i--) { //左方向
		if (Stage_Hantei[y_1][i] == 0) break;//味方がいるところから先には置けない
		if (Stage_Hantei[y_1][i + 1] == 1) break;//敵がところを越したら置けない
		Stage_Place[y_1][i] = 2;
	}
	for (i = (y_1 + 1); i < 9; i++) { //後方向]

		if (Stage_Hantei[i][x_1] == 0) break;//味方がいるところから先には置けない
		if (Stage_Hantei[i - 1][x_1] == 1) break;//敵がところを越したら置けない
		Stage_Place[i][x_1] = 2;
	}
	for (i = (x_1 + 1); i < 9; i++) {//右方向
		if (Stage_Hantei[y_1][i] == 0) break;//味方がいるところから先には置けない
		if (Stage_Hantei[y_1][i - 1] == 1) break;//敵がところを越したら置けない
		Stage_Place[y_1][i] = 2;
	}

}

void kaku_way(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	for (int i = 1; i < 9; i++) {	//右斜め下
		if (Stage_Hantei[y_1 + i][x_1 + i] == 0) break;
		if (Stage_Hantei[y_1 + i - 1][x_1 + i - 1] == 1) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 + i][x_1 + i] = 2;
	}
	for (int i = 1; i < 9; i++) {//右斜め上
		if (Stage_Hantei[y_1 - i][x_1 + i] == 0) break;
		if (Stage_Hantei[y_1 - i + 1][x_1 + i - 1] == 1) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 - i][x_1 + i] = 2;
	}
	for (int i = 1; i < 9; i++) {
		if (Stage_Hantei[y_1 + i][x_1 - i] == 0) break;
		if (Stage_Hantei[y_1 + i - 1][x_1 - i + 1] == 1) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 + i][x_1 - i] = 2;	//左斜め下
	}
	for (int i = 1; i < 9; i++) {
		if (Stage_Hantei[y_1 - i][x_1 - i] == 0) break;
		if (Stage_Hantei[y_1 - i + 1][x_1 - i + 1] == 1) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 - i][x_1 - i] = 2;	//左斜め上
	}

}

//　これより先、敵

void ou_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 1)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//右下
	if (Stage_Hantei[y_1 + 1][x_1] != 1)		Stage_Place[y_1 + 1][x_1] = 2;		//下
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 1)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//左下
	if (Stage_Hantei[y_1][x_1 + 1] != 1)		Stage_Place[y_1][x_1 + 1] = 2;		//右
	if (Stage_Hantei[y_1][x_1 - 1] != 1)		Stage_Place[y_1][x_1 - 1] = 2;		//左

	if (C_y != 0) {
		if (Stage_Hantei[y_1 - 1][x_1 + 1] != 1)		Stage_Place[y_1 - 1][x_1 + 1] = 2;	//右上
		if (Stage_Hantei[y_1 - 1][x_1] != 1)		Stage_Place[y_1 - 1][x_1] = 2;		//上
		if (Stage_Hantei[y_1 - 1][x_1 - 1] != 1)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//左上
	}

}

void kin_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1] != 1)		Stage_Place[y_1 + 1][x_1] = 2;		//下
	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 1)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//右下
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 1)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//左下
	if (Stage_Hantei[y_1][x_1 + 1] != 1)		Stage_Place[y_1][x_1 + 1] = 2;		//右
	if (Stage_Hantei[y_1][x_1 - 1] != 1)		Stage_Place[y_1][x_1 - 1] = 2;		//左
	if (C_y != 0) {
		if (Stage_Hantei[y_1 - 1][x_1] != 1)		Stage_Place[y_1 - 1][x_1] = 2;		//上
	}
}

void gin_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1] != 1)		Stage_Place[y_1 + 1][x_1] = 2;		//下
	if (Stage_Hantei[y_1 + 1][x_1 + 1] != 1)	Stage_Place[y_1 + 1][x_1 + 1] = 2;	//右下
	if (Stage_Hantei[y_1 + 1][x_1 - 1] != 1)	Stage_Place[y_1 + 1][x_1 - 1] = 2;	//左下
	if (C_y != 0) {
		if (Stage_Hantei[y_1 - 1][x_1 + 1] != 1)	Stage_Place[y_1 - 1][x_1 + 1] = 2;	//右上
		if (Stage_Hantei[y_1 - 1][x_1 - 1] != 1)	Stage_Place[y_1 - 1][x_1 - 1] = 2;	//左上
	}

}

void keima_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 2][x_1 + 1] != 1)	Stage_Place[y_1 + 2][x_1 + 1] = 2;	//右二個下
	if (Stage_Hantei[y_1 + 2][x_1 - 1] != 1)	Stage_Place[y_1 + 2][x_1 - 1] = 2;	//左二個下

}

void hohei_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	if (Stage_Hantei[y_1 + 1][x_1] != 1)	Stage_Place[y_1 + 1][x_1] = 2;		//下



}

void kou_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	int i;

	for (i = (y_1 + 1); i < 9; i++) { //前方向

		if (Stage_Hantei[i][x_1] == 1) break;//味方がいるところから先には置けない
		if (Stage_Hantei[i - 1][x_1] == 0) break;//敵がところを越したら置けない
		Stage_Place[i][x_1] = 2;
	}
}

void hisha_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	int i;

	for (int i = (y_1 - 1); i > -1; i--) {//前方向
		if (Stage_Hantei[i][x_1] == 1) break;//味方がいるところから先には置けない
		if (Stage_Hantei[i + 1][x_1] == 0) break;//敵がところを越したら置けない
		Stage_Place[i][x_1] = 2;
	}
	for (i = (x_1 - 1); i > -1; i--) { //左方向
		if (Stage_Hantei[y_1][i] == 1) break;//味方がいるところから先には置けない
		if (Stage_Hantei[y_1][i + 1] == 0) break;//敵がところを越したら置けない
		Stage_Place[y_1][i] = 2;
	}
	for (i = (y_1 + 1); i < 9; i++) { //後方向]

		if (Stage_Hantei[i][x_1] == 1) break;//味方がいるところから先には置けない
		if (Stage_Hantei[i - 1][x_1] == 0) break;//敵がところを越したら置けない
		Stage_Place[i][x_1] = 2;
	}
	for (i = (x_1 + 1); i < 9; i++) {//右方向

		if (Stage_Hantei[y_1][i] == 1) break;//味方がいるところから先には置けない
		if (Stage_Hantei[y_1][i - 1] == 0) break;//敵がところを越したら置けない
		Stage_Place[y_1][i] = 2;
	}


}

void kaku_way_Enemy(int x_1, int y_1) {

	Stage_Place[y_1][x_1] = 1;

	for (int i = 1; i < 9; i++) {	//右斜め下
		if (Stage_Hantei[y_1 + i][x_1 + i] == 1) break;
		if (Stage_Hantei[y_1 + i - 1][x_1 + i - 1] == 0) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 + i][x_1 + i] = 2;
	}
	for (int i = 1; i < 9; i++) {//右斜め上
		if (Stage_Hantei[y_1 - i][x_1 + i] == 1) break;
		if (Stage_Hantei[y_1 - i + 1][x_1 + i - 1] == 0) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 - i][x_1 + i] = 2;
	}
	for (int i = 1; i < 9; i++) {
		if (Stage_Hantei[y_1 + i][x_1 - i] == 1) break;
		if (Stage_Hantei[y_1 + i - 1][x_1 - i + 1] == 0) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 + i][x_1 - i] = 2;	//左斜め下
	}
	for (int i = 1; i < 9; i++) {
		if (Stage_Hantei[y_1 - i][x_1 - i] == 1) break;
		if (Stage_Hantei[y_1 - i + 1][x_1 - i + 1] == 0) break;
		if (x_1 > 8 && y_1 > 8)					break;
		Stage_Place[y_1 - i][x_1 - i] = 2;	//左斜め上
	}
	
}