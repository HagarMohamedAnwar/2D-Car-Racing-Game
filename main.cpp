#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include<cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
#include <thread>
#include "soundcontroller.h"

using namespace std;


//Game Speed
double FPS = 50;

//Game Track
int start=0;
int gv=0;
int level = 0;
int flag = 0;
int lives = 4;
int livesTemp1 = lives - 1;
int livesTemp2 = lives - 1;
int livesTemp3 = lives - 1;
int f1 = 0;
int f2 = 0;
int f3 = 0;


//Track Score
int score = 0;
int p1lose = 0;
int p2lose = 0;
int high_score;

//Form moving Road
int roadDivTopMost = 0;
int roadDivTop = 0;
int roadDivMd2 = 0;
int roadDivMd3 = 0;
int roadDivBtm = 0;
int roadDivTop_1 = 0;
int roadDivMd2_1 = 0;
int roadDivMd3_1 = 0;
int roadDivBtm_1 = 0;

//For Multiplayer
int mul = 0;

//For Player 1
    //Left and Right
int lrIndexP1 = 0 ;
    //Up and Down
int udIndexP1 = 0;

//For Player 2
    //Left and Right
int lrIndexP2 = 100;
    //Up and Down
int udIndexP2 = 0;

//Player 1 Obstacles
    //Y Axis
int car1 = 0;
int car2 = 30;
int car3 = 60;
    //X Axis
int random1=0;
int random2=0;
int random3=0;
//Player 2 Obstacles
    //Y Axis
int car4 = 0;
int car5 = 30;
int car6 = 60;
    //X Axis
int random4=100;
int random5=100;
int random6=100;
//tree height
int yTree1=100;
int yTree2=150;
int yTree3=200;
int yTree4=250;

int game_ends = 0;

//For Display TEXT
const int font1=(int)GLUT_BITMAP_TIMES_ROMAN_24;
const int font2=(int)GLUT_BITMAP_HELVETICA_18 ;
const int font3=(int)GLUT_BITMAP_8_BY_13;

char s[30];

//Changes Highscore in text file
void changeHighScore(){

    FILE *file;
    //Opens file r for read
    file = fopen("text.txt","r");
    //Reads high score
    fscanf(file,"%d", &high_score);
    //Closes file
    fclose(file);

    //checks weather or not the player score is bigger than old highscore in file, if yes the hghscore is updated
    if (high_score < score){
        FILE *file;
        //Opens file w for write
        file = fopen("text.txt","w");
        //Writes High score
        fprintf(file,"%d",score);
        //Closes file
        fclose(file);
    }
}

//sound
int check1 = 0;
int check2 =0 ;
void soundGameOver(){
    if(check1 == 0)
    {
        PlaySound("gameover.wav", NULL, SND_FILENAME | SND_ASYNC );
        check1 = 1;
    }
}
void soundWin(){
    if(check2 == 0)
    {
        PlaySound("win.wav", NULL, SND_FILENAME | SND_ASYNC );
        check2 = 1;
    }
}
//Function for displaying Text on screen
void renderBitmapString(float x, float y, void *font,const char *string)
{
    //A pointer on the string required that types character by character on the screen
    const char *c;
    glRasterPos2f(x, y);
    for (c=string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}

//The static trees displayed on the main menu
void tree(int x, int y)
{
    //The x and y shift for the tree
    int newx=x;
    int newy=y;

    //Tree Left

    //Bottom of the tree (el 5ashab)
    glColor3f(0.871, 0.722, 0.529);
    glBegin(GL_TRIANGLES);
    glVertex2f(newx+11,newy+55);
    glVertex2f(newx+12,newy+55-10);
    glVertex2f(newx+10,newy+55-10);
    glEnd();

    //Upper of the tree (el leaves)
    glColor3f(0.133, 0.545, 0.133);
    glBegin(GL_TRIANGLES);
    glVertex2f(newx+11,newy+55+3);
    glVertex2f(newx+12+3,newy+55-3);
    glVertex2f(newx+10-3,newy+55-3);
    glEnd();
}


//Spawn player 1
void player1Spawn()
{
    //MAIN car
        //Front Tire
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndexP1+24,udIndexP1 + 5);
    glVertex2f(lrIndexP1+24,udIndexP1 + 7);
    glVertex2f(lrIndexP1+34,udIndexP1 + 7);
    glVertex2f(lrIndexP1+34,udIndexP1 + 5);
    glEnd();
        //Back Tire
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndexP1+24,udIndexP1 + 1);
    glVertex2f(lrIndexP1+24,udIndexP1 + 3);
    glVertex2f(lrIndexP1+34,udIndexP1 + 3);
    glVertex2f(lrIndexP1+34,udIndexP1 + 1);
    glEnd();
        //Car Body
    glColor3f(0.00, 0.00, 0.00);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndexP1+26,udIndexP1 + 1);
    glVertex2f(lrIndexP1+26,udIndexP1 + 8);
    glColor3f(0.00, 0.00, 0.00);

    glVertex2f(lrIndexP1+29,udIndexP1 + 10);
    glVertex2f(lrIndexP1+32,udIndexP1 + 8);
    glVertex2f(lrIndexP1+32,udIndexP1 + 1);
    glVertex2f(lrIndexP1+29,udIndexP1 + -2);
    glEnd();
}

