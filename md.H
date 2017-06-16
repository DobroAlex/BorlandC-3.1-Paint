#include <dos.h>
#ifndef MD_H
#define MD_H
#ifndef __regs
#define __regs
struct REGPACK regs;
#endif

const int BOUND_HORIZONTAL = 0;
const int BOUND_VERTICAL = 1;

unsigned CURSOR_POINTER[17] = {0x0000,0x4000,0x3000,0x3C00,0x1F00,0x0FC0,0x0FF0,0x0FFC,
0x07FF,0x0780,0x0380,0x0380,0x0180,0x0180,0x0080,0x0080,0x1100};
unsigned CURSOR_PENCIL[17] = {0x001C,0x003F,0x0073,0x00B3,0x01BF,0x03DE,0x07E4,0x0BF8,
0x0BF0,0x1DE0,0x1E40,0x3F80,0x2E00,0x5800,0x6000,0x6000,0x0F01};
unsigned CURSOR_ERASER[17] = {0x0FC0, 0x1BE0, 0x3DF0, 0x7EF8, 0xFF7C,
0x7FBE, 0x3FDF, 0x1FE1, 0x0FDE, 0x0378, 0x0378, 0x01F0, 0, 0, 0, 0, 0x0402};
int selectedCursor = -1;

void intrm(int in) {
	regs.r_ax = in;
	intr(0x33, &regs);
}

void initMouse(void) {
	intrm(0);
}

void showMouse(void) {
	intrm(1);
}

void hideMouse(void) {
	intrm(2);
}

void setMousePos(int x, int y) {
	regs.r_cx = x;
	regs.r_dx = y;
	intrm(4);
}

void getMouseState(int &mX, int &mY, int &mB) {
	intrm(3);
	mX = regs.r_cx;
	mY = regs.r_dx;
	mB = regs.r_bx;
}

void setMouseBound(int type, int min, int max) {
	regs.r_cx = min;
	regs.r_dx = max;
	intrm(type+7);
}

int setMouseCursor(unsigned design[17]) {
	int num = design[16]%0x100;
	if(selectedCursor!=num) {
		unsigned full[32];
		for(int i=0; i<16; i++) {
			full[i] = (~design[i]);
			full[i+16] = design[i];
		}
		regs.r_bx = design[16]/0x1000;
		regs.r_cx = design[16]/0x100%0x10;;
		regs.r_es = FP_SEG(&full);
		regs.r_dx = FP_OFF(&full);
		intrm(9);
		selectedCursor = num;
	}
	return num;
}
#endif