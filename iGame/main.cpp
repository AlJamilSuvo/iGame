# include "iGraphics.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#define WALL 1001
#define JWALL 1002
#define MWALL 1003



int height[50000];
int blockData[50000];
int doubleS[500];
int doubleE[500];
int doubleUp[500];
int doubleDown[500];
int doubleUpH[500];
int doubleDownH[500];


void playerHeight();
int getHeight(int tx, int ty);

int EnemyNo=3;
int mWallNo = 1;
int wallNo = 4;
int jWallNo = 1;
int playerBlockNo;
int buttonNo = 2;

typedef struct Button{
	int x;
	int y;
	int OnOff;
};
typedef struct Enemy{
	int x;
	int y;
	int life;
	int blockNo;
	int p;
	int k;
	int a;
	int m;
};
typedef struct Block{
	int start;
	int end;
};
typedef struct Wall{
	int sPos;
	int ePos;
	int h;
};
typedef struct JumpingWall{
	int Pos;
	int upperLimit;
	int lowerLimit;
	int d;
	int h;
	int buttonNo;
	int blockNo;
};
typedef struct MovingWall{
	int left;
	int right;
	int h;
	int d;
	int current;
	int buttonNo;
	int blockNo;
};
int gravitalForce;
Button button[500];
Enemy enemy[1000];
int cEnemy=-1;
int atMode;
void update();
Block block[500];
Wall wall[100];
JumpingWall jWall[100];
MovingWall mWall[100];

int playerMaxHealth = 100;
int playerHealth = 100;
int playerX=300, playerY=300;
int cX=0, cY=0;
int p=0;
int k = 0;
char *n[] = { "prince/l1.bmp", "prince/l2.bmp", "prince/l3.bmp", "prince/l4.bmp", "prince/l5.bmp", "prince/l6.bmp", "prince/l7.bmp", "prince/l8.bmp",
				"prince/l9.bmp", "prince/l10.bmp", "prince/l11.bmp", "prince/l12.bmp", "prince/l13.bmp",
				"prince/r1.bmp", "prince/r2.bmp", "prince/r3.bmp", "prince/r4.bmp", "prince/r5.bmp", "prince/r6.bmp", "prince/r7.bmp", "prince/r8.bmp",
				"prince/r9.bmp", "prince/r10.bmp", "prince/r11.bmp", "prince/r12.bmp", "prince/r13.bmp" };

void iDraw()
{
	iClear();
	/*Wall*//*Start*/
	for (int j = 0; j < wallNo; j++){
		int gh = wall[j].h - 55;
		iShowBMP(wall[j].sPos+cX, gh, "lc.bmp");
		int gx = wall[j].sPos + 64;
		while (gx < wall[j].ePos - 64) {
			iShowBMP(gx+cX, gh, "uw.bmp");
			gx += 64;
		}
		gx = wall[j].sPos;
		for (int i = 1; i < 3; i++){
			gh = wall[j].h - 55 - i * 64;
			gx = wall[j].sPos;
			while (gx < wall[j].ePos - 64){
				iShowBMP(gx+cX, gh, "wall.bmp");
				gx += 64;
			}

		}
		iShowBMP(wall[j].ePos - 64+cX, wall[j].h - 55, "rc.bmp");
		iShowBMP(wall[j].ePos - 64+cX, wall[j].h - 55 - 64, "rw.bmp");
		iShowBMP(wall[j].ePos - 64+cX, wall[j].h - 55 - 64 - 64, "rwb.bmp");

	}
	/*Wall*//*End*/



	/*Jumping Wall*//*Start*/
	for (int j = 0; j < jWallNo; j++){
		iShowBMP(jWall[j].Pos + cX, jWall[j].h - 55, "lc.bmp");
		iShowBMP(jWall[j].Pos + 64 + cX, jWall[j].h - 55, "rcb.bmp");
	}
	/*Jumping Wall*//*End*/



	/*Moving Wall*//*Start*/
	for (int j = 0; j < mWallNo; j++){
		iShowBMP(mWall[j].current + cX, mWall[j].h - 55, "lc.bmp");
		iShowBMP(mWall[j].current + 64 + cX, mWall[j].h - 55, "rcb.bmp");
	}
	/*Moving Wall*//*End*/

	/*Enemy*//*Start*/

	for (int j = 0; j < EnemyNo; j++){
		if (enemy[j].a == 1){
			if (enemy[j].k == 0)iShowBMP(enemy[j].x - 45 + cX, enemy[j].y, "al.bmp");
			else iShowBMP(enemy[j].x - 20 + cX, enemy[j].y, "ar.bmp");
			enemy[j].a = 0;
			enemy[j].m = 0;
			if (enemy[j].k == 0 && enemy[j].x - playerX <= 64) playerHealth -= 2;
			if (enemy[j].k != 0 && playerX - enemy[j].x <= 64) playerHealth -= 2;
		}
		else{
			iShowBMP(enemy[j].x - 32 + cX, enemy[j].y, n[enemy[j].p % 13 + enemy[j].k]);
			if (enemy[j].m != 5) enemy[j].m++;
		}
		iSetcolor(255, 0, 0);
		iLine(enemy[j].x - 25 + cX, enemy[j].y + 46, enemy[j].x - 25 + enemy[j].life + cX, enemy[j].y + 46);
		iSetcolor(255, 255, 255);
	}
	/*Enemy*//*End*/


	/*Player*//*Start*/
	playerHeight();
	if (playerBlockNo == -1){
		if (k == 0) iShowBMP(playerX - 32 + cX, playerY, "freeL.bmp");
		else iShowBMP(playerX - 32 + cX, playerY, "freeR.bmp");
	}
	else if (atMode) {
		if (k == 0) iShowBMP(playerX - 45 + cX, playerY, "al.bmp");
		else iShowBMP(playerX - 20 + cX, playerY, "ar.bmp");
		if (cEnemy != -1 && k == 0 && playerX - enemy[cEnemy].x <= 64) enemy[cEnemy].life -= 5;
		if (cEnemy != -1 && k != 0 && enemy[cEnemy].x - playerX <= 64) enemy[cEnemy].life -= 5;
	}
	else iShowBMP(playerX - 32 + cX, playerY, n[p % 13 + k]);
	if (atMode) atMode--;
	/*Player*//*End*/
	


	/*Switch*//*Start*/
	for (int j = 0; j < buttonNo; j++){
		if (button[j].OnOff) iShowBMP(button[j].x-32+cX, button[j].y, "onSwitch.bmp");
		else iShowBMP(button[j].x-32+cX, button[j].y, "offSwitch.bmp");
	}


	/*Switch*//*End*/





	/*Info*//*Start*/
	iSetcolor(255, 255, 255);
	iShowBMP(20, 660, "life.bmp");
	iRectangle(38, 660, playerMaxHealth + 4, 16);
	iFilledRectangle(40, 662, playerHealth, 12);
	
	
	
	if (playerBlockNo != -1){
		if (playerX - block[playerBlockNo].start < 52) iText(20, 20, "Press \'A\' to Jump left From Block and \'D\' to move right");
		else if (block[playerBlockNo].end - playerX < 50) iText(20, 20, "Press \'A\' to move left and \'D\' to Jump right from Block");
		else iText(20, 20, "Press \'A\' to move left and \'D\' to move right");
	}
	/*Info*//*End*/
	
	update(); //update for next frame//


	
}


void iMouseMove(int mx, int my)
{

}


void iMouse(int button, int state, int mx, int my)
{
	
}


void iKeyboard(unsigned char key)
{
	if (key == 'a' && playerBlockNo!=-1){
		if (playerX - block[playerBlockNo].start < 52) {
			int nh = getHeight(playerX-100, playerY);
			if (!(nh>playerY && nh - playerY <= 64 * 4 - 55)) {
				playerX -= 102;
				cEnemy = -1;
			}
		}
		else {
			if (!(cEnemy != -1 && playerX - enemy[cEnemy].x < 64)) playerX -= 2;
		}
		if (k == 2) p = 0;
		else p++;
		k = 0;

	}
	if (key == 'd' && playerBlockNo != -1){
		if (block[playerBlockNo].end - playerX < 50){
			int nh = getHeight(playerX + 100, playerY);
			if (!(nh>playerY && nh - playerY <= 64 * 4 - 55)) {
				playerX += 100;
				cEnemy = -1;
			}
		}
		
		else {
			if (!(cEnemy != -1 && enemy[cEnemy].x - playerX< 64)) playerX += 2;
		}
		if (k == 0) p = 0;
		else p++;
		k = 13;
	}
	if (key == 'p'){
		playerX = 300;
		playerY = 300;
	}
	if (key == ' '){
		for (int j = 0; j < buttonNo; j++){
			if (abs(playerX - button[j].x) <= 32 && button[j].y>playerY && button[j].y - playerY <= 48){
				if (button[j].OnOff) button[j].OnOff = 0;
				else button[j].OnOff = 1;
			}
		}
	}
	if (key == 'e'){
		atMode = 5;
	}
	
}

