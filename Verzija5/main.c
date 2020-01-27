#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

static int timer_active, timer_active_target;
static float timer_variable = 0, timer_variable_target = 0;
static int bullet_fired = 0;
static int status = 1;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_timer_target(int value);
static void on_display(void);
static void fire_bullet();
static void draw_axis();

static float corV1x = 500, corV1y = 260;
static float corV2x = 500, corV2y = 240;
static float corH1x = 490, corH1y = 250;
static float corH2x = 510, corH2y = 250;

static float bulletX = 500, bulletY = 100, bulletZ = 805;
static float bulletMoveX = 0, bulletMoveY = 0, bulletMoveZ = 0;
static float aimX, aimY, aimZ = 805;
static float targetX = 500, targetY = 100, targetZ = 0;
static float targetMoveX = 0, targetMoveY = 0, targetMoveZ = 0;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    timer_active = 0;
    timer_active_target = 1;

    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);

    glutMainLoop();

    return 0;
}

static void on_keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;

    case 'A':
    case 'a':
      corV1x -= 8;
      corV2x -= 8;
      corH1x -= 8;
      corH2x -= 8;
      glutPostRedisplay();
      break;

    case 'W':
    case 'w':
      corV1y += 8;
      corV2y += 8;
      corH1y += 8;
      corH2y += 8;
      glutPostRedisplay();
      break;

    case 'D':
    case 'd':
      corV1x += 8;
      corV2x += 8;
      corH1x += 8;
      corH2x += 8;
      glutPostRedisplay();
      break;

    case 'S':
    case 's':
      corV1y -= 8;
      corV2y -= 8;
      corH1y -= 8;
      corH2y -= 8;
      glutPostRedisplay();
      break;

    case 'P':
    case 'p':
      if(!bullet_fired){
        aimX = 500 - corV1x;
        aimY = 100 - corH1y;
        fire_bullet();
      }
      break;

   case 'G':
   case 'g':
      glutTimerFunc(50, on_timer_target, 1);
      break;
    }
}

static void on_timer(int value)
{
    if (value != 0)
        return;

    bulletX -= 3.0/100 * aimX;
    bulletY -= 3.0/100 * aimY;
    bulletZ -= 3.0/100 * aimZ;

    if(bulletZ <= targetZ)
      if(bulletX <= (targetX + 30) && bulletX >= (targetX - 30))
        if(bulletY <= (targetY + 30) && bulletY >= (targetY - 30)){
          timer_variable = 0;
          timer_active = 0;
          bulletMoveX = 0;
          bulletMoveY = 0;
          bulletMoveZ = 0;
          bulletX = 500;
          bulletY = 100;
          bulletZ = 805;
          bullet_fired = 0;
          printf("POGODAK!\n");
          glutPostRedisplay();
          return;
        }

    if(bulletMoveZ > 850){
      timer_variable = 0;
      timer_active = 0;
      bulletMoveX = 0;
      bulletMoveY = 0;
      bulletMoveZ = 0;
      bulletX = 500;
      bulletY = 100;
      bulletZ = 805;
      bullet_fired = 0;
      glutPostRedisplay();
      return;
    }

    timer_variable += 2;

    glutPostRedisplay();

    if (timer_active)
        glutTimerFunc(30, on_timer, 0);
}

static void on_timer_target(int value){
    if(value != 1)
      return;

    if(targetY >= 470)
      status = -1;

    if(targetY <= 30)
      status = 1;

    if(status > 0)
      targetY += 4;

    else
      targetY -= 4;

    glutPostRedisplay();

    if(timer_active_target)
      glutTimerFunc(30, on_timer_target, 1);
}

static void on_reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 1500);
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(500, 100, 800, 500, 100, 0, 0, 1, 0);


    /* FLOOR */
    glPushMatrix();
      glTranslatef(500, 0, 500);
      glScalef(1, 0.1, 1);
      glColor3f(1, 0, 0);
      glutWireCube(1000);
    glPopMatrix();

    /* 3 WALLS */
    glPushMatrix();
      glTranslatef(500, 0, -50);
      glScalef(1, 1, 0.1);
      glColor3f(1, 1, 0);
      glutWireCube(1000);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-50, 0, 500);
      glScalef(0.1, 1, 1);
      glColor3f(1, 1, 0);
      glutWireCube(1000);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(1050, 0, 500);
      glScalef(0.1, 1, 1);
      glColor3f(1, 1, 0);
      glutWireCube(1000);
    glPopMatrix();

    /* CORSAIR */
    glColor3f(0 ,1 , 1);
    glPushMatrix();
      glBegin(GL_LINES);
        glVertex3f(corV1x, corV1y, 4);
        glVertex3f(corV2x, corV2y, 4);
      glEnd();

      glBegin(GL_LINES);
        glVertex3f(corH1x, corH1y, 4);
        glVertex3f(corH2x, corH2y, 4);
      glEnd();
    glPopMatrix();

    /* BULLET */
    glColor3f(1, 1, 1);
    bulletMoveX = timer_variable/100 * aimX;
    bulletMoveY = timer_variable/100 * aimY;
    bulletMoveZ = timer_variable/100 * aimZ;

    glPushMatrix();
      glTranslatef(-bulletMoveX, -bulletMoveY, -bulletMoveZ);
      glTranslatef(500, 100, 805);
      glutSolidSphere(3, 100, 100);
    glPopMatrix();

    /* TARGET */
    glColor3f(1, 0, 1);

    glPushMatrix();
      glTranslatef(targetX, targetY, targetZ);
      glScalef(1, 1, 0.01);
      glutSolidCube(60);
    glPopMatrix();

    glutSwapBuffers();
}

static void fire_bullet(){
  bullet_fired = 1;
  timer_active = 1;
  glutTimerFunc(50, on_timer, 0);
}

static void draw_axis(){
  glColor3f(1, 0, 0);
  glBegin(GL_LINES);
    glVertex3f(5000, 0, 0);
    glVertex3f(0, 0, 0);
  glEnd();

  glColor3f(0, 1, 0);
  glBegin(GL_LINES);
    glVertex3f(0, 5000, 0);
    glVertex3f(0, 0, 0);
  glEnd();

glColor3f(0, 0, 1);
  glBegin(GL_LINES);
    glVertex3f(0, 0, 5000);
    glVertex3f(0, 0, 0);
  glEnd();
}