//Check Player1 Collision with Player 1 Enemies and Player 2 Enemies
void player1CollisionCheck()
{
    //Check if Player 1 has hit Player 1's First Obstacle (Car 1)
    if((abs(lrIndexP1-random1)<8) && (car1+150-udIndexP1)<10 && (car1+150-udIndexP1)> -3)
    {
        //checks if its multiplayer mode
        if(mul ==1)
        {
            p1lose = 1;
            start = 0;
            gv = 1;
            game_ends = 1;
        }

        //Flag 1, if f1 = 1, this means that we are colliding with car1
        f1 = 1;

        //if livesTemp1 == Lives, function wont continue, and no lives--
        //if livesTemp1 != Lives, function will continue normally
        if(livesTemp1 == lives)
            return;

        //Decrease lives cuz collided
        lives--;

        //Test
        //cout << lives << endl;

        //check if lives reached 0, if reached 0, game ends
        if(lives == 0)
        {
            start = 0;
            gv = 1;
            game_ends = 1;
        }

        //For Test
        //cout  << "EnemyP1 1:" <<car1 << "P1:" << udIndexP1 << endl;
    }

    //Check if Player 1 has hit Player 1's Second Obstacle (Car 2)
    if((abs(lrIndexP1-random2) < 8) && (car2+150-udIndexP1)<10 && (car2+150-udIndexP1)>-3)
    {
        //Checks if its Multiplayer Mode
        if(mul ==1)
        {
            p1lose = 1;
            start = 0;
            gv = 1;
            game_ends = 1;
        }

        //Flag 2, if f2 = 1, this means that we are colliding with car2
        f2 = 1;

        //if livesTemp2 == Lives, function wont continue, and no lives--
        //if livesTemp2 != Lives, function will continue normally
        if(livesTemp2 == lives)
            return;

        //Decrease lives cuz collided
        lives--;

        //Test
        //cout << lives << endl;

        //check if lives reached 0, if reached 0, game ends
        if(lives == 0)
        {
            start = 0;
            gv = 1;
            game_ends = 1;
        }

        //For Test
        //cout  << "EnemyP1 2:" <<car2 << "P1:" << udIndexP1 << endl;
    }


    //Check if Player 1 has hit Player 1's Third Obstacle (Car 3)
    if((abs(lrIndexP1-random3)<8) && (car3+150-udIndexP1)<10 && (car3+150-udIndexP1)>-3)
    {
        //Check if multiplayer
        if(mul ==1)
        {
            p1lose = 1;
            start = 0;
            gv = 1;
            game_ends = 1;
        }

        //Flag 3, if f3 = 1, this means that we are colliding with car3
        f3 = 1;

        //if livesTemp3 == Lives, function wont continue, and no lives--
        //if livesTemp3 != Lives, function will continue normally
        if(livesTemp3 == lives)
            return;

        //Decrease lives cuz collided
        lives--;

        //Test
        //cout << lives << endl;

        //check if lives reached 0, if reached 0, game ends
        if(lives == 0)
        {
            start = 0;
            gv = 1;
            game_ends = 1;
        }

        //For Test
        //cout  << "EnemyP1 3:" <<car3 << "P1:" << udIndexP1 << endl;
    }

    //checks collision with Player 2's Obstacles
    if(mul == 1)
    {
        if((abs(lrIndexP1-random4)<8) && (car4+150-udIndexP1)<10 && (car4+150-udIndexP1)>-3 && mul == 1)
        {
            start = 0;
            gv=1;
            p1lose = 1;
            game_ends = 1;

            //For Test
            //cout  << "EnemyP2 1:" <<car4 << "P1:" << udIndexP1 << endl;
        }
        if((abs(lrIndexP1-random5) < 8) && (car5+150-udIndexP1)<10 && (car5+150-udIndexP1)>-3 && mul == 1)
        {
            start = 0;
            p1lose = 1;
            gv=1;
            game_ends = 1;

            //For Test
            //cout  << "EnemyP2 2:" <<car5 << "P1:" << udIndexP1 << endl;
        }
        if((abs(lrIndexP1-random6)<8) && (car6+150-udIndexP1)<10 && (car6+150-udIndexP1)>-3 && mul == 1)
        {
            start = 0;
            p1lose = 1;
            gv=1;
            game_ends = 1;

            //For Test
            //cout  << "EnemyP2 6:" <<car6 << "P1:" << udIndexP1 << endl;

        }
    }
}