int main()
{
	wall[0].sPos = 220;
	wall[0].h = 220;
	wall[0].ePos = 540;
	block[0].start = 220;
	block[0].end = 540;
	for (int i = 220 / 2; i <= 540 / 2; i++)
	{
		height[i] = 200;
	}
	wall[1].sPos = 220;
	wall[1].h = 520;
	wall[1].ePos = 540;
	block[1].start = 220;
	block[1].end = 540;
	for (int i = 220 / 2; i <= 540 / 2; i++)
	{
		height[i] = 292;
	}

	enemy[0].x = 500;
	enemy[0].y = 520;
	enemy[0].life = 20;
	enemy[0].blockNo = 1;
	enemy[0].a = 0;
	enemy[0].p = 0;
	enemy[0].k = 13;
	enemy[0].life = 50;
	button[0].y = 220 + 48;
	button[0].x = 450;
	button[0].OnOff = 0;
	doubleS[0] = 220;
	doubleE[0] = 540;
	doubleUp[0] = 1;
	doubleUpH[0] = 520;
	doubleDownH[0] = 220;
	doubleDown[0] = 0;
	jWall[0].Pos = 92;
	jWall[0].h = 292;
	jWall[0].d = 1;
	jWall[0].upperLimit = 596;
	jWall[0].lowerLimit = 150;
	jWall[0].buttonNo = 0;
	jWall[0].blockNo = 2;
	block[2].start = 92;
	block[2].end = 92+64*2;
	for (int i = jWall[0].Pos / 2; i < (jWall[0].Pos + 128) / 2; i++){
		height[i] = jWall[0].h;
		blockData[i] = 2;
	}
	button[1].y = 400 + 48;
	button[1].x = 700;
	button[1].OnOff = 0;
	enemy[1].x = 750;
	enemy[1].y = 400;
	enemy[1].blockNo = 3;
	enemy[1].a = 0;
	enemy[1].p = 0;
	enemy[1].k = 0;
	enemy[1].life = 50;
	wall[2].sPos = 540;
	wall[2].h = 400;
	wall[2].ePos = 540+320;
	block[3].start = 540;
	block[3].end = 860;
	for (int i = 520 / 2; i <= 860 / 2; i++)
	{
		height[i] = 400;
		blockData[i] = 3;
	}
	mWall[0].buttonNo = 1;
	mWall[0].left = 860;
	mWall[0].right = 1122;
	mWall[0].current = mWall[0].left;
	mWall[0].h = 380;
	mWall[0].d = 1;
	mWall[0].blockNo = 4;
	for (int i = mWall[0].left/2; i < mWall[0].right/2+64; i++){
		height[i] = 0;
		blockData[i] = -1;
	}
	for (int i = mWall[0].current / 2; i < mWall[0].current / 2 + 64; i++){
		height[i] = mWall[0].h;
		blockData[i] = 4;
	}
	block[4].start = mWall[0].current;
	block[4].end = mWall[0].current + 128;
	wall[3].sPos = 1250;
	wall[3].h = 380;
	wall[3].ePos = 1250 + 320;
	block[5].start = 1250;
	block[5].end = 1250 + 320;;
	for (int i = 1250 / 2; i <= (1250+320) / 2; i++)
	{
		height[i] = 380;
		blockData[i] = 5;
	}
	enemy[2].x = 1480;
	enemy[2].y = 380;
	enemy[2].blockNo = 5;
	enemy[2].a = 0;
	enemy[2].p = 0;
	enemy[2].k = 0;
	enemy[2].life = 50;


	iInitialize(1350, 680, "Snake Game");
	return 0;
}
void update()
{
	if (playerBlockNo == -1){
		if (playerY <= 0){
			playerHealth = 0;
		}
		else gravitalForce++;
	}
	else{
		playerHealth -= gravitalForce / 5;
		gravitalForce = 0;
	}
	/*Camera*//*Update*//*Start*/
	if (playerX + cX>800)cX = 800 - playerX;
	else if (playerX + cX < 200) cX = 200 - playerX;
	/*Camera*//*Update*//*End*/


	/*Jumping Wall*//*Update*//*Start*/
	for (int j = 0; j < jWallNo; j++){
		if (button[jWall[j].buttonNo].OnOff){
			jWall[j].h += jWall[j].d;
			if (jWall[j].h >= jWall[j].upperLimit || jWall[j].h <= jWall[j].lowerLimit) jWall[j].d = -jWall[j].d;
		}
		for (int i = jWall[j].Pos / 2; i < (jWall[j].Pos + 128) / 2; i++){
			height[i] = jWall[j].h;
			if (playerBlockNo == jWall[j].blockNo) playerY = jWall[j].h;
		}
	}
	/*Jumping Wall*//*Update*//*End*/


	/*Moving Wall*//*Update*//*Start*/
	for (int j = 0; j < mWallNo; j++){
		if (button[mWall[j].buttonNo].OnOff){
			mWall[j].current += mWall[j].d;
			playerHeight();
			if (playerBlockNo == mWall[j].blockNo) playerX += mWall[j].d;
			if (mWall[j].current <= mWall[j].left || mWall[j].current >= mWall[0].right) mWall[j].d = -mWall[j].d;
		}
		for (int i = mWall[j].left / 2; i < mWall[j].right / 2+64; i++){
			height[i] = 0;
			blockData[i] = -1;
		}
		for (int i = mWall[j].current / 2; i < mWall[j].current / 2 + 64; i++){
			height[i] = mWall[j].h;
			blockData[i] = 4;
		}
		block[4].start = mWall[j].current;
		block[4].end = mWall[j].current + 128;
	}
	/*Moving Wall*//*Update*//*End*/

	/*Enemy*//*Update*//*Start*/

	for (int j = 0; j < EnemyNo; j++){
		if (enemy[j].life <= 0){
			int t = EnemyNo - 1;
			enemy[j].a = enemy[t].a;
			enemy[j].blockNo = enemy[t].blockNo;
			enemy[j].x = enemy[t].x;
			enemy[j].y = enemy[t].y;
			enemy[j].k = enemy[t].k;
			enemy[j].life = enemy[t].life;
			enemy[j].m = enemy[t].m;
			enemy[j].p = enemy[t].p;
			if (cEnemy == j) cEnemy = -1;
			EnemyNo--;
			continue;
		}
		if (playerBlockNo == enemy[j].blockNo){
			cEnemy = j;
			if (playerX < enemy[j].x){
				enemy[j].k = 0;
				if (enemy[j].x - playerX <= 64){
					if (enemy[j].m == 5) enemy[j].a = 1;
				}
				else {
					enemy[j].x -= 2;
					enemy[j].p++;

				}
			}
			else {
				enemy[j].k = 13;
				if (playerX - enemy[j].x <= 64){
					if (enemy[j].m == 5) enemy[j].a = 1;
				}
				else {
					enemy[j].x += 2;
					enemy[j].p++;
				}
			}
		}
	}
	/*Enemy*//*Update*//*End*/
	

}
void playerHeight(){
	int h;
	playerBlockNo = -1;
	if (playerX >=doubleS[0] && playerX <= doubleE[0]){
		if (playerY < doubleUpH[0]) {
			h = doubleDownH[0];
			if ((playerY-h)>=0 && (playerY-h)<5) playerBlockNo = doubleDown[0];
		}
		else {
			h = doubleUpH[0];
			if ((playerY - h) >= 0 && (playerY - h)<5) playerBlockNo = doubleUp[0];
		}
	}
	else{
		h = height[playerX / 2];
		if ((playerY - h) >= 0 && (playerY - h)<5) playerBlockNo = blockData[playerX / 2];
	}

	if (playerBlockNo == -1) playerY-=5;
	else playerY = h;
}
int getHeight(int tx,int ty){
	int h;
	if (tx >= doubleS[0] && tx <= doubleE[0]){
		if ( ty > doubleDownH[0]) {
			h = doubleUpH[0];
		}
		else {
			h = doubleDownH[0];
		}
	}
	else{
		h = height[tx / 2];
	}
	return h;
}