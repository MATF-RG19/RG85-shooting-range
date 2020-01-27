#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>

GLfloat light_position[] = { 0, 2000, 2000, 0 };

GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };

GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1 };

GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1 };

GLfloat ambient_coeffs[] = { 0.7, 0, 0, 1 };

GLfloat diffuse_coeffs[] = { 1, 0, 0, 1 };

GLfloat specular_coeffs[] = { 0.5, 0, 0, 1 };

GLfloat shininess = 30;

void* currentFont = GLUT_BITMAP_TIMES_ROMAN_24;

static int timer_active, timer_active_target;
static float timer_variable = 0;
static int bullet_fired = 0;
static int status = 1;
static int menu_mode = 1;
static int target_mode = 1;
static int lower, upper;
static int bullets_fired = 0;
static int bullets_hit = 0;
static char score[2];
static char final_score[2];

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_timer_target(int value);
static void on_display(void);
static void fire_bullet();
static void draw_string(float x, float y, float z, char* string);
static void random_target_position();

static float corV1x = 500, corV1y = 260;
static float corV2x = 500, corV2y = 240;
static float corH1x = 490, corH1y = 250;
static float corH2x = 510, corH2y = 250;

static float bulletX = 500, bulletY = 100, bulletZ = 805;
static float bulletMoveX = 0, bulletMoveY = 0, bulletMoveZ = 0;
static float aimX, aimY, aimZ = 805;
static float targetX = 500, targetY = 500, targetZ = 0;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    timer_active = 0;
    timer_active_target = 0;
    glLineWidth(2);
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

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
      if(menu_mode == 0)
        if(!bullet_fired){
          aimX = 500 - corV1x;
          aimY = 100 - corH1y;
          fire_bullet();
        }
      break;

   case 'G':
   case 'g':
      menu_mode = 0;

      random_target_position();

      if(!timer_active_target)
        timer_active_target = 1;
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
      if(bulletX <= (targetX + 40) && bulletX >= (targetX - 40))
        if(bulletY <= (targetY + 40) && bulletY >= (targetY - 40)){
          bullets_hit++;
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
          random_target_position();
          glutPostRedisplay();
          return;
        }

    if(bulletZ < -50){
      if(bullets_fired == 20)
        menu_mode = 2;

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

    timer_variable += 4;

    glutPostRedisplay();

    if (timer_active)
        glutTimerFunc(20, on_timer, 0);
}

static void on_timer_target(int value){
    if(value != 1)
      return;

    if(target_mode == 1){
      if(targetY >= 470)
        status = -1;

      if(targetY <= 75)
        status = 1;

      if(status > 0)
        targetY += 2;

      else
        targetY -= 2;
    }

    if(target_mode == 2){
      if(targetX >= 970)
        status = -1;

      if(targetX <= 75)
        status = 1;

      if(status > 0)
        targetX += 2;

      else
        targetX -= 2;
    }

    glutPostRedisplay();

    if(timer_active_target)
      glutTimerFunc(10, on_timer_target, 1);
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

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* MENU MODE */
    if(menu_mode == 1){
      /* FLOOR */
      ambient_coeffs[0] = 0.1;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0;

      diffuse_coeffs[0] = 0.1;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glPushMatrix();
        glTranslatef(500, 0, 500);
        glScalef(1, 0.1, 1);
        glutSolidCube(1000);
      glPopMatrix();

      /* 3 WALLS */
      ambient_coeffs[0] = 0;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0.1;

      diffuse_coeffs[0] = 0;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0.2;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0.3;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glPushMatrix();
        glTranslatef(500, 0, -50);
        glScalef(1, 1, 0.1);
        glutSolidCube(1000);
      glPopMatrix();

      ambient_coeffs[0] = 0;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0.3;

      diffuse_coeffs[0] = 0;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0.3;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0.3;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glPushMatrix();
        glTranslatef(-50, 0, 500);
        glScalef(0.1, 1, 1);
        glutSolidCube(1000);
      glPopMatrix();

      glPushMatrix();
        glTranslatef(1050, 0, 500);
        glScalef(0.1, 1, 1);
        glutSolidCube(1000);
      glPopMatrix();

      /* MENU TEXT */
      draw_string(480, 135, 700, "Shooting Range");
      draw_string(480, 131, 700, "Press G to start the game");
      draw_string(480, 127, 700, "Use W A S D to move the crosshair");
      draw_string(480, 123, 700, "Use P to shoot");
      draw_string(480, 119, 700, "You have 20 bullets, good luck!");

      glutSwapBuffers();
    }

    /* NON MENU MODE */
    else if(menu_mode == 0){
      /* FLOOR */
      ambient_coeffs[0] = 0.1;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0;

      diffuse_coeffs[0] = 0.1;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glPushMatrix();
        glTranslatef(500, 0, 500);
        glScalef(1, 0.1, 1);
        glutSolidCube(1000);
      glPopMatrix();

      /* 3 WALLS */
      ambient_coeffs[0] = 0;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0.1;

      diffuse_coeffs[0] = 0;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0.2;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0.3;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glPushMatrix();
        glTranslatef(500, 0, -50);
        glScalef(1, 1, 0.1);
        glutSolidCube(1000);
      glPopMatrix();

      ambient_coeffs[0] = 0;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0.3;

      diffuse_coeffs[0] = 0;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0.3;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0.3;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glPushMatrix();
        glTranslatef(-50, 0, 500);
        glScalef(0.1, 1, 1);
        glutSolidCube(1000);
      glPopMatrix();

      glPushMatrix();
        glTranslatef(1050, 0, 500);
        glScalef(0.1, 1, 1);
        glutSolidCube(1000);
      glPopMatrix();

      /* CORSAIR */

      ambient_coeffs[0] = 0;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0;

      diffuse_coeffs[0] = 0;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

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

      ambient_coeffs[0] = 0;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0;

      diffuse_coeffs[0] = 0;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

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

      ambient_coeffs[0] = 0;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0;

      diffuse_coeffs[0] = 0.1;
      diffuse_coeffs[1] = 0.1;
      diffuse_coeffs[2] = 0;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glColor3f(1, 0, 1);

      glPushMatrix();
        glTranslatef(targetX, targetY, targetZ);
        glScalef(1, 1, 0.01);
        glutSolidCube(60);
      glPopMatrix();

      sprintf(score, "%d", bullets_hit);
      draw_string(380, 190, 600, "Current score:");
      draw_string(422, 190, 600, score);

      glutSwapBuffers();
    }

    /* GAME END MODE */
    else if(menu_mode == 2){
      ambient_coeffs[0] = 0.1;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0;

      diffuse_coeffs[0] = 0.1;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glPushMatrix();
        glTranslatef(500, 0, 500);
        glScalef(1, 0.1, 1);
        glutSolidCube(1000);
      glPopMatrix();

      /* 3 WALLS */
      ambient_coeffs[0] = 0;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0.1;

      diffuse_coeffs[0] = 0;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0.2;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0.3;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glPushMatrix();
        glTranslatef(500, 0, -50);
        glScalef(1, 1, 0.1);
        glutSolidCube(1000);
      glPopMatrix();

      ambient_coeffs[0] = 0;
      ambient_coeffs[1] = 0;
      ambient_coeffs[2] = 0.3;

      diffuse_coeffs[0] = 0;
      diffuse_coeffs[1] = 0;
      diffuse_coeffs[2] = 0.3;

      specular_coeffs[0] = 0;
      specular_coeffs[1] = 0;
      specular_coeffs[2] = 0.3;

      glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
      glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
      glMaterialf(GL_FRONT, GL_SHININESS, shininess);

      glPushMatrix();
        glTranslatef(-50, 0, 500);
        glScalef(0.1, 1, 1);
        glutSolidCube(1000);
      glPopMatrix();

      glPushMatrix();
        glTranslatef(1050, 0, 500);
        glScalef(0.1, 1, 1);
        glutSolidCube(1000);
      glPopMatrix();

      /* MENU TEXT */
      sprintf(final_score, "%d", bullets_hit);
      draw_string(490, 130, 700, "GAME OVER");
      draw_string(490, 126, 700, "Final Score: ");
      draw_string(510, 126, 700, final_score);

      glutSwapBuffers();
    }
}

static void fire_bullet(){
  bullets_fired++;
  printf("%d\n", bullets_fired);
  bullet_fired = 1;
  timer_active = 1;
  glutTimerFunc(50, on_timer, 0);
}

void draw_string(float x, float y, float z, char *string) {
  glRasterPos3f(x, y, z);

  char *c = string;
    while(*c != '\0'){
    glutBitmapCharacter(currentFont, *c);
    c++;
  }
}

void random_target_position(){

  upper = 2;
  lower = 1;
  target_mode = (rand() %
      (upper - lower + 1)) + lower;

  if(target_mode == 1){
    targetY = 500;
    upper = 900;
    lower = 100;
    targetX = (rand() %
        (upper - lower + 1)) + lower;

  }

  else{
    targetX = 500;
    upper = 470;
    lower = 75;
    targetY = (rand() %
        (upper - lower + 1)) + lower;

  }
}
