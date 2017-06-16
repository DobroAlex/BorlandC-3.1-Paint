#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <graphics.h>
struct   REGPACK regs;
typedef enum { false, true} bool;
const X1 = 0,Y1=0, X2 = 640 , Y2 = 480;
void drawHead(void);
void drawLeftButton (bool state);
void drawRightButton (bool state);
void drawMiddleButton (bool state);
void drawLeftRightButtons (bool state);
void drawBar (int left, int top, int right, int bottom, int collor);
void drawEllipse (int x, int y, int xrad, int yrad, int color);
int main (){
   clrscr ( );
   regs.r_ax = 0;
   intr (0x33, &regs);
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
initgraph(&gdriver, &gmode, "");

/* read result of initialization */
errorcode = graphresult();

if (errorcode != grOk)  /* an error occurred */
{
   printf("Graphics error: %s\n", grapherrormsg(errorcode));
   printf("Press any key to halt:");
   getch();
   exit(1);             /* return with error code */
}
   drawHead();
   int mX, mY, mB, oldmX, oldmY, color= GREEN;
   bool butt1 = false;
   bool butt2 = false;
   bool butt3 = false;
   bool butt4 = false;   /*left, right, mid, left+right*/
   regs.r_ax = 1;
   intr (0x33, &regs);
   regs.r_ax= 8;
   regs.r_cx = 0;
   regs.r_dx = Y2-31;
   intr(0x33, &regs);
   while (1){
      if (kbhit()){
	 break;
      }
      oldmX=mX;
      oldmY = mY;
      regs.r_ax = 3;
      intr(0x33,&regs);
      mX=regs.r_cx;
      mY=regs.r_dx;
      mB=regs.r_bx;
      gotoxy (1, 25);
      printf ("X=%d,Y=%d",mX, mY);
      if (mB == 1){
	 if (!butt1){
	    butt1= true;
	    drawLeftButton(true);
	 }
      setcolor (color);
       regs.r_ax=2;
      intr(0x33,&regs);
      line (oldmX, oldmY, mX, mY);
      regs.r_ax=1;
      intr(0x33,&regs);
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
	 }
	 if (mY < (Y2-35)){
	    drawRightButton(true);
	    regs.r_ax=2;
	    intr(0x33,&regs);
	    drawEllipse(mX,mY,5, 5,BLACK);
	    regs.r_ax=1;
	    intr(0x33, &regs);
	 }
      }
      else {
	 if (butt2){
	    drawRightButton (false);
	 }
      }
      if(mB == 4){
	  if (!butt4){
	     butt4=true;
	     drawEllipse (625, 465, 4, 4, color);
	  }
	  color++;
	  if(color>getmaxcolor()){
		color=0;
	  }
	  regs.r_ax=1;
	  intr(0x33,&regs);

      }
      else {
	 if (butt4){
	    drawBar (620, 450, 630, 470, WHITE);
	    butt4=false;
	 }
      }
      if (mB==3){
	 if (!butt3){
	    butt3=true;
	    drawLeftRightButtons (true);
	 }
	 regs.r_ax=2;
	 intr(0x33,&regs);
	 drawBar (X1,Y1, X2, 450, BLACK);
	 regs.r_ax=1;
	 intr(0x33,&regs);
      }
      else {
	 if( butt3){
	    drawLeftRightButtons (false);
	    butt3=false;
	  }
      }
   }//end while
   getch();
   return 0;

}
void drawHead(void){
   int curColor=getcolor();
   drawBar(X1,Y2-30,X2, Y2,DARKGRAY );
   drawBar (X2-30, Y2-30, X2, Y2, WHITE);   /*draw mouse icon*/
   drawBar (X2-20, Y2-5, X2-15, Y2, RED);    /*dhows that mouse is connected*/

   setcolor (curColor);
}
void drawBar( int left, int top, int right, int bottom, int color){
   setfillstyle(SOLID_FILL, color);
   setcolor (color);
   bar (left, top, right, bottom);
}
void drawEllipse (int x, int y, int xrad, int yrad, int color){
   int curColor = getcolor();
   setfillstyle (SOLID_FILL, color);
   setcolor(color);
   fillellipse (x, y, xrad,yrad);
}
void drawLeftButton (bool state){
	if (state==true){
		drawBar (X2-30, Y2-30, X2-15, Y2-15, LIGHTGREEN);
	}
	else if (state==false) {
		drawBar(X2-30,Y2-30,X2-15, Y2-15, WHITE);
	}
}
void drawRightButton (bool state){
	if (state==true){
		drawBar(X2-15,Y2-30, X2, Y2-15, LIGHTGREEN);
	}
	else if (state==false) {
		drawBar(X2-15, Y2-30 , X2, Y2-15, WHITE);
	}
}
void drawLeftRightButtons(bool state){
	if (state==true){
		drawBar(X2-30, Y2-30, X2 , Y2-15, LIGHTGREEN);
	}
	else if (state==false){
		drawBar(X2-30,Y2-30,X2,Y2-15, WHITE);
	}
}