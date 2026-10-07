#ifndef TETRIS_CPP_
#define TETRIS_CPP_
#include "util.h"
#include <iostream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<ctime>
#include<string>
#include<sys/wait.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sstream>
#include<cmath>    
#include<fstream>
using namespace std;

void SetCanvasSize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1); 
    glMatrixMode( GL_MODELVIEW);
    glLoadIdentity();
}

int meters = 1;                            //Distance Covered By Object
int speed = 1;                             //Game speed controller
int x5 = 380 , y5 = 100;                   //X an Y axis of Object
int x4[4] = { 310 , 400 , 500 , 600 } , y4[4] = {800, 1050, 900, 1500};   //Enemies X and Y axis  
int lives = 3;
int score = 0;
string finalscore ;
string livescore;
bool stop = false;                          //For pause and resume
bool start = false;   
int stary[100] , starx[100];                  //Stars x-axis and y-axis
int bltx[3] , blty[3] = { -1 , -1 , -1 };   //Bullets x-axis and y-axis  
string name;                                //User name
string hearts;
string meter;
bool restart = false;
bool state = false;
string previous[15];
bool l = false;
void Display() {
    
    //Game Interface
    if ( start == false ) {                
      DrawSquare( 0 , 0 , 900 , colors[BLACK] );
      DrawString( 238, 550, "---Galaxy Defender Game---", colors[RED]);
      DrawString( 130, 500, "Enter Your Name: ", colors[RED]);
      DrawString( 350, 500, name , colors[WHITE]);
      DrawString( 265, 450, "Press Enter To Start Game", colors[RED]);
      DrawString( 235, 400, "Press B To See Previous Records", colors[RED]);
      for( int i = 0 ; i < 100 ; i++ ) {
        DrawCircle( starx[i] , stary[i] , 1 , colors[16] );                      
      } 
      ifstream data;
      data.open("playerdata.txt");
      if(data.is_open()) {
        for( int i = 0; i < 15 ; i++ ) {
          getline(data,previous[i]);
        }
      }
      data.close();
    }
    
    //Pause and Resume Interface
    else if ( stop == true ) {
      DrawSquare( 0 , 0 , 900 , colors[BLACK] );
      for( int i = 0 ; i < 100 ; i++ ) {
        DrawCircle(starx[i],stary[i],1,colors[16]);                      
      } 
      DrawString( 350, 600, "MENU", colors[RED]);
      DrawString( 325, 500, "Resume (R/r)", colors[RED]);
      DrawString( 330, 450, "Restart (A/a)", colors[RED]);
      DrawString( 270, 400, "Press B To See Previous Records", colors[RED]);
      DrawString( 270, 3350, "Press ESC To End Game", colors[RED]);
    }
    
    //Game Play
    else {
      glClearColor( 0 , 0.0 , 0.0, 0 );
      glClear(GL_COLOR_BUFFER_BIT);
    
      //Stars
      for( int i = 0 ; i < 100 ; i++ ) {
        DrawCircle(starx[i],stary[i],1,colors[16]);
        stary[i] -= 1;
        if( stary[i] <= 0 ) {
          stary[i] = 800;                         
        }
      } 
    
      livescore = "Score: " + to_string(score);
      hearts = "Lives: " + to_string(lives);
      meter = "Meters: " + to_string(meters);
      DrawString( 10 , 755 , livescore , colors[MISTY_ROSE]);
      DrawString( 10 , 715 , hearts , colors[MISTY_ROSE]);
      DrawString( 10 , 670 , meter , colors[MISTY_ROSE]);
   
      //Enemy Generator
      for( int i = 0 ; i < 4 ; i++ ) {
        DrawTriangle( x4[i] , y4[i] , x4[i] + 20 , y4[i] , x4[i] + 10 , y4[i] - 75 , colors[RED] );
        DrawTriangle( x4[i] - 20 , y4[i] - 30 , x4[i] + 40 , y4[i] - 30 , x4[i] + 11 , y4[i] - 50 , colors[MAROON] );
      }
    
    
      //Boundaries
      DrawLine( 150 , 0 ,  150 , 800 , 3 , colors[GRAY] );
      DrawLine( 650 , 0 ,  650 , 800 , 3 , colors[GRAY] ); 
    
      //Main Object
      DrawTriangle( x5 , y5 - 75 , x5 + 20 , y5 - 75 , x5 + 10 , y5, colors[AQUA] );
      DrawTriangle( x5 - 20 , y5 - 45 , x5 + 40 , y5 - 45 , x5  + 11 , y5 - 25, colors[GOLD] );
    
      //Bullets Generator
      for( int i = 0 ; i < 3 ; i++ ) {
        if( blty[i] != -1 ) {     
          DrawTriangle( bltx[i] , blty[i] , bltx[i] + 6 , blty[i] , bltx[i] + 3 , blty[i] + 20 , colors[ORANGE] );
        }
      }
    
      //Bullets Collision
      for( int i = 0; i < 4; i++ ) {
        for ( int j = 0; j < 3; j++ ) {
          if((abs(bltx[j] - x4[i]) < 40) && (abs(blty[j] - y4[i]) < 40)) {
            blty[j] = -1;
            y4[i] = 1100;             
            x4[i] = 170 + rand() % 440;
            score += 10;
          }
        }
      }

      //Planes Collision
      for( int i = 0; i < 4; i++ ) {
        if( (abs(x5 - x4[i]) < 60) && (abs(y5 - y4[i]) < 30)) {
          DrawSquare( 0 , 0 , 800 , colors[WHITE] );
          y4[i] = 1100;            
          DrawSquare( 0 , 0 , 800 , colors[WHITE] );
          x4[i] = 170 + rand() % 440;
          DrawSquare( 0 , 0 , 800 , colors[WHITE] );
          lives--;
        }  
      }
    
      //Flash For Losing Life
      for ( int i = 0; i < 3; i++ ) {
        if( y4[i] - 35 < 10 ) {
          DrawSquare( 0 , 0 , 800 , colors[WHITE] );
        }
      }  
    
      //Speed Increases After 1000 Score
      if( meters % 1000 == 0 ) {
        speed += 1;
      }
      meters++; //Game pixels increases and total distance is measured
   }
   
   if( lives <= 0 ) {
      state = false;
      finalscore = name + " Scored: " + to_string(score);
      DrawSquare( 0 ,  0 , 950 , colors[130] );
      DrawString( 310 , 550 , "GAME OVER" , colors[1]);
      DrawString( 290 , 500 , finalscore , colors[WHITE]);  
      DrawString( 290 , 450 , "Press a/A To Restart" , colors[WHITE]);
      DrawString( 270 , 400 , "Press ESC To Exit Game" , colors[WHITE]);
      DrawString( 235, 400, "Press B To See Previous Records", colors[WHITE]);
      if( score >= 100 ) {
        DrawString( 330 , 580, "You WON!!!", colors[RED]);
      }
      else {
        DrawString( 330 , 580, "You Lose!!!", colors[RED]);
      }
   }
   
   if( l == true ) {
    DrawSquare( 0 ,  0 , 950 , colors[130] );
    DrawString( 300 , 750 , "Previous Records" , colors[WHITE]);
    DrawString( 150 , 650 , "Name" , colors[RED]);
    DrawString( 400 , 650 , "Score" , colors[RED]);
    DrawString( 650 , 650 , "Lives" , colors[RED]);
    int k = 600;   
    int c = 150;
    for(int i = 1; i <= 15; i++) {
        DrawString( c , k , previous[i-1] , colors[WHITE]);
        c += 250;
        if( i % 3 == 0) {
          k -= 70;
          c = 150;
        }  
    }  
   }
   glutSwapBuffers();
}     //End of void display

void NonPrintableKeys(int key, int x, int y) {
    
    if (key == GLUT_KEY_LEFT) {
      x5 -= 30;
      if ( x5 < 180 ) {
        x5 = 180;
      }
    }
    
    else if (key == GLUT_KEY_RIGHT) {
      x5 += 30;
      if ( x5 > 600 ) {
        x5 = 600;
      }
    }  
    
    glutPostRedisplay();
}

void PrintableKeys(unsigned char key, int x, int y) {

    if (key == KEY_ESC) { 
      
      ofstream fout("playerdata.txt",ios::app);
      if(fout.is_open()) {
        fout << name << endl;
        fout << score << endl;
        fout << lives << endl;
        fout.close();
      }
      
        exit(1);
    }
    if ((key == 'R' || key=='r') && start == true) {   
        stop = false;
    }
    
    if ((key == 'P' || key=='p') && start == true) {  
        stop = true;
    }
    
    if (int(key) == 13) {        
        start = true;
        state = true;
    }
    
    if ((key == 'b' || key=='B') && start == true) {  
        l = true;
    }
    
    if ((key == 'a' || key=='A') && start == true) {  
          restart = true;
          ofstream fout("playerdata.txt",ios::app);
          if(fout.is_open()) {
            fout << name << "\t\t\t";
            fout << score << "\t\t";
            fout << lives <<endl;
            fout.close();
          }
          lives = 3;
          score = 0;
          meters = 0;
          start = false;
          stop = false;
          speed = 1;
          int count = 800;
          for ( int  i = 0 ; i < 4; i++) {
            y4[i] = count; 
            count += 100;
          }
          name = "";
    }
    
    else if( key != 13 && start == false && lives > 0 ) {
        name += key;
    }
    
    if( key == ' ' && start == true ) {
      for(int i = 0 ; i < 3 ; i++ ) {
        if( blty[i] == -1 ) {
            bltx[i] = x5 + 5;
            blty[i] = y5;
            break;          
        }
      }
    }
   
    glutPostRedisplay();
}

void Timer(int m) { 
  if (state == true) {
    for( int i = 0 ; i < 4 ; i++ ) {   
      y4[i] -= 1 * speed;
      if( y4[i] - 30 < 10 ) {
        y4[i] = 900; 
        lives--;
        x4[i] = 170 + rand() % 440;
        // Add this back in Timer after respawn
        for ( int j = 0; j < 4 ; j++ ) {
          if((abs(x4[i] - x4[j]) < 80) && (i != j)) {
            x4[i] = 170 + rand() % 440;
          }
        }
      }
    }
    for( int i = 0 ; i < 3 ; i++ ) {
      if(blty[i] != -1) {
        blty[i] += 15;
        if(blty[i] > 800) {
          blty[i] = -1;
        }
      }
    }
  }  
    glutPostRedisplay();
    glutTimerFunc(1000.0 / FPS, Timer, 0);
}

int main(int argc, char*argv[]) {
    int width = 800, height = 800; 
    for( int i = 0; i < 100; i++ ) {   
      stary[i] = rand() % 800;
      starx[i] = rand() % 800;
    }
    ofstream fout;
    fout.open("playerdata.txt",ios::app);
    if( !fout.is_open() ) {
      cout << "There is an error in the file!" << endl;
      return 1;
    }
    fout.close(); 
    InitRandomizer(); 
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
    glutInitWindowPosition(250, 250); 
    glutInitWindowSize(width, height); 
    glutCreateWindow("Galaxy Defender Game"); 
    SetCanvasSize(width, height); 
    glutDisplayFunc(Display); 
    glutSpecialFunc(NonPrintableKeys); 
    glutKeyboardFunc(PrintableKeys); 
    glutTimerFunc(100.0 / FPS, Timer, 0);
    glutMainLoop();
    return 1;
}
#endif
