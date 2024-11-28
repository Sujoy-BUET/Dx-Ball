# include "iGraphics.h"
#include<iostream>
#include<windows.h>
#include<mmsystem.h>
#include<stdio.h>
using namespace std;
#define screenWidth 1366
#define screenHight 900
#pragma comment(lib,"winmm.lib")
/*  function iDraw() is called again and again by the system.   */

#define yAxisMargin 50
#define pi 3.1416

bool music_on=true,is_started=false,show_menu=true;
int r=255,g=255,b=255;
char bc[][100]={"image\\final_bc.bmp","image\\fire.bmp","image\\fire.bmp"};
int xBall=screenWidth/2,yBall=yAxisMargin,radius=10,v=25;
float dxBall=15 ,dyBall=15;
bool fireball=false,drop_fireball=false;
int fireX=300,fireY=600;
int xBoard=xBall-75,yBoard=yBall-radius-25,dxBoard=25;
int dxBrick=50,dyBrick=40;
int ms=0,s=0,m=0,h=0;


#define total 3*6*10
struct Brick{
    int x=0,y=0;
    bool show=true;
    int red,green,blue;
    int life;
}bricks[10][18];

void setAll(){
    int xx=0,yy=400;
    for(int k=0;k<10;k++){
        xx=100;
        for(int i=1;i<=18;i++){
            bricks[k][i-1].x=xx;
            bricks[k][i-1].y=yy;
            if(k==0 || k==9 || i==1 || i==6 || i==7 || i==12 || i==13 || i==18){
                bricks[k][i-1].red=255;
                bricks[k][i-1].green=200;
                bricks[k][i-1].blue=0;
                bricks[k][i-1].life=3;
            }else{
                bricks[k][i-1].red=rand()%256;
                bricks[k][i-1].green=rand()%256;
                bricks[k][i-1].blue=rand()%100;
                bricks[k][i-1].life=1;
            }
            if(i%6==0) xx+=100;
            xx+=50;
        }
        yy+=40;
    }
}