//Spawn Player 1 Obstacles (Enemies)
void player1ObstaclesSpawn()
{
    //Opposite car 1
        //Front Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random1+24,car1+146);
    glVertex2f(random1+24,car1+144);
    glVertex2f(random1+34,car1+144);
    glVertex2f(random1+34,car1+146);
    glEnd();
        //Back Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random1+24,car1+150);
    glVertex2f(random1+24,car1+148);
    glVertex2f(random1+34,car1+148);
    glVertex2f(random1+34,car1+150);
    glEnd();
        //Body
    glColor3f(1.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random1+26,car1+150);
    glVertex2f(random1+26,car1+143);
    glVertex2f(random1+29,car1+141);
    glVertex2f(random1+32,car1+143);
    glVertex2f(random1+32,car1+150);
    glEnd();

    //Decrease Car1 Y axis with time
    car1--;

    //Check if Obstacle 1 Reached the bottom
    if(car1<-150)
    {
        //Reset Y Coordinate back to the top
        car1=0;
        //Get Random X Coordinate across the road length
        random1 = 1 + (rand() % 100);
    }


    //Opposite car 2
        //Front Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random2+24,car2+146);
    glVertex2f(random2+24,car2+144);
    glVertex2f(random2+34,car2+144);
    glVertex2f(random2+34,car2+146);
    glEnd();
        //Back Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random2+24,car2+150);
    glVertex2f(random2+24,car2+148);
    glVertex2f(random2+34,car2+148);
    glVertex2f(random2+34,car2+150);
    glEnd();
        //Body
    glColor3f(0.294, 0.000, 0.510);
    glBegin(GL_POLYGON);
    glVertex2f(random2+26,car2+150);
    glVertex2f(random2+26,car2+143);
    glVertex2f(random2+29,car2+141);
    glVertex2f(random2+32,car2+143);
    glVertex2f(random2+32,car2+150);
    glEnd();

    //Decrease car2 Y axis with time
    car2--;

    //Check if Obstacle 2 Reached the bottom
    if(car2<-150)
    {
        //Reset Y Coordinate back to the top
        car2=0;
        //Get Random X Coordinate across the road length
        random2 = 1 + (rand() % 100);
    }



    //Opposite car 3
        //Front Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random3+24,car3+146);
    glVertex2f(random3+24,car3+144);
    glVertex2f(random3+34,car3+144);
    glVertex2f(random3+34,car3+146);
    glEnd();
        //Back Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random3+24,car3+150);
    glVertex2f(random3+24,car3+148);
    glVertex2f(random3+34,car3+148);
    glVertex2f(random3+34,car3+150);
    glEnd();
        //Body
    glColor3f(1.000, 0.271, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random3+26,car3+150);
    glVertex2f(random3+26,car3+143);
    glVertex2f(random3+29,car3+141);
    glVertex2f(random3+32,car3+143);
    glVertex2f(random3+32,car3+150);
    glEnd();

    //Decrease car3 Y Axis with time
    car3--;

    //Check if Obstacle 3 Reached the bottom
    if(car3<-150)
    {
        //Reset Y Coordinate back to the top
        car3=0;
        //Get Random X Coordinate across the road length
        random3 = 1 + (rand() % 100);
    }

    //Check Player 1 Collisions with all PLayer 1 obstacles (enemies)
    player1CollisionCheck();

    //Dont check collision with enenmy 1 if collision already exists
    if(!((car1+150-udIndexP1)<10 && (car1+150-udIndexP1)> -3) && f1 == 1)
    {
        livesTemp1--;
        livesTemp2--;
        livesTemp3--;
        f1 = 0;
    }

    //Dont check collision enemy 2 if collision already exists
    if(!((car2+150-udIndexP1)<10 && (car2+150-udIndexP1)>-3) && f2 == 1)
    {
        livesTemp1--;
        livesTemp2--;
        livesTemp3--;
        f2 = 0;
    }

    //Dont check collision enemy 3 if collision already exists
    if(!((car3+150-udIndexP1)<10 && (car3+150-udIndexP1)>-3) && f3 == 1)
    {
        livesTemp1--;
        livesTemp2--;
        livesTemp3--;
        f3 = 0;
    }
}

//Spawn Player 2
void player2Spawn()
{
    //MAIN car
    //Front Tire
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndexP2+24,udIndexP2+5);
    glVertex2f(lrIndexP2+24,udIndexP2+7);
    glVertex2f(lrIndexP2+34,udIndexP2+7);
    glVertex2f(lrIndexP2+34,udIndexP2+5);
    glEnd();
    //Back Tire
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndexP2+24,udIndexP2+1);
    glVertex2f(lrIndexP2+24,udIndexP2+3);
    glVertex2f(lrIndexP2+34,udIndexP2+3);
    glVertex2f(lrIndexP2+34,udIndexP2+1);
    glEnd();
    //Car Body
    glColor3f(0.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(lrIndexP2+26,udIndexP2+1);
    glVertex2f(lrIndexP2+26,udIndexP2+8);
    glColor3f(0.00, 1.000, 0.000);
    glVertex2f(lrIndexP2+29,udIndexP2+10);
    glVertex2f(lrIndexP2+32,udIndexP2+8);
    glVertex2f(lrIndexP2+32,udIndexP2+1);
    glVertex2f(lrIndexP2+29,udIndexP2+-2);

    glEnd();
}

//Check Player 2 Collision with Player 1 Enemies and Player 2 Enemies
void player2CollisionCheck()
{
    //Checks if Player 2 Hit Player 2 car4 (Enemy 1)
    if((abs(lrIndexP2-random4)<8) && (car4+150-udIndexP2)<10 && (car4+150-udIndexP2)>-3)
    {
        //Reset
        start = 0;
        gv=1;
        p2lose = 1;
        game_ends = 1;

        //Test
        //cout  << "EnemyP2 1:" <<car4 << "P2:" << udIndexP2 << endl;
    }

    //Checks if Player 2 Hit Player 2 car5 (Enemy 2)
    if((abs(lrIndexP2-random5)<8) && (car5+150-udIndexP2)<10 && (car5+150-udIndexP2)>-3)
    {
        //Reset
        start = 0;
        gv=1;
        p2lose = 1;
        game_ends = 1;

        //Test
        //cout  << "EnemyP2 2:" <<car5 << "P2:" << udIndexP2 << endl;
    }

    //Checks if Player 2 Hit Player 2 car6 (Enemy 3)
    if((abs(lrIndexP2-random6)<8) && (car6+150-udIndexP2)<10 && (car6+150-udIndexP2)>-3)
    {
        //Reset
        start = 0;
        gv=1;
        p2lose = 1;
        game_ends = 1;

        //Test
        //cout  << "EnemyP2 3:" <<car6 << "P2:" << udIndexP2 << endl;
    }

    //Checks if Player 2 Hit Player 1 car1 (Enemy 1)
    if((abs(lrIndexP2-random1)<8) && (car1+150-udIndexP2)<10 && (car1+150-udIndexP2)>-3)
    {
        //Reset
        start = 0;
        gv=1;
        p2lose = 1;
        game_ends = 1;

        //Test
        //cout  << "EnemyP1 1:" <<car1 << "P2:" << udIndexP2 << endl;
    }

    //Checks if Player 2 Hit Player 1 car2 (Enemy 2)
    if((abs(lrIndexP2-random2)<8) && (car2+150-udIndexP2)<10 && (car2+150-udIndexP2)>-3)
    {
        //Reset
        start = 0;
        gv=1;
        p2lose = 1;
        game_ends = 1;

        //Test
        //cout  << "EnemyP1 2:" <<car2 << "P2:" << udIndexP2 << endl;
    }

    //Checks if Player 2 Hit Player 1 car3 (Enemy 3)
    if((abs(lrIndexP2-random3)<8) && (car3+150-udIndexP2)<10 && (car3+150-udIndexP2)>-3)
    {
        //Reset
        start = 0;
        gv=1;
        p2lose = 1;
        game_ends = 1;

        //Test
        //cout  << "EnemyP1 3:" <<car3 << "P2:" << udIndexP2 << endl;

    }
}

//Spawn Player 2 Obstacles (Enemies)
void player2ObstaclesSpawn()
{
    //Opposite car 1
        //Front Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random4+24,car4+146);
    glVertex2f(random4+24,car4+144);
    glVertex2f(random4+34,car4+144);
    glVertex2f(random4+34,car4+146);
    glEnd();
        //Back Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random4+24,car4+150);
    glVertex2f(random4+24,car4+148);
    glVertex2f(random4+34,car4+148);
    glVertex2f(random4+34,car4+150);
    glEnd();
        //Body
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random4+26,car4+150);
    glVertex2f(random4+26,car4+143);
    glVertex2f(random4+29,car4+141);
    glVertex2f(random4+32,car4+143);
    glVertex2f(random4+32,car4+150);
    glEnd();

    //Decrease car4 Y Axis with time
    car4--;

    //Check if car4 reached the bottom
    if(car4<-150)
    {
        //Resets Y Coordinate back to the top
        car4=0;
        //Gets random X Coordinate across the road length
        random4 = 1 + (rand() % 100);
    }

    //Opposite car 2
        //Front Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random5+24,car5+146);
    glVertex2f(random5+24,car5+144);
    glVertex2f(random5+34,car5+144);
    glVertex2f(random5+34,car5+146);
    glEnd();
        //Back Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random5+24,car5+150);
    glVertex2f(random5+24,car5+148);
    glVertex2f(random5+34,car5+148);
    glVertex2f(random5+34,car5+150);
    glEnd();
        //Body
    glColor3f(0.000, 0.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(random5+26,car5+150);
    glVertex2f(random5+26,car5+143);
    glVertex2f(random5+29,car5+141);
    glVertex2f(random5+32,car5+143);
    glVertex2f(random5+32,car5+150);
    glEnd();

    //Decrease car5 Y Coordinate with time
    car5--;

    //Checks if car5 reached the bottom
    if(car5<-150)
    {
        //Reset Y Coordinate back to the top
        car5=0;
        //Gets Random X Coordinate across the road length
        random5 = 1 + (rand() % 100);
    }


    //Opposite car 3
        //Front Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random6+24,car6+146);
    glVertex2f(random6+24,car6+144);
    glVertex2f(random6+34,car6+144);
    glVertex2f(random6+34,car6+146);
    glEnd();
        //Back Tyres
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random6+24,car6+150);
    glVertex2f(random6+24,car6+148);
    glVertex2f(random6+34,car6+148);
    glVertex2f(random6+34,car6+150);
    glEnd();
        //Body
    glColor3f(0.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(random6+26,car6+150);
    glVertex2f(random6+26,car6+143);
    glVertex2f(random6+29,car6+141);
    glVertex2f(random6+32,car6+143);
    glVertex2f(random6+32,car6+150);
    glEnd();

    //Decrease car6 Y Coordinate with time
    car6--;

    //Checks if car6 reached the bottom
    if(car6<-150)
    {
        //Reset Y coordinate to the top
        car6=0;
        //Get random X Coordinate across the road length
        random6 = 1 + (rand() % 100);
    }

    //Check Player 2 Collision with PLayer 1 Enemies and Player 2 Enemies
    player2CollisionCheck();

}

//Show First Heart
void heart1()
{
    //1st heart

    //1st curve
    glBegin(GL_POLYGON);
    glColor3f(1,0,0);
    for(float t=0;t<1;t=t+0.05){
            glVertex2f(pow((1-t),3) *88.5  + 3 * pow(1-t,2) * t * 103 +3*(1-t)*pow(t,2)*97 +pow(t,3)*87 , pow((1-t),3) *125 + 3 * pow(1-t,2) * t * 127.4 +3*(1-t)*pow(t,2)*135+pow(t,3)*130);

    }

    //2nd curve
    for(float t=0;t<1;t=t+0.05){
            glVertex2f(pow((1-t),3) *88.5+ 3 * pow(1-t,2) * t * 73 +3*(1-t)*pow(t,2)*80+pow(t,3)*90, pow((1-t),3) * 125 + 3 * pow(1-t,2) * t * 127.4 +3*(1-t)*pow(t,2)*135+pow(t,3)*130);

    }
    glEnd();
}

//Show Second Heart
void heart2()
{
    //2nd heart

    glBegin(GL_POLYGON);
    glColor3f(1,0,0);
    for(float t=0;t<1;t=t+0.05){
            glVertex2f(pow((1-t),3) *108.5  + 3 * pow(1-t,2) * t * 123 +3*(1-t)*pow(t,2)*117 +pow(t,3)*107 , pow((1-t),3) *125 + 3 * pow(1-t,2) * t * 127.4 +3*(1-t)*pow(t,2)*135+pow(t,3)*130);

    }

    for(float t=0;t<1;t=t+0.05){
            glVertex2f(pow((1-t),3) *108.5+ 3 * pow(1-t,2) * t * 93 +3*(1-t)*pow(t,2)*100 +pow(t,3)*110, pow((1-t),3) * 125 + 3 * pow(1-t,2) * t * 127.4 +3*(1-t)*pow(t,2)*135+pow(t,3)*130);

    }
    glEnd();
}

