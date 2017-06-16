#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <graphics.h>
#include <bios.h>
#define KB_TAB 0x0F09 
#define KB_ESC 0x011B
#define KB_ENTER 0x1C0D
#define KB_BSP 0x0E08
#define NUM_PLUS 0x4E2B //увеличить размер шрифта,например
#define NUM_MINUS 0x4A2D
#define KB_PLUS 0x0D3D
#define KB_MINUS 0x02CD
#define KB_END 0x4F00
#define KB_UA 0x4800 // up arrow
#define KB_DA 0x5000 //Down appow
#define KB_RA 0x4D00 // right arrow
#define KB_LA 0x4B00 //left arrow
#define KB_F1 0x3B00 //F1
#define KB_F2 0x3C00 //F2
#define KB_F3 0x3D00 //F3
#define KB_DEL 0x5300 //delete		
#define KB_PGUP 0x4900  //PageUp
#define KB_PGDW 0x5100 //PageDown
#define KB_INS 0x5200 //INSERT	
#define KB_HOME 0x4700 //homeб в начало окна 
//#include "C:\OS\MD.H"
#include "md.H"
//struct   REGPACK regs;
typedef enum { false, true} bool;
typedef enum {head, right, bottom, left } position;
position posit = head;
const X1 = 0,Y1=0, X2 = 640 , Y2 = 480;
void drawGUI(void);
void drawLeftButton (bool state);
void drawRightButton (bool state);
void drawMiddleButton (bool state);
void drawLeftRightButtons (bool state);
void drawBar (int left, int top, int right, int bottom, int color);
void drawEllipse (int x, int y, int xrad, int yrad, int color);
void drawColButton (bool state, int color);
void drawingSTUFF (int &mX,int &mY, int &mB, int &oldmX,int  &oldmY,int &color, int &tmpColor,int &scanCode, bool &butt1,bool &butt2,bool &butt3, bool &butt4,bool &firstPress);
void processScanCode(int &mX, int &mY, int &mB, int scanCode, int &color, int &font, int &fontSize);

int main (){
   clrscr ( );
   initMouse();
   if (regs.r_ax == 0){
      printf (" Mouse not found!\n Press any key to continue ...");
      getch ();
      return -1;
   }
   else {
      printf ("\n Mouse found and have %d buttons ", regs.r_bx);
      getch ();
   }
      /* request auto detection */
	int gdriver = DETECT, gmode, errorcode;

/* initialize graphics mode */
	initgraph(&gdriver, &gmode, "C:\OS");

/* read result of initialization */
	errorcode = graphresult();

	if (errorcode != grOk)  /* an error occurred */
	{
	   printf("Graphics error: %s\n", grapherrormsg(errorcode));
	   printf("Press any key to halt:");
	   getch();
	   exit(1);             /* return with error code */
	}
	   bool butt1 = false;
	   bool butt2 = false;
	   bool butt3 = false;
	   bool butt4 = false;/*left, right, mid, left+right*/
	   bool firstPress=true;
	   int mX, mY, mB, oldmX, oldmY, color = WHITE, tmpColor, scanCode, font=0, fontSize=0;/*колор ВАЙТ - костыль,чтоб не было  конфликта цветов.TODO спросить ,почему сначала разные цвета*/
   showMouse();
   setMousePos(300, 300);
   if (posit == bottom){
	setMouseBound(BOUND_VERTICAL,0, Y2-30);
   }
   else if (posit == head){
	setMouseBound (BOUND_VERTICAL, Y1+30, Y2);
   }
   else if (posit == left){
	setMouseBound (BOUND_HORIZONTAL, X1+30, X2);
   }
   else if (posit == right){
	setMouseBound (BOUND_HORIZONTAL , X1, X2-30);
   }
   drawGUI();
   while (1){
	if (bioskey(1)!=0){
		scanCode=bioskey(0);
		if (scanCode==KB_ESC){
			break;
		}
		getMouseState (mX, mY, mB);
		processScanCode(mX, mY,mB, scanCode, color, font, fontSize); // надо еще написать по-человечески
	}
	oldmX=mX;
	oldmY=mY;
	getMouseState(mX,mY,mB);
	drawingSTUFF(mX,mY,mB,oldmX,oldmY,color,tmpColor,scanCode, butt1,butt2,butt3,butt4, firstPress);
   }
   closegraph();
  return 0;
}