void iDraw() {
	//place your drawing codes here
	iClear();
	// iClear er por korte hoy
	// ei part bar bar run hote thake
	// 1) iRectangle(x,y,Len,wid),iFilledRectangle(,,,)
    // 2) iCircle(x,y,r), iFilledCircle()
    // 3) iLine(x1,y1,x2,y2)
    // 4) iEllipse(x,y,a,b) iFilledEllipse()
    // 5) iSetColor(R,G,B)`
    // 6) iText(x,y,"",font) font=GLUT_BITMAP.....
    // 7) iShowBMP(x,y,bc[idx]) & iShowBMP2( , , ,ignore color)


    if(show_menu){
        iShowBMP2(0,20,(char*)bc[0],0);
        return;
    }

    for(int i=0;i<screenWidth/4;i++){
        iSetColor(0,0,200-i);
        iRectangle(2*i,i,screenWidth-2*i,screenHight-i);
    }
    iSetColor(r,g,b);

    if(fireball){
        iSetColor(255,0,0);
    }
    iFilledCircle(xBall,yBall,radius,1000);
    iSetColor(0,255,100);
    iFilledRectangle(xBoard,yBoard,150,20);
    // Time
    char timer[50];
    sprintf(timer, "Time: %02d:%02d:%02d", h, m, s);
    iText(screenWidth-150,screenHight-50,timer,GLUT_BITMAP_HELVETICA_18);

    int remaining_bricks=0;
    for(int i=0;i<10;i++){
        for(int j=0;j<18;j++){
            //if(bricks[i][j].show && xBall>=bricks[i][j].x && xBall<=bricks[i][j].x+dxBrick && yBall>=bricks[i][j].y && yBall<=bricks[i][j].y+dyBrick){
            if (bricks[i][j].show && xBall + radius > bricks[i][j].x && xBall - radius < bricks[i][j].x + 50 && yBall + radius > bricks[i][j].y && yBall - radius < bricks[i][j].y + 40) {
                bricks[i][j].life--;
                bricks[i][j].red-=25,bricks[i][j].green-=25,bricks[i][j].blue-=0;
                if(bricks[i][j].life==0) bricks[i][j].show=0;
                //PlaySound("music\\HitBrick.wav",NULL,SND_ASYNC);
                //dyBall*=-1;
                int overlap_left = abs((xBall + radius) - bricks[i][j].x);
                int overlap_right = abs((xBall - radius) - (bricks[i][j].x + 40));
                int overlap_top = abs((yBall - radius) - (bricks[i][j].y + 40));
                int overlap_bottom = abs((yBall + radius) - bricks[i][j].y);

                // Determine the smallest overlap
                int min_overlap = min(min(overlap_left, overlap_right), min(overlap_top, overlap_bottom));

                if (min_overlap == overlap_left || min_overlap == overlap_right) {
                // Horizontal collision (left or right)
                dxBall = -dxBall;
                } else if (min_overlap == overlap_top || min_overlap == overlap_bottom) {
                // Vertical collision (top or bottom)
                dyBall = -dyBall;
                }

            }
            if(bricks[i][j].show){
                iSetColor(bricks[i][j].red,bricks[i][j].green,bricks[i][j].blue);
                iFilledRectangle(bricks[i][j].x,bricks[i][j].y,dxBrick,dyBrick);
                //for(int m=bricks[i][j]+dxBrick/4;m>=bricks[i][j];m--){}
                int R=bricks[i][j].red,G=bricks[i][j].green,B=bricks[i][j].blue;
                for(int m=1;m<dxBrick/4;m++){
                    iSetColor(R,G,B);
                    iRectangle(bricks[i][j].x+m,bricks[i][j].y+m/2,dxBrick-m,dyBrick-m);
                    R-=10,G-=10;
                }
                remaining_bricks++;
            }
        }
    }
    if(remaining_bricks==0) exit(0) ;
    if(drop_fireball){
        iSetColor(0,0,0);
        iShowBMP2(fireX,fireY,(char*)bc[1],0);
        if(fireY<0) drop_fireball=false;
    }
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/

void iKeyboard(unsigned char key) {
	if (key == 'q'){
        is_started=false;
        iText(150,50,"Do you want to quit?",GLUT_BITMAP_HELVETICA_18);
	}else if(key=='y'){
        exit(0);
	}else if(key=='n'){
        is_started = true;
    }else if(key=='s'){

    }else if(key=='r'){

    }else if(key=='g'){

    }else if(key=='b'){

    }else if(key==' '){
        if(is_started){
            is_started=false;
        }else{
            is_started=true,show_menu=0;
        }
        if(is_started){
            PlaySound("music\\bgmusic.wav",NULL,SND_LOOP|SND_ASYNC);
        }
        else if(show_menu) PlaySound("music\\theme_music.wav",NULL,SND_LOOP|SND_ASYNC);
        else PlaySound(0,0,0);
    }
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {

	if (key == GLUT_KEY_END) {
		show_menu=true;
		PlaySound("music\\theme_music.wav",NULL,SND_LOOP|SND_ASYNC);
        music_on=true;
	}else if(key==GLUT_KEY_DOWN){

	}else if(key==GLUT_KEY_LEFT){
	    if(xBoard>=0){
            xBoard-=dxBoard;
            if(!is_started)
                xBall-=dxBoard;
	    }
	}else if(key==GLUT_KEY_RIGHT){
	    if(xBoard+100<=screenWidth){
            xBoard+=dxBoard;
            if(!is_started)
                xBall+=dxBoard;
	    }
	}else if(key==GLUT_KEY_UP){

	}else if(key==GLUT_KEY_HOME){

	}else if(key==GLUT_KEY_INSERT){

	}else if(key==GLUT_KEY_F4){
	    if(music_on){
            PlaySound(0,0,0);
            music_on=false;
	    }else{
            PlaySound("music\\theme_music.wav",NULL,SND_LOOP|SND_ASYNC);
            music_on=true;
	    }
	}
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	printf("x = %d, y= %d\n",mx,my);
	//place your codes here
	//x=mx,y=my;
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if(my<=540 && my>=470 && mx>=325 && mx<=1040){
            show_menu=false;
        }else if(my>=400 && my<=470 && mx>=325 && mx<=1040){

        }else if(my>=330 && my<=400 && mx>=325 && mx<=1040){

        }else if(my>=260 && my<=330 && mx>=325 && mx<=1040){
            exit(0);
        }
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {

	}
}

void restart(){
    is_started=false;
    xBall=screenWidth/2,yBall=yAxisMargin,radius=10;
    xBoard=xBall-75,yBoard=yBall-radius-20,dxBoard=25;
}


void change(){
    if(is_started){
        xBall+=dxBall,yBall+=dyBall;
        if(xBall>=screenWidth || xBall<=0) dxBall*=-1;
        //if(yBall<=yAxisMargin || yBall>=screenHight) dyBall*=-1;
        if(yBall>=screenHight) dyBall*=-1;
    }
    if((xBall>=xBoard && xBall<=xBoard+150)&&(yBall-radius<=yBoard+20)){
         //dyBall*=-1;
        float theta=pi-((xBall-xBoard)*1.0/150.0)*pi;
        dxBall = v*cos(theta);
        dyBall=v*sin(theta);

        if(!show_menu){
            //PlaySound("music\\HitPad.wav",NULL,SND_ASYNC);
        }
    }
    else{
        if(yBall+radius<yAxisMargin){
            PlaySound(0,0,0);
            restart();
        }
    }
    if(is_started) ms+=50;
    if(ms>=1000){
        ms=0;
        s++;
        if(s==60){
            s=0;
            m++;
            if(m==60){
                m=0;
                h++;
            }
        }
    }
    fireY-=40;
    if(s>=10) drop_fireball=true;
}


int main() {
    setAll();
    iSetTimer(10,change);
    PlaySound("music\\theme_music.wav",NULL,SND_LOOP|SND_ASYNC);
	iInitialize(screenWidth,screenHight, "My Game"); // gives frame of the code
	return 0;
}