//Show Third Heart
void heart3()
{
    //3nd heart

    glBegin(GL_POLYGON);
    glColor3f(1,0,0);
    for(float t=0;t<1;t=t+0.05){
            glVertex2f(pow((1-t),3) *128.5  + 3 * pow(1-t,2) * t * 143 +3*(1-t)*pow(t,2)*137 +pow(t,3)*127 , pow((1-t),3) *125 + 3 * pow(1-t,2) * t * 127.4 +3*(1-t)*pow(t,2)*135+pow(t,3)*130);

    }

    for(float t=0;t<1;t=t+0.05){
            glVertex2f(pow((1-t),3) *128.5+ 3 * pow(1-t,2) * t * 113 +3*(1-t)*pow(t,2)*120 +pow(t,3)*130, pow((1-t),3) * 125 + 3 * pow(1-t,2) * t * 127.4 +3*(1-t)*pow(t,2)*135+pow(t,3)*130);

    }
    glEnd();
}

//Show Road, Players, Obstacles
void startGame()
{

    //Road
    glColor3f(0.412, 0.412, 0.412);
    glBegin(GL_POLYGON);
    glVertex2f(10,0);
    glVertex2f(10,150);
    glVertex2f(140,150);
    glVertex2f(140,0);
    glEnd();
    //Green Left
    glColor3f(0.000, 0.465, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(-40, 0);
    glVertex2f(-40, 150);
    glVertex2f(10, 150);
    glVertex2f(10, 0);
    glEnd();
    //Green Right
    glColor3f(0.000, 0.465, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(143, 0);
    glVertex2f(143, 150);
    glVertex2f(190, 150);
    glVertex2f(190, 0);
    glEnd();
    //Road Left Border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(10,0);
    glVertex2f(10,150);
    glVertex2f(13,150);
    glVertex2f(13,0);
    glEnd();

    //Road Right Border
    glColor3f(1.000, 1.000, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(140,0);
    glVertex2f(140,150);
    glVertex2f(143,150);
    glVertex2f(143,0);
    glEnd();

    //Road Middel1 Border
    //Top
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48,roadDivTop+120);
    glVertex2f(48,roadDivTop+140);
    glVertex2f(52,roadDivTop+140);
    glVertex2f(52,roadDivTop+120);
    glEnd();

    //Decrease Y Coordinate with time
    roadDivTop--;

    //Check if it reached the bottom
    if(roadDivTop<-140)
    {
        //Reset its Y Coordinate to the top
        roadDivTop=30;
        score++;
    }

    //Middle2
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48,roadDivMd2+80);
    glVertex2f(48,roadDivMd2+100);
    glVertex2f(52,roadDivMd2+100);
    glVertex2f(52,roadDivMd2+80);
    glEnd();

    //Decrease Y Coordinate with time
    roadDivMd2--;

    //Check if it reached the bottom
    if(roadDivMd2<-100)
    {
        //Reset its Y coordinate to the top
        roadDivMd2=70;
        score++;
    }

    //Middle3
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48,roadDivMd3+40);
    glVertex2f(48,roadDivMd3+60);
    glVertex2f(52,roadDivMd3+60);
    glVertex2f(52,roadDivMd3+40);
    glEnd();

    //Decrease Y Coordinate with time
    roadDivMd3--;

    //Check if it reached the bottom
    if(roadDivMd3<-60)
    {
        //Reset Y Coordinate to the top
        roadDivMd3=110;
        score++;
    }
    //Bottom
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(48,roadDivBtm+0);
    glVertex2f(48,roadDivBtm+20);
    glVertex2f(52,roadDivBtm+20);
    glVertex2f(52,roadDivBtm+0);
    glEnd();

    //Decrease Y coordinate with time
    roadDivBtm--;

    //Check if it reached the bottom
    if(roadDivBtm<-20)
    {
        //Reset its Y Coordinate to the top
        roadDivBtm=150;
        score++;
    }

    //Road Middle1 Border
    //Top
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(98,roadDivTop_1+120);
    glVertex2f(98,roadDivTop_1+140);
    glVertex2f(102,roadDivTop_1+140);
    glVertex2f(102,roadDivTop_1+120);
    glEnd();

    //Decrease Y Coordinate with time
    roadDivTop_1--;

    //Check if it reach the bottom
    if(roadDivTop_1<-140)
    {
        //Reset its Y Coordinate to the top
        roadDivTop_1=30;
        score++;
    }

    //Middle2
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(98,roadDivMd2_1+80);
    glVertex2f(98,roadDivMd2_1+100);
    glVertex2f(102,roadDivMd2_1+100);
    glVertex2f(102,roadDivMd2_1+80);
    glEnd();

    //Decrease Y coordinate with time
    roadDivMd2_1--;

    //Check if it reached the bottom
    if(roadDivMd2_1<-100)
    {
        //Reset its Y Coordinate to the top
        roadDivMd2_1=70;
        score++;
    }
    //Middle3
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(98,roadDivMd3_1+40);
    glVertex2f(98,roadDivMd3_1+60);
    glVertex2f(102,roadDivMd3_1+60);
    glVertex2f(102,roadDivMd3_1+40);
    glEnd();

    //Decrease Y coordinate with time
    roadDivMd3_1--;

    //Check if it reached the bottom
    if(roadDivMd3_1<-60)
    {
        //Reset its Y Coordinate to the top
        roadDivMd3_1=110;
        score++;
    }
    //Bottom
    glColor3f(1.000, 1.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(98,roadDivBtm_1+0);
    glVertex2f(98,roadDivBtm_1+20);
    glVertex2f(102,roadDivBtm_1+20);
    glVertex2f(102,roadDivBtm_1+0);
    glEnd();

    //Decrease Y Coordinate with time
    roadDivBtm_1--;

    //Check if it reached bottom
    if(roadDivBtm_1<-20)
    {
        //Reset its Y Coordinate to the top
        roadDivBtm_1=150;
        score++;
    }
        //side trees
    tree(150,yTree1);
    tree(-15,yTree1);
    yTree1--;
    if(yTree1 < -50)
        yTree1 = 150;
    tree(150,yTree2);
    tree(-15,yTree2);
    yTree2--;
    if(yTree2 < -50)
        yTree2 = 150;
    tree(150, yTree3);
    tree(-15,yTree3);
    yTree3--;
    if(yTree3 < -50)
        yTree3 = 150;
    tree(150, yTree4);
    tree(-15,yTree4);
    yTree4--;
    if(yTree4 < -50)
        yTree4 = 150;


    //Score Board
    glColor3f(0.000, 0.000, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(80,135);
    glVertex2f(135,135);
    glVertex2f(135,147);
    glVertex2f(80,147);
    glEnd();

    //Print Score
    char buffer [50];
    sprintf (buffer, "SCORE: %d", score);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(83,136,(void *)font3,buffer);

    //Print High Score
    if(mul != 1)
    {
        char buffer2 [50];
        sprintf (buffer, "HIGHSCORE: %d", high_score);
        glColor3f(0.000, 1.000, 0.000);
        renderBitmapString(83,140,(void *)font3,buffer);
    }

    //level Print
    if(score % 50 == 0)
    {
        int last = score /50;
        if(last!=level)
        {
            level = score /50;
            FPS=FPS+2;
        }
    }
    char level_buffer [50];
    sprintf (level_buffer, "LEVEL: %d", level);
    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(83,144,(void *)font3,level_buffer);

    //Spawn Player 1
    player1Spawn();
    //Spawn Player 1 Obstacles
    player1ObstaclesSpawn();


    //Multiplayer
    if(mul == 1)
    {
        //Spawn Player 2
        player2Spawn();
        //Spawn Player 2 Obstacles
        player2ObstaclesSpawn();
    }

    //Lives
    if(mul!=1){
        if(lives == 4)
        {
            //Show the 3 Hearts
            heart1();
            heart2();
            heart3();
        }
        else if(lives == 3)
        {
            //Show the 2 Hearts
            heart2();
            heart3();

        }
        else if(lives == 2)
        {
            //Show only 1 Heart
            heart3();
        }
    }

}

//Menu
void menuDesign()
{
    PlaySound("home.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);

    //Road Backgound
    glColor3f(0.000, 0.392, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(0,55);
    glVertex2f(100,55);
    glColor3f(0.604, 0.804, 0.196);
    glVertex2f(100,0);
    glVertex2f(0,0);
    glEnd();

    //Road Design In Front Page
    glColor3f(00, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(32-2+21,55);
    glVertex2f(32+58,50-50);
    glVertex2f(32-22,50-50);
    glEnd();
    //Road Middle
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(32-2+21,55);
    glVertex2f(50+2,50-50);
    glVertex2f(50-2,50-50);
    glEnd();

    //Road Sky
    glColor3f(0.000, 0.749, 1.000);
    glBegin(GL_POLYGON);
    glVertex2f(100,100);
    glVertex2f(0,100);
    glColor3f(0.686, 0.933, 0.933);
    glVertex2f(0,55);
    glVertex2f(100,55);
    glEnd();

    //Hill 1
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(20,55+10);
    glVertex2f(20+7,55);
    glVertex2f(0,55);
    glEnd();

    //Hill 2
    glColor3f(0.000, 0.502, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(20+15,55+12);
    glVertex2f(20+20+10,55);
    glVertex2f(0+10,55);
    glEnd();

    //Hill 4
    glColor3f(0.235, 0.702, 0.443);
    glBegin(GL_TRIANGLES);
    glVertex2f(87,55+10);
    glVertex2f(100,55);
    glVertex2f(60,55);
    glEnd();

    //Hill 3
    glColor3f(0.000, 0.502, 0.000);
    glBegin(GL_TRIANGLES);
    glVertex2f(70,70);
    glVertex2f(90,55);
    glVertex2f(50,55);
    glEnd();


    //Tree Left
    //Bottom
    glColor3f(0.871, 0.722, 0.529);
    glBegin(GL_TRIANGLES);
    glVertex2f(11,55);
    glVertex2f(12,55-10);
    glVertex2f(10,55-10);
    glEnd();
    //Up
    glColor3f(0.133, 0.545, 0.133);
    glBegin(GL_TRIANGLES);
    glVertex2f(11,55+3);
    glVertex2f(12+3,55-3);
    glVertex2f(10-3,55-3);
    glEnd();

    //Trees displayed in the menu
    tree(5,-15);
    tree(9,5);
    tree(85,9);
    tree(75,-5);


    //Menu Place Holder
    glColor3f(0.098, 0.098, 0.439);
    glBegin(GL_POLYGON);
    glVertex2f(32-4,50+10+10);
    glVertex2f(32+46,50+10+10);
    glVertex2f(32+46,50-15+10);
    glVertex2f(32-4,50-15+10);
    glEnd();

    glColor3f(00, 0, 0.000);
    glBegin(GL_POLYGON);
    glVertex2f(32-4,50+10+10);
    glVertex2f(32+46,50+10+10);
    glVertex2f(32+46,50+9+10);
    glVertex2f(32-4,50+9+10);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(32+45,50+10+10);
    glVertex2f(32+46,50+10+10);
    glVertex2f(32+46,50-15+10);
    glVertex2f(32+45,50-15+10);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(32-4,50-14+10);
    glVertex2f(32+46,50-14+10);
    glVertex2f(32+46,50-15+10);
    glVertex2f(32-4,50-15+10);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(32-4,50+10+10);
    glVertex2f(32-5,50+10+10);
    glVertex2f(32-5,50-15+10);
    glVertex2f(32-4,50-15+10);
    glEnd();

    //Instructions
    glColor3f(1.000, 1.000, 0.000);
    renderBitmapString(30,80,(void *)font1,"Project Racing Game ");

    glColor3f(0.000, 1.000, 0.000);
    renderBitmapString(30,50+15,(void *)font2,"Press SPACE to START");
    renderBitmapString(30,50+11,(void *)font2,"Press M for MULTIPLAYER");
    renderBitmapString(30,50-3+10,(void *)font2,"Press ESC to Exit");

    glColor3f(1.000, 1.000, 1.000);
    renderBitmapString(30,50-6+10,(void *)font3,"Press UP to move Forward");
    renderBitmapString(30,50-8+10,(void *)font3,"Press DOWN to move Backward");
    renderBitmapString(30,50-10+10,(void *)font3,"Press RIGHT to turn Right");
    renderBitmapString(30,50-12+10,(void *)font3,"Press LEFT to turn Left");


}

//Function Display for choose which display to show
void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // deleting the current color buffer
    //cout << glutGetWindow();

    if (start == 1)   // if start equals 1
    {
        glClearColor(0.000, 0.0, 0.000, 1); //RGB and the last parameter as the transparency, Dark color
        startGame(); // will call the function startGame
    }
    else if (gv == 1 && game_ends == 1) // if Game Over(gv) equals 1
    {
        if(mul == 0)
        {
            //call sound function
            soundGameOver();

            changeHighScore();
            glColor3f(0.000, 1.000, 0.000); // Color of the text
            renderBitmapString(33, 60 + 15, (void*)font1, "GAME OVER!!"); // mssg  // the void converts from int to void to
            glColor3f(0.000, 1.000, 0.000);
            char buffer2[50]; //Creating memory for a space of [50] characters
            sprintf(buffer2, "Your Best Score is : %d", high_score); //sprintf stands for "string print"
            renderBitmapString(18, 60, (void*)font1, buffer2);
            glColor3f(0.000, 1.000, 0.000);

            char buffer3[50]; //Creating memory for a space of [50] characters
            sprintf(buffer3, "Your Score is : %d", score); //sprintf stands for "string print"
            renderBitmapString(33, 60 - 5 + 10, (void*)font1, buffer3);


        }
        else if(mul == 1)
        {
            //call sound function
            soundWin();

            if(p1lose == 1)
            {

                glColor3f(0.000, 1.000, 0.000); // Color of the text
                renderBitmapString(35, 60 + 10, (void*)font1, "Player 2 Wins!"); // mssg  // the void converts from int to void to
                glColor3f(0.000, 1.000, 0.000);

                char buffer2[50]; //Creating memory for a space of [50] characters
                sprintf(buffer2, "Your Score is : %d", score); //sprintf stands for "string print"
                renderBitmapString(31, 60 - 5 + 10, (void*)font1, buffer2);

            }
            else if (p2lose == 1)
            {

                glColor3f(0.000, 1.000, 0.000); // Color of the text
                renderBitmapString(35, 60 + 10, (void*)font1, "Player 1 Wins!"); // mssg  // the void converts from int to void to
                glColor3f(0.000, 1.000, 0.000);

                char buffer2[50]; //Creating memory for a space of [50] characters
                sprintf(buffer2, "Your Score is : %d", score); //sprintf stands for "string print"
                renderBitmapString(31, 60 - 5 + 10, (void*)font1, buffer2);

            }
        }
    }
    else
    {
        menuDesign(); // else it will go to the function first design
    }

    glFlush();
    glutSwapBuffers();
}

//PLayer 2 Controls
void player2Controls(int key, int x, int y)
{
    switch (key)
    {
    //Press down arrow
    case GLUT_KEY_DOWN:
        //Check if he can get down any further
        if(udIndexP2 >= 0)
        {
            //if he can, decrease Player 2 Y Coordinate
            udIndexP2 -= (FPS/25);
            if(udIndexP2 < 0)
            {
                udIndexP2 = 0;
            }
        }
        break;
    //Press up arrow
    case GLUT_KEY_UP:
        //Check if he can get up any further
        if(udIndexP2 <= 125)
        {
            //if he can, Increase Player 2 Y Coordinate
            udIndexP2 += (FPS/25);
            if(udIndexP2 > 125)
            {
                udIndexP2 = 125;
            }
        }
        break;
    //Press Left Arrow
    case GLUT_KEY_LEFT:
        //Check if he can move left any further
        if(lrIndexP2>=0)
        {
            //if he can, decrease Player 2 X coordinate
            lrIndexP2 = lrIndexP2 - (FPS/25);
            if(lrIndexP2<0)
            {
                lrIndexP2=-1;
            }
        }
        break;
    //Press Right Arrow
    case GLUT_KEY_RIGHT:
        if(lrIndexP2<=99)
        {
            //If he can, Increase Player 2 X Coordinate
            lrIndexP2 = lrIndexP2 + (FPS/25);
            if(lrIndexP2>99)
            {
                lrIndexP2 = 100;
            }
        }
        break;

    default:
        break;
    }

}

//reset variables everytime player wants to play again
void resetVariables()
{
    gv = 0;
    start = 1;
    //Game Speed
    FPS = 50;

    //Road
    roadDivTopMost = 0;
    roadDivTop = 0;
    roadDivMd2 = 0;
    roadDivMd3 = 0;
    roadDivBtm = 0;
    roadDivTop_1 = 0;
    roadDivMd2_1 = 0;
    roadDivMd3_1 = 0;
    roadDivBtm_1 = 0;

    //Player1 Positions
    lrIndexP1 = 0 ;
    udIndexP1 = 0;

    //Player2 Positions
    lrIndexP2 = 100;
    udIndexP2 = 0;

    //PLayer 1 Obstacles
        //Y Coordinates
    car1 = 0;
    car2 = 30;
    car3 = 60;
        //X Coordinates
    random1=0;
    random2=0;
    random3=0;

    //Player 2 Obstacles
        //Y Coordinates
    car4 = 0;
    car5 = 30;
    car6 = 60;
        //X Coordinates
    random4 = 100;
    random5 = 100;
    random6 = 100;

    score=0;
    level=0;

    //Multiplayer Variables
    p1lose = 0;
    p2lose = 0;

    //Reset Lives
    f1 = 0;
    f2 = 0;
    f3 = 0;
    lives = 4;
    livesTemp1 = lives - 1;
    livesTemp2 = lives - 1;
    livesTemp3 = lives - 1;
}

//Player 1 Keys + General Keys
void processKeys(unsigned char key, int x, int y)
{
    if(game_ends == 0)
    {
        switch(key)
        {
        //if pressed space: single player
        case ' ':
            if(start==0)
            {
                //Reset Variables
                resetVariables();
                mul = 0;

                glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
                glutInitWindowSize(500,650);
                glutInitWindowPosition(200,20);
                glutCreateWindow("Car Game");

                glutDisplayFunc(display);

                //Player 1 Controls + General Controls
                glutKeyboardFunc(processKeys);

                glOrtho(-40,190,0,150,-1,1);
                glClearColor(0.184, 0.310, 0.310,1);
            }
            break;
        //if pressed m: multiplaer
        case 'm':
            if(start == 0)
            {
                //Reset Variables
                resetVariables();
                mul = 1;

                glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
                glutInitWindowSize(500,650);
                glutInitWindowPosition(200,20);
                glutCreateWindow("Car Game");

                glutDisplayFunc(display);

                //Player 2 Controls
                glutSpecialFunc(player2Controls);

                //Player 1 Controls + General Controls
                glutKeyboardFunc(processKeys);

                glOrtho(-40,190,0,150,-1,1);
                glClearColor(0.184, 0.310, 0.310,1);
            }
            break;
        //if pressed esc: exit
        case 27:
            exit(0);
            break;

        //Player 1 Movement
        case 's':
            //Player 1 moves down
            if(udIndexP1 >= 0)
            {
                //Decrease Player 1 Y Coordinate
                udIndexP1 -= (FPS/25);
                if(udIndexP1 < 0)
                {
                    udIndexP1 = 0;
                }
            }
            break;
        case 'w':
            //Player 1 moves up
            if(mul == 1)
            {
                //Multiplayer top limit
                if(udIndexP1 <= 125)
                {
                    //Increase Player 1 Y Coordinate
                    udIndexP1 += (FPS/25);
                    if(udIndexP1 > 125)
                    {
                        udIndexP1 = 125;
                    }
                }
            }
            //Single player top limit
            else if( mul != 1)
            {
                if(udIndexP1 <= 110)
                {
                    //Increase Player 1 Y Coordinate
                    udIndexP1 += (FPS/25);
                    if(udIndexP1 > 110)
                    {
                        udIndexP1 = 110;
                    }
                }
            }
            break;

        case 'a':
            //Player 1 moves left
            if(lrIndexP1>=0)
            {
                //Decrease Player 1 X Coordinate
                lrIndexP1 = lrIndexP1 - (FPS/25);
                if(lrIndexP1<0)
                {
                    lrIndexP1=-1;
                }
            }
            break;


        case 'd':
            //Player 1 moves right
            if(lrIndexP1<=99)
            {
                //Increase Player 1 X Coordinate
                lrIndexP1 = lrIndexP1 + (FPS/25);
                if(lrIndexP1>99)
                {
                    lrIndexP1 = 100;
                }
            }
            break;
        default:
            break;
        }
    }
    //If game ends, press enter to close the lose window and enter ur new game mode selection (single or multiplayer)
    else if (game_ends == 1)
    {
        switch(key)
        {
        case 13:
            glutSetWindow(1);
            glutDestroyWindow(2);
            game_ends = 0;
            break;
        }
    }
}

//Increase speed with time
void timer(int)
{
    FPS += 0.01;
    glutPostRedisplay();
    glutTimerFunc(1000/FPS,timer,0);
}


int main(int argc, char *argv[])
{
    //Open highscore text file saved in project file named text.txt
    FILE *file;
    file = fopen("text.txt","r");
    fscanf(file,"%d", &high_score);
    fclose(file);

    //Changes Randomness seed of spawning obstacles' X Coordinates
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500,650);
    glutInitWindowPosition(200,20);
    glutCreateWindow("Start Screen");

    glutDisplayFunc(display);
    glutSpecialFunc(player2Controls);
    glutKeyboardFunc(processKeys); //and player 1

    glOrtho(0,100,0,100,-1,1);

    glClearColor(0.184, 0.310, 0.310,1);

    glutTimerFunc(1000/FPS,timer,0);
    glutMainLoop();
    return 0;
}
