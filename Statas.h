// 駒によって、二回目以降にクリックされた座標を制限する
// 失敗するため、読み飛ばす

#define Statas_
#ifndef Statas_

//入力情報…今のマウスとクリックフラッグ１の時のx座標・y座標、出力情報…コマが置けるかどうか？（０…不可能、１…可能）

//歩兵のコマ設置確認(x_1,y_1…クリックフラッグ１の時の座標、x_now, y_now…今の座標）、出力は今の座標が選択可能かどうか（０…不可能、１…可能）
int hohei_way(int x_1, int y_1, int x_now, int y_now);

//王、入力、出力は歩兵と同じ
void ou_way(int x_1, int y_1, int x_now, int y_now);

//金、入力、出力は歩兵と同じ
int kin_way(int x_1, int y_1, int x_now, int y_now);

//銀、入力、出力は歩兵と同じ
int gin_way(int x_1, int y_1, int x_now, int y_now);

//桂馬、入力、出力は歩兵と同じ
int keima_way(int x_1, int y_1, int x_now, int y_now);

//香車、入力、出力は歩兵と同じ
int kou_way(int x_1, int y_1, int x_now, int y_now);

//飛車、入力、出力は歩兵と同じ
int hisha_way(int x_1, int y_1, int x_now, int y_now);

//角行、入力、出力は歩兵と同じ

*/

#endif