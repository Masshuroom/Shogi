#ifndef DEF_PLAYER_H //二重include防止

#define DEF_PLAYER_H

typedef struct {
	int x;
	int y;
	int statas;
} Player_t;

// 動きを計算する
void Player_Update(); //格納変数…Player_t型の変数とその駒のステータス

// マウスの碁盤上の座標を取得する
void Player_MouseUpdate();

// 駒を選択し、次の手を選択する
void Player_Select();

// 駒が置けるかどうか？
void Player_Place();

// 持ち駒の処理

void Player_Motigoma();

// 描画する
void Player_Draw();

// これ以下、置けるかどうかの判定の関数を示す（ステージプレイス更新）味方

void ou_way(int x_1, int y_1);

void kin_way(int x_1, int y_1);

void gin_way(int x_1, int y_1);

void keima_way(int x_1, int y_1);

void hohei_way(int x_1, int y_1);

void kou_way(int x_1, int y_1);

void hisha_way(int x_1, int y_1);

void kaku_way(int x_1, int y_1);


// これ以下、置けるかどうかの判定の関数を示す（ステージプレイス更新）敵

void ou_way_Enemy(int x_1, int y_1);

void kin_way_Enemy(int x_1, int y_1);

void gin_way_Enemy(int x_1, int y_1);

void keima_way_Enemy(int x_1, int y_1);

void hohei_way_Enemy(int x_1, int y_1);

void kou_way_Enemy(int x_1, int y_1);

void hisha_way_Enemy(int x_1, int y_1);

void kaku_way_Enemy(int x_1, int y_1);

#endif