void processScanCode(int &mX, int &mY, int &mB, int scanCode, int &color, int &font, int &fontSize){
	int charWidth = textwidth("A");
	int charHeight = textheight ("A"); 
	if (mX>getmaxx()-2*charWidth){
		hideMouse();
		setMousePos(0,mY+charHeight);
		showMouse();
	}
	if((char)scanCode==0){
		if(scanCode==KB_RA){
			setMousePos(mX+charWidth,mY);
		}
		else if(scanCode==KB_LA){
			setMousePos(mX-charWidth,mY);
		}
		else if(scanCode==KB_UA){
			setMousePos(mX,mY-charHeight);
		}		
		else if(scanCode==KB_DA){
			setMousePos(mX, mY+charHeight);
		}
		else if (scanCode==KB_END){
			setMousePos(getmaxx()-charWidth,mY);
		}
		else if (scanCode==KB_DEL){ 
			hideMouse();
			drawBar(mX,mY,mX+charWidth,mY+charHeight,BLACK );
			showMouse();
			}
		else if (scanCode==KB_PGUP){
			hideMouse();
			setMousePos(mX,mY-charHeight);
			showMouse();
		}
		else if (scanCode == KB_PGDW){
			hideMouse();
			setMousePos(mX,mY+charWidth);
			showMouse();
		}
		else if(scanCode == KB_HOME){
			hideMouse();
			setMousePos(0,mY);
			showMouse();
		} 
		else if (scanCode == KB_INS){
			setMousePos(getmaxx(),getmaxy());
		}
		else if (scanCode == KB_F2){
			if (fontSize < 10){
				settextstyle (font, HORIZ_DIR, ++fontSize);
			}
			else {
				settextstyle (font,HORIZ_DIR, fontSize);
			}
		}
		else if (scanCode == KB_F1){
			if (fontSize>0){
				settextstyle(font,HORIZ_DIR, --fontSize);
			}
			else if (fontSize==0){
				settextstyle (font,HORIZ_DIR, fontSize);
			}
		}
		else if (scanCode == KB_F3){
			if (color<getmaxcolor()){
				setcolor(++color);
			}
			else if (color>=getmaxcolor()){
				color = 0;
				setcolor(color);
			}
		}
		else {
			printf("scanCode=%04X(functinoal buttons)",scanCode);
		}
	}
	else if ((char)scanCode!=0){
		if (scanCode == KB_BSP){
			hideMouse();
			drawBar(mX-charWidth, mY+charHeight, mX,mY,BLACK);
			setMousePos(mX-charWidth,mY);
			showMouse();
		}
	else if (scanCode == NUM_PLUS){
		if(font<5){
			settextstyle(++font,HORIZ_DIR,fontSize);
		}
		else if (font==5){
			settextstyle(font,HORIZ_DIR,fontSize);
		}
		
	}
	else if (scanCode == NUM_MINUS){
		if (font>0){
			settextstyle (--font,HORIZ_DIR, fontSize);
		}
		else if (font==0){
			settextstyle (font, HORIZ_DIR, fontSize);
		}
	}
	else if (scanCode == KB_ENTER){
		hideMouse();
		setMousePos(0, mY+charHeight);
		showMouse();
	}
	else if (scanCode == KB_TAB){ // добавить проверку на перенос
		hideMouse();
		setMousePos(mX+4*charWidth, mY);
		showMouse();
	}
	else {
		char* strSymb ="*";
		strSymb[0]=(char)scanCode;
		hideMouse();
		getMouseState(mX,mY,mB);
		outtextxy(mX,mY,strSymb);
		setMousePos(mX+charWidth,mY);//добавить переносы
		showMouse();
	}
	/*else {
		printf ("scanCode %04X ASCII",scanCode);
	}*/
}
	gotoxy(0,2);
	printf("size:%2d , font:%2d", fontSize,font);
}
void drawingSTUFF(int &mX,int &mY, int &mB, int &oldmX,int  &oldmY,int &color, int &tmpColor,int &scanCode, bool &butt1,bool &butt2,bool &butt3, bool &butt4,bool &firstPress){

  
   if (firstPress==true){
  	setMousePos(300,300);
  	oldmX=300;
	oldmY=300;
	mX=300;
	mY=300;
	firstPress=false;
   }
      getMouseState(mX, mY, mB);
      if (posit == head || posit == bottom){
	gotoxy (1, 1);
	gotoxy (3,1);
	printf ("%3d", mX);
	gotoxy (8,1);
	printf ("%3d",mY);

      }
      if (regs.r_bx==0){
	setMouseCursor(CURSOR_POINTER);
	showMouse();
      }
      if (mB == 1){
	 if (!butt1){
	    butt1= true;
	    setMouseCursor(CURSOR_PENCIL);
	    drawLeftButton(true);
	 }
      setcolor (color);
      hideMouse();
      line (oldmX, oldmY, mX, mY);
      showMouse();
     //putpixel (mX, mY, getcolor());
      }   //end mb=1
      else {
	if (butt1){
	   drawLeftButton(false);
	   butt1=false;
	}
      }
      if (mB == 2){
	 if(!butt2){
	    butt2=true;
	    drawRightButton(true);
	    setMouseCursor(CURSOR_ERASER);
	 }
	 if (posit == head){
		if (mY>=(Y1+35)){
			drawRightButton(true);
			hideMouse();
			drawEllipse(mX, mY,5,5,BLACK);
			showMouse();
		}
	 }
      }
      else {
	 if (butt2){
	    drawRightButton (false);
	    butt2=false;
	    //showMouse();
	 }
      }
     /* if(mB == 4){
      hideMouse();
	  if (!butt4){
	     butt4=true;
	     if (color<getmaxcolor()){
		drawColButton(true, ++color);
	     }
	     else if (color>=getmaxcolor()){
		color = 0;
		drawColButton(true, color);
	     }
	  }
	  showMouse();

      }
      else {
	 if (butt4){
	    drawColButton(false, color);
	    butt4=false;
	 }
      }*/
      if (mB==3){
	// drawBar(X1,Y1,X2,450,BLACK);
	 if (!butt3){
	    butt3=true;
	   // drawLeftRightButtons (true);
	   drawRightButton(true);
	   drawLeftButton(true);
	 }
	 hideMouse();
	 if (posit == head){
		drawBar(X1, Y1+30,X2,Y2,BLACK);
	 }
	 showMouse();
      }
      else {
	 if( butt3){
	   // drawLeftRightButtons (false);
	   drawLeftButton (false);
	   drawRightButton(false);
	    butt3=false;
	  }
      }


  // }//end while

	
}
void drawGUI(void){
	int CurColor=getcolor();
	if (posit == head){
	drawBar(X1, Y1, X2, Y1+30, DARKGRAY); /*draws GUI bar */
	drawBar (X2-30, Y1, X2,Y1+30, WHITE); /*draws mouse*/
	drawEllipse ( X2-17, Y1+26, 2, 2, RED);                            /*shows that mouse is connected */
	gotoxy(1,1);
	printf ("X:");
	gotoxy(7,1);
	printf ("Y:");
   }
   setcolor(CurColor);
}
void drawBar( int left, int top, int right, int bottom, int color){ 
	int curColor = getcolor();
   setfillstyle(SOLID_FILL, color);
   setcolor (color);
   bar (left, top, right, bottom);
   setcolor(curColor);
}
void drawEllipse (int x, int y, int xrad, int yrad, int color){
   int curColor = getcolor();
   setfillstyle (SOLID_FILL, color);
   setcolor(color);
   fillellipse (x, y, xrad,yrad);
   setcolor(curColor);
}
void drawLeftButton (bool state){
	 if (posit == head){
		if (state==true){
			drawBar (X2-30, Y1, X2-15, Y1+15, LIGHTGREEN);
		}
		else {
			drawBar(X2-30, Y1, X2-15, Y1+15, WHITE);
		}
	}
}
void drawRightButton (bool state){
	 if (posit == head){
		if (state==true){
			drawBar(X2-15, Y1, X2, Y1+15, LIGHTGREEN);
		}
		else {
			drawBar (X2-15, Y1, X2, Y1+15, WHITE );
		}
	}
}
void drawLeftRightButtons(bool state){   /*TODO : make fix with butt 1 and butt 2 == true*/
	 if (posit == head){
		if (state==true){
			drawBar (X2-30, Y1, X2, Y2-15, LIGHTGREEN);
		}
		else if (state == false){
			drawBar(X2-30, Y1, X2, Y2-15, WHITE );
		}
	}
}
void drawColButton (bool state, int color){
	if (posit == head){
		if (state== true){
			drawEllipse (X2-15, Y1+20, 4, 3 , color);
		}
		else {
			drawEllipse (X2-15, Y1+20, 4,3 , WHITE);
		}
	}
}
