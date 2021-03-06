#include <GL/glut.h>
#include <GL/gl.h>
#include <AL/alut.h>

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "data.h"

//Compile Command: gcc -o car FormulaTwo.c -lglut -lalut -lGL -lopenal -lGLU -pthread -w

struct computerCarData ccdata;
struct playerCarData pcdata;
struct fuelBarData fbdata;

#define NUM_BUFFERS 6
#define NUM_SOURCES 6
#define NUM_ENVIRONMENTS 1

#define accident 2
#define race 0
#define angryBoy 3
#define start 1

ALfloat listenerPos[]={0.0,0.0,4.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat	listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};

ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

ALfloat source1Pos[]={ 2.0, 0.0, 0.0};
ALfloat source1Vel[]={ 1.0, 0.0, 0.0};

ALfloat source2Pos[]={ 4.0, 0.0, 0.0};
ALfloat source2Vel[]={ 1.0, 0.0, 0.0};

ALfloat source3Pos[]={ 3.0, 0.0, 0.0};
ALfloat source3Vel[]={ 1.0, 0.0, 0.0};

ALfloat source4Pos[]={ 5.0, 0.0, 0.0};
ALfloat source4Vel[]={ 1.0, 0.0, 0.0};

ALfloat source5Pos[]={ 6.0, 0.0, 0.0};
ALfloat source5Vel[]={ 1.0, 0.0, 0.0};

ALuint	buffer[NUM_BUFFERS];
ALuint	source[NUM_SOURCES];
ALuint  environment[NUM_ENVIRONMENTS];

ALsizei size,freq;
ALboolean al_bool;
ALenum 	format;
ALvoid 	*data;
int block = 0;

int score1 = 48;
int score2 = 48;
int score3 = 48;
int score4 = 48;
int score5 = 48;
int sycle = 0;

sem_t sem1;
int flag;
int ticket1;

int mainWindow;
int pauseWindow;

long time1;
long time2;
int t1,t2,t3;

int tt1,tt2;

int fcounter;
/////////////------ MIDDLELINE ------////////////////
GLfloat x=10 , y=158;
GLfloat sum =0;  
////////////-----------------------------------------

/////////----- CAR --- COMPUTER-----////////////////
int j=0;
GLfloat f ;
GLfloat ff ;
GLfloat vertex[1000];
////////---------------------------------------------

float Timer=0;

int block2 = 0;

void moveComputerCar(void *p)
{
  while(1)
  {
    while(ticket1 != 0);
    if(rand()%100 > 85)
    {
      ccdata.fuelFlag = 1;
    }
    ccdata.now = rand()%2;
    if(ccdata.now == ccdata.preview)
    {
      ccdata.preview = ccdata.now;
      ccdata.xx = ccdata.x + ccdata.step;
      ccdata.step = ccdata.step + 160 + ccdata.distance;
      
      if(ccdata.now == 0)
	ccdata.yy = 0;
      else
	ccdata.yy = ccdata.y;
	
    }
    else
    {
      ccdata.preview = ccdata.now;
      ccdata.xx = ccdata.x + ccdata.step + 200;
      ccdata.step = ccdata.step + 400 +ccdata.distance;
      
      if(ccdata.now == 1)
	ccdata.yy = ccdata.y;
      else
	ccdata.yy = 0;
    }
    ticket1 = 1;
  }
}

void   initAL(void)
{
	alutInit(0, NULL);

    alListenerfv(AL_POSITION,listenerPos);
    alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);
    
    alGetError(); // clear any error messages
    
    if(alGetError() != AL_NO_ERROR) 
    {
        printf("- Error creating buffers !!\n");
        exit(1);
    }
    else
    {
        printf("init() - No errors yet.");

    }
    
    // Generate buffers, or else no sound will happen!
    alGenBuffers(NUM_BUFFERS, buffer);
    
	char theme[14] = {'s','o','u','n','d','s','/','0','0','.','w','a','v','\0'};
	srand(time(0));
	int num = rand()%16;

	if(num < 10)
	{
		theme[7] = '0';
		theme[8] = (char)(num+48);
	}
	else
	{
		theme[7] = '1';
		theme[8] = (char)(num%10 + 48);
	}

	printf("\n%s\n",theme);
	printf("\n%d\n",num);
	printf("\n%c\n",(char)(num+48));

    alutLoadWAVFile((ALbyte *)"sounds/race.wav",&format,&data,&size,&freq,&al_bool);
    alBufferData(buffer[0],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

	alutLoadWAVFile((ALbyte *)"sounds/start.wav",&format,&data,&size,&freq,&al_bool);
    alBufferData(buffer[1],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

	alutLoadWAVFile((ALbyte *)"sounds/accident.wav",&format,&data,&size,&freq,&al_bool);
    alBufferData(buffer[2],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

	alutLoadWAVFile((ALbyte *)theme,&format,&data,&size,&freq,&al_bool);
    alBufferData(buffer[3],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

	alutLoadWAVFile((ALbyte *)"sounds/good.wav",&format,&data,&size,&freq,&al_bool);
    alBufferData(buffer[4],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

	alutLoadWAVFile((ALbyte *)"sounds/game_over.wav",&format,&data,&size,&freq,&al_bool);
    alBufferData(buffer[5],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);


    alGetError(); /* clear error */
    alGenSources(NUM_SOURCES, source);

    if(alGetError() != AL_NO_ERROR) 
    {
        printf("- Error creating sources !!\n");
        exit(2);
    }
    else
    {
        printf("init - no errors after alGenSources\n");
    }

    alSourcef(source[race],AL_PITCH,0.8f);
    alSourcef(source[race],AL_GAIN,0.5f);
    alSourcefv(source[race],AL_POSITION,source0Pos);
    alSourcefv(source[race],AL_VELOCITY,source0Vel);
    alSourcei(source[race],AL_BUFFER,buffer[0]);
    alSourcei(source[race],AL_LOOPING,AL_TRUE);

	alSourcef(source[start],AL_PITCH,1.0f);
    alSourcef(source[start],AL_GAIN,1.9f);
    alSourcefv(source[start],AL_POSITION,source1Pos);
    alSourcefv(source[start],AL_VELOCITY,source1Vel);
    alSourcei(source[start],AL_BUFFER,buffer[start]);
    alSourcei(source[start],AL_LOOPING,AL_TRUE);

	alSourcef(source[accident],AL_PITCH,1.0f);
    alSourcef(source[accident],AL_GAIN,10.7f);
    alSourcefv(source[accident],AL_POSITION,source2Pos);
    alSourcefv(source[accident],AL_VELOCITY,source2Vel);
    alSourcei(source[accident],AL_BUFFER,buffer[accident]);
    alSourcei(source[accident],AL_LOOPING,AL_TRUE);

	alSourcef(source[angryBoy],AL_PITCH,1.0f);
    alSourcef(source[angryBoy],AL_GAIN,0.3f);
    alSourcefv(source[angryBoy],AL_POSITION,source3Pos);
    alSourcefv(source[angryBoy],AL_VELOCITY,source3Vel);
    alSourcei(source[angryBoy],AL_BUFFER,buffer[angryBoy]);
    alSourcei(source[angryBoy],AL_LOOPING,AL_TRUE);

	alSourcef(source[4],AL_PITCH,1.0f);
    alSourcef(source[4],AL_GAIN,5.7f);
    alSourcefv(source[4],AL_POSITION,source4Pos);
    alSourcefv(source[4],AL_VELOCITY,source4Vel);
    alSourcei(source[4],AL_BUFFER,buffer[4]);
   // alSourcei(source[4],AL_LOOPING,AL_TRUE);

	alSourcef(source[5],AL_PITCH,1.0f);
    alSourcef(source[5],AL_GAIN,1.7f);
    alSourcefv(source[5],AL_POSITION,source5Pos);
    alSourcefv(source[5],AL_VELOCITY,source5Vel);
    alSourcei(source[5],AL_BUFFER,buffer[5]);
   // alSourcei(source[5],AL_LOOPING,AL_TRUE);

}

void init(void)
{
  glClearColor(0.3,0.8,1.0,1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0,800.0,0.0,300.0);
}


void road(void)
{
  
  int i = 0;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glColor3f(0.0,0.0,0.0);
  
//////////////----ROAD----////////////////
  glBegin(GL_QUADS);
    glVertex2f(0.0,68.0);
    glVertex2f(800.0,68.0);
    glVertex2f(800.0,240.0);
    glVertex2f(0.0,240.0);
  glEnd();

////////////------- SCORE Board ----------////////////
  sycle +=1;
  if(sycle == 10)
  {
    score1 += 1;
	if(score1 == 58)
	{
		score2 += 1;
		score1 = 48;
	}
	if(score2 == 58)
	{
		score3 += 1;
		score2 = 48;
	}
	if(score3 == 58)
	{
		score4 += 1;
		score3 = 48;
	}
	if(score4 == 58)
	{
		score5 += 1;
		score4 = 48;
	}
	sycle = 0;
  }
  
  
  
  glBegin(GL_QUADS); 
    glColor3f(0.445,0.445,.413);
    glVertex2f(490,287.0);
    glVertex2f(795,287.0);
    glColor3f(0.645,0.545,.613);
    glVertex2f(795.0,264.0);
    glVertex2f(490,264.0);
  glEnd();
  
  glColor3f(0.8,0.0,0.0);
    
//  glRasterPos2i(650, 269);
  glRasterPos2i(675, 269);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'S');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'c');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'o');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'r');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'e');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ':');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ' ');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)score5);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)score4);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)score3);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)score2);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)score1);

//////////////----FUEL BAR----////////////
  
  glBegin(GL_QUADS);
    glColor3f(1.0,0.0,0.0);
    glVertex2f(60.0,283.0);
    glColor3f(.0,1.0,0.0);
  //  glVertex2f(fbdata.xLeft1,283.0);
  //  glVertex2f(fbdata.xLeft2,268.0);
    glVertex2f(470,283.0);
    glVertex2f(470,268.0);
    glColor3f(1.0,0.0,0.0);
    glVertex2f(60.0,268.0);
  glEnd();
    
  glBegin(GL_QUADS); 
    glColor3f(1.0,1.0,1.0);
    glVertex2f(fbdata.xRight1,283.0);
    glVertex2f(470,283.0);
    glVertex2f(470.0,268.0);
    glVertex2f(fbdata.xRight2,268.0);
  glEnd();
  
  glBegin(GL_LINE_STRIP);//Line of around the fuel bar 
    glColor3f(.0,.0,.0);
    
    glVertex2f(60,283.0);
    glVertex2f(470,283.0);
   
    glVertex2f(470.0,268.0);
    glVertex2f(60,268.0);
    
    glVertex2f(60,283.0);
    glVertex2f(470,283.0);
  
    glVertex2f(60,283.0);
    glVertex2f(470,283.0);
  glEnd();
  
  fbdata.xLeft1=fbdata.xLeft1-fbdata.decSpeed;
  fbdata.xLeft2=fbdata.xLeft2-fbdata.decSpeed;
  fbdata.xRight1=fbdata.xRight1-fbdata.decSpeed;
  fbdata.xRight2=fbdata.xRight2-fbdata.decSpeed;
  
  glColor3f(.7,0.0,.0);
    
  glRasterPos2i(9, 268);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'F');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'U');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'E');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'L');
  
  if(fbdata.xRight1<60 )
  {
    glColor3f(1.0,0.0,.0);
    
    glRasterPos2i(215, 271);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'F');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'U');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'E');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'L');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'F');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'I');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'N');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'I');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'S');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'H');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'E');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'D');
    
    glColor3f(1.0,0.0,1.0);
    
    glRasterPos2i(345, 145);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'G');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'a');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'm');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'e');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'v');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'e');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'r');

	alSourceStop(source[0]);
	alSourceStop(source[3]);
	alSourcePlay(source[5]);

    glColor3f(1.0,0.84,.0);
    
    glRasterPos2i(500, 269);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'T');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'I');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'M');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'E');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ':');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)t3);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)t2);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)t1);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 's');
  
    glutSwapBuffers();
	
    sleep(7);
    exit(0);
  }
 glColor3f(1.00,1.0,1.0);

////////////----MIDDLELINE-----////////////  
  glColor3f(1.00,1.0,1.0);
  glLineWidth(10);  
  GLfloat xx=x-sum;
  for(i = 0 ; i<1000 ; i++)
  {
    glBegin(GL_LINES);
      glVertex2f(xx,y);
      glColor3f(1.00,1.0,1.0);
      glVertex2f(xx+70.0,y);
    glEnd();

    xx =xx+180.0;
  }
  sum = sum+ccdata.speed-2;

///////////----‌BORDER LINE-----/////////////  
  glColor3f(1,1.5,1.0);
  glLineWidth(10);  
  GLfloat xxx=x-sum;
  
  glBegin(GL_LINES);
      glVertex2f(0.00,y-95);
      glColor3f(1.0,1.5,1.0);
      glVertex2f(800.0,y-95.0);
      
      glVertex2f(0.0,y+87.0);
      glVertex2f(800.0,y+87.0);
  glEnd();
  
  for(i = 0 ; i<1000 ; i++)
  {
    glBegin(GL_LINES);
      glVertex2f(xxx,y-95);
      glColor3f(0.7,0.5,0.0);
      glVertex2f(xxx+50.0,y-95.0);
      
      glVertex2f(xxx,y+87.0);
      glVertex2f(xxx+50.0,y+87.0);
    glEnd();

    xxx =xxx+70.0;
  }
////////////---car---player///////////////////////
  glLineWidth(2);
  glColor3f(1.0,0.5,1.0);
  
  glBegin(GL_QUADS);
    glColor3f(0.7,0.5,1.0);

    glVertex2f(147.0,142.0 + pcdata.yplus);
    glVertex2f(160.0,142.0 + pcdata.yplus);
    glVertex2f(160.0,82.0 + pcdata.yplus);
    glVertex2f(147.0,82.0 + pcdata.yplus);
    glVertex2f(150.0,117.0 + pcdata.yplus);
    glVertex2f(150.0,107.0 + pcdata.yplus);
    glVertex2f(55.0,103.0 + pcdata.yplus);
    glVertex2f(55.0,121.0 + pcdata.yplus);
    glVertex2f(35.0,144.0 + pcdata.yplus);
    glVertex2f(55.0,144.0 + pcdata.yplus);
    glVertex2f(55.0,80.0 + pcdata.yplus);
    glVertex2f(35.0,80.0 + pcdata.yplus);
    glVertex2f(10.0,144.0 + pcdata.yplus);
    glVertex2f(10.0,125.0 + pcdata.yplus);
    glVertex2f(35.0,125.0 + pcdata.yplus);
    glVertex2f(35.0,144.0 + pcdata.yplus);
    glVertex2f(35.0,99.0 + pcdata.yplus);
    glVertex2f(10.0,99.0 + pcdata.yplus);
    glVertex2f(10.0,80.0 + pcdata.yplus);
    glVertex2f(35.0,80.0 + pcdata.yplus);     
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glVertex2f(55.0,121.0 + pcdata.yplus);
    glVertex2f(125.0,135.0 + pcdata.yplus);
    glVertex2f(125.0,117.0 + pcdata.yplus); 

    glVertex2f(55.0,103.0 + pcdata.yplus);
    glVertex2f(125.0,89.0 + pcdata.yplus);
    glVertex2f(125.0,107.0 + pcdata.yplus);
  glEnd();

/////////----- SHOW TIME ---------///////////////
  
  time2 = time(0);
  if(time2-time1 >= 1)
  {
    t1 += (time2-time1);
    if(t1 == 58)
    {
      t2 += 1;
      t1 = 48;
    }
    if(t2 == 58)
    {
      t3 += 1;
      t2 = 48;
    }
  }
  

	if(t1 == 52 && t2 == 48 && t3 == 48 && block != 1)
	{
		alSourceStop(source[1]);
		alSourcePlay(source[0]);
		block = 1;
	}

  glColor3f(1.0,0.84,.0);
    
  glRasterPos2i(500, 269);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'T');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'I');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'M');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 'E');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ':');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ' ');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)t3);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)t2);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , (char)t1);
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ' ');
  glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , 's');
  
  time1 = time2;
      
/////////----- CAR --- COMPUTER-----////////////////
  if(flag == 1)
  {
    j=0;
    for(i = 0 ; i<500 ; i++)
    {
      glLineWidth(2);
      glColor3f(0.0,0.5,1.0);
      f = vertex[j];
      ff = vertex[j+1];
      
      ///////-------EAT FUEL------//////////
      
      if((f+10 <= 160 && f+160>=10) && ff-100==pcdata.yPosition && ff>=100)
      {
	if(!(fbdata.xLeft1+7 >470) && !(fbdata.xRight1+7 > 470))
	{
	  //fbdata.xLeft1 = fbdata.xLeft1+7;
	  //fbdata.xLeft2 = fbdata.xLeft2+7;
	  fbdata.xRight1 = fbdata.xRight1+7;
	  fbdata.xRight2 = fbdata.xRight2+7 ;
	  score2 += 4; 
	  if(score2 >= 57)
	  {
		score3 += 1;
		score2 = 48;
	  }
	  if(score3 == 57)
	  {
		score4 += 1;
		score3 = 48;
	  }
	  sycle = 9;
	}
	else
	{
	  //fbdata.xLeft1 = 470;
	  //fbdata.xLeft2 = 470;
	  fbdata.xRight1 = 470;
	  fbdata.xRight2 = 470 ;
	}
      }
      ////// ----- accident ----//////
      else if((f+10 <= 160 && f+160>=10) && ff==pcdata.yPosition)
      {
		alSourceStop(source[angryBoy]);
		alSourcePlay(source[accident]);
		glBegin(GL_QUADS);
	  glColor3f(1.0,0.0,0.0);

	  glVertex2f(136.0,142.0 + pcdata.yplus);
	  glVertex2f(167.0,142.0 + pcdata.yplus);
	  glVertex2f(167.0,112.0 + pcdata.yplus);//addition
	  glVertex2f(156.0,82.0 + pcdata.yplus);
	  glVertex2f(147.0,82.0 + pcdata.yplus);
	  glVertex2f(150.0,117.0 + pcdata.yplus);
	  glVertex2f(140.0,107.0 + pcdata.yplus);
	  glVertex2f(55.0,103.0 + pcdata.yplus);
	  glVertex2f(55.0,121.0 + pcdata.yplus);
	  glVertex2f(30.0,144.0 + pcdata.yplus);
	  glVertex2f(55.0,144.0 + pcdata.yplus);
	  glVertex2f(55.0,80.0 + pcdata.yplus);
	  glVertex2f(35.0,80.0 + pcdata.yplus);
	  glVertex2f(10.0,144.0 + pcdata.yplus);
	  glVertex2f(10.0,125.0 + pcdata.yplus);
	  glColor3f(0.7,0.5,1.0);
	  glVertex2f(35.0,125.0 + pcdata.yplus);
	  glVertex2f(35.0,144.0 + pcdata.yplus);
	  glVertex2f(32.0,99.0 + pcdata.yplus);
	  glVertex2f(12.0,99.0 + pcdata.yplus);
	  glVertex2f(10.0,80.0 + pcdata.yplus);
	  glVertex2f(39.0,80.0 + pcdata.yplus);     
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	  glVertex2f(55.0,121.0 + pcdata.yplus);
	  glVertex2f(125.0,135.0 + pcdata.yplus);
	  glVertex2f(125.0,117.0 + pcdata.yplus); 

	  glVertex2f(55.0,103.0 + pcdata.yplus);
	  glVertex2f(100.0,89.0 + pcdata.yplus);
	  glVertex2f(125.0,107.0 + pcdata.yplus);
	glEnd();
	
        /////-------Game Over Message----------///////

	alSourceStop(source[0]);
	alSourceStop(source[1]);
	alSourcePlay(source[4]);

	glColor3f(1.0, 0.0, 1.0);
        
	glLineWidth(5);
	glColor3f(1.0,0.0,1.0);
	glRasterPos2i(335, 185);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'G');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'a');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'm');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'e');
	//glRasterPos2i(355, 130);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ' ');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'O');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'v');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'e');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'r');
	
	
	///----> show the frame message after game over
	glColor3f(.0,0.0,.0);
      
	glRasterPos2i(15, 25);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'F');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'R');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'A');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'M');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'E');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'S');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , ':');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ' ');
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , (char)tt2);
	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , (char)tt1);

	glutSwapBuffers();
	
	sleep(5);
      	exit(0);
      }
      
      f = f-ccdata.speed;
      vertex[j] = f;
      j=j+2;
      /////-----FUEL SHOW-----///////////
      if(ff>=100)
      {
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_QUADS);
		  glColor3f(1.0,1.0,0.0);
		  glVertex2f(50.0+f,100+ff-100.0);
		  glVertex2f(90.0+f,100.0+ff-100.0);
		  glVertex2f(90.0+f,87.0+ff-100.0);
		  glVertex2f(50.0+f,87.0+ff-100.0);
		glEnd();
      }
      else
      {
	glBegin(GL_QUADS);
	  glColor3f(0.0,0.5,1.0);
	  glVertex2f(23.0+f,142.0+ff);
	  glVertex2f(10.0+f,142.0+ff);
	  glVertex2f(10.0+f,82.0+ff);
	  glVertex2f(23.0+f,92.0+ff);
	  glVertex2f(20.0+f,117.0+ff);
	  glVertex2f(20.0+f,107.0+ff);
	  glVertex2f(115.0+f,103.0+ff);
	  glVertex2f(115.0+f,121.0+ff);
	  glVertex2f(135.0+f,144.0+ff);
	  glVertex2f(115.0+f,144.0+ff);
	  glVertex2f(115.0+f,80.0+ff);
	  glVertex2f(135.0+f,80.0+ff);
	  glVertex2f(160.0+f,144.0+ff);
	  glVertex2f(160.0+f,125.0+ff);
	  glVertex2f(135.0+f,125.0+ff);
	  glVertex2f(135.0+f,144.0+ff);
	  glVertex2f(135.0+f,99.0+ff);
	  glVertex2f(160.0+f,99.0+ff);
	  glVertex2f(160.0+f,80.0+ff);
	  glVertex2f(135.0+f,80.0+ff);     
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	  glVertex2f(115.0+f,121.0+ff);
	  glVertex2f(45.0+f,135.0+ff);
	  glVertex2f(45.0+f,117.0+ff); 

	  glVertex2f(115.0+f,103.0+ff);
	  glVertex2f(45.0+f,89.0+ff);
	  glVertex2f(45.0+f,107.0+ff);
	glEnd();
      }
    } 
  }
  if(flag == 0)
  {
    j=0;
    for(i = 0 ; i<500 ; i++)
    {
      glLineWidth(2);
      glColor3f(1.0,0.5,1.0);
      
      while(ticket1 != 1);
      f = ccdata.xx;
      ff = ccdata.yy;
      if(ccdata.fuelFlag == 1)
      {
	ff = ff+100;
	ccdata.fuelFlag = 0;
      }
      ticket1 = 0;
      sem_wait(&sem1);
      vertex[j]=f;
      vertex[j+1] = ff;
      sem_post(&sem1);
      j=j+2;
    } 
  }
  if(flag == 0)
    flag = 1;
  
  Timer = Timer+0.5;
  if(Timer == 50)
  {
    Timer = 0;
    ccdata.speed=ccdata.speed+0.3;
    fbdata.decSpeed = fbdata.decSpeed + .03;
  }
////////-------MASTER MESSAGE ------///////////////
  if(ccdata.speed >= 12 && ccdata.firsttime>3)
  {
    glColor3f(1.0,0.0,1.0);
	glRasterPos2i(350, 155);
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'M');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'a');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 's');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 't');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'e');
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'r');
	
	ccdata.firsttime++ ;
	glutSwapBuffers();
  }

  /////////-------- SHOW FRAME COUNT ----------/////////////
  
  if((t1 == 50 && t2 == 48))
  {
      fcounter ++;
    
    if(fcounter < 10)
    {
      tt2 = 48;
      tt1 = fcounter;
    }
    else if(fcounter >=10 && fcounter <20)
    {
      tt2 = 49;
      tt1 = fcounter %10 + 48;
    }
    else if(fcounter >=20 && fcounter <30)
    {
      tt2 = 50;
      tt1 = fcounter %10 + 48;
    }
    else if(fcounter >=30 && fcounter <40)
    {
      tt2 = 51;
      tt1 = fcounter %10 + 48;
    }
    else if(fcounter >=40 && fcounter <50)
    {
      tt2 = 52;
      tt1 = fcounter %10 + 48;
    }
    else if(fcounter >=50 && fcounter <60)
    {
      tt2 = 53;
      tt1 = fcounter %10 + 48;
    }
    else if(fcounter >=60 && fcounter <70)
    {
      tt2 = 54;
      tt1 = fcounter %10 + 48;
    }
    else if(fcounter >=70 && fcounter <80)
    {
      tt2 = 55;
      tt1 = fcounter %10 + 48;
    }
    }
    glColor3f(.0,0.0,.0);
      
    glRasterPos2i(15, 25);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'F');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'R');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'A');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'M');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'E');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , 'S');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , ':');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , ' ');
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , (char)tt2);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12 , (char)tt1);
  
  
  glutSwapBuffers();    
	
	if(block2 == 0)
	{
		usleep(1300000);
		block2 = 1;
	}
  glutPostRedisplay();

}

int pauseKeyLock = 0;
int exitWhileFlag = 1;

void mypause()
{

  glClearColor(0.545,0.545,.513,1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0,400.0,0.0,220.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    glColor3f(1.0,0.98,.803);
    
    glRasterPos2i(70, 95);
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'P');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'U');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'S');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'E');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '.');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '.');
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '.');
    
    glutSwapBuffers(); 
    
   // while(exitWhileFlag);
    sleep(3);
//    char resume;
//    scanf("%c",&resume);
    
    pauseKeyLock = 0;
    glutDestroyWindow(pauseWindow);
   // glexit();
}

void keyPressed (unsigned char key, int x, int y) 
{
  if (key == 'p') 
  {
    
  }
}

void keyUp (unsigned char key, int x, int y) 
{
  if (key == 'p') 
  {
    /*
    glutInitWindowPosition(300,200);
    glutInitWindowSize(200,100);
    glutCreateWindow("Second");
    
    glutDisplayFunc(mypause);
    
    sleep(2);
  */
    if(pauseKeyLock == 0)
    {
      pauseKeyLock = 1;
      pauseWindow = glutCreateSubWindow (mainWindow,300,100,150,100);
      glutDisplayFunc(mypause);
    }
  }
  if (key == 'r')
  {
    if (pauseKeyLock == 1)
      pauseKeyLock = 0;
      glutDestroyWindow(pauseWindow);
  }
  
}

void keySpecialUp (int key, int x, int y) 
{
  if (key == GLUT_KEY_UP) 
  {
    pcdata.yplus = 89;
    pcdata.yPosition = 89;
	glutPostRedisplay() ;
  }
  if (key == GLUT_KEY_DOWN) 
  {
    pcdata.yplus = 0;
    pcdata.yPosition = 0;
	glutPostRedisplay() ;
  }
  
}

int main(int argc , char** argv)
{
////----------------------------------
  ccdata.fuelFlag=0;
  ccdata.xx = 0;
  ccdata.x = 1100;
  ccdata.y=89;
  ccdata.yy = 0;
  ccdata.now = 0;
  ccdata.preview = 0;
  ccdata.step = 400;
  ccdata.distance = 40;
  ccdata.speed = 9.00;
  ccdata.firsttime = 1;
////----------------------------------
  pcdata.yplus = 0;
  pcdata.yPosition = 0;
  pcdata.gameOver = 0;
////----------------------------------
  fbdata.xLeft1 = 1;
  fbdata.xLeft2 = 1;
  fbdata.xRight1 = 470;
  fbdata.xRight2 = 470;
  fbdata.decSpeed = .2;
////---------------------------------
  time1 = time(0);
  t1 = t2 = t3 = 48;
////----------------------------------
  tt1 = tt2 = 48;
  
  sem_init(&sem1 , 0 , 1);
  pthread_t thread1,thread2;
  
  ticket1 = 0;
  flag = 0;
  
  fcounter = 0;
  pthread_create(&thread1 , 0 ,(void*) &moveComputerCar , 0);
  
  glutInit(&argc , argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(300,200);
  glutInitWindowSize(800,300);

  alutInit(&argc, argv);

  initAL();
  
  
  mainWindow = glutCreateWindow("FormulaTwo (ver 2.0.0) --------- Designed by: Behnam Mohammad Karimi");
  
  init();
  
  glutDisplayFunc(road);

	alSourcePlay(source[3]);
	alSourcePlay(source[1]);
  
  //glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyUp);
  glutSpecialUpFunc(keySpecialUp);
  glutMainLoop();
//     pthread_join(thread1 , 0);

  sem_destroy(&sem1);
  return 0;
}
