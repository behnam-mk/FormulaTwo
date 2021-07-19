
struct computerCarData
{
  int fuelFlag;
  GLfloat xx;
  GLfloat x;
  GLfloat y;
  GLfloat yy;
  GLfloat step;
  GLfloat speed;
  GLfloat distance;
  int preview;
  int now;
  short firsttime;
};

struct playerCarData
{
  GLfloat yplus;
  int yPosition;
  int gameOver;
};
struct fuelBarData
{
  GLfloat xLeft1;
  GLfloat xLeft2;
  
  GLfloat xRight1;
  GLfloat xRight2;
  
  GLfloat decSpeed;
};