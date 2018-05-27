
#ifndef Mouse
#define Mouse

typedef struct {
	int x, y;
}Mouse_ch;

int Mouse_Input();

void MouseDraw();

void Mouse_Update();

#endif