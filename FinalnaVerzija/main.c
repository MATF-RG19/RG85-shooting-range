#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <GL/glut.h>
#include "image.h"

#define FILENAME "plate.bmp"
#define FILENAME1 "wall.bmp"
#define FILENAME2 "horizons.bmp"
#define FILENAME3 "wood.bmp"
#define FILENAME4 "target.bmp"
#define FILENAME5 "bricks2.bmp"

GLfloat light_position[] = { 0, 2000, 2000, 0 };

GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1 };

GLfloat light_diffuse[] = { 0.6, 0.6, 0.6, 1 };

GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1 };

GLfloat ambient_coeffs[] = { 0.7, 0, 0, 1 };

GLfloat diffuse_coeffs[] = { 1, 0, 0, 1 };

GLfloat specular_coeffs[] = { 0.5, 0, 0, 1 };

GLfloat shininess = 30;

void* currentFont = GLUT_BITMAP_TIMES_ROMAN_24;

static GLuint names[6];

static int timer_active, timer_active_target;
static float timer_variable = 0;
static int bullet_fired = 0;
static int status = 1;
static int menu_mode = 1;
static int target_mode = 1;
static int lower, upper;
static int pillarX, pillarY, pillarZ = 300;
static int bullets_fired = 0;
static int bullets_hit = 0;
static char score[2];
static char final_score[2];

static void on_keyboard(unsigned char key, int x, int y);
static void on_mouse(int button, int state, int x, int y);
static void on_motion(int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_timer_target(int value);
static void on_display(void);
static void draw_range();
static void draw_pillar();
static void fire_bullet();
static void draw_string(float x, float y, float z, char* string);
static void random_target_position();
static void initialize_game();

static float corV1x = 500, corV1y = 260;
static float corV2x = 500, corV2y = 240;
static float corH1x = 490, corH1y = 250;
static float corH2x = 510, corH2y = 250;

static float bulletX = 500, bulletY = 100, bulletZ = 805;
static float bulletMoveX = 0, bulletMoveY = 0, bulletMoveZ = 0;
static float aimX, aimY, aimZ = 805;
static float targetX = 500, targetY = 500, targetZ = 2;

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

    glutInitWindowSize(1200, 800);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glutKeyboardFunc(on_keyboard);
    glutMouseFunc(on_mouse);
    glutPassiveMotionFunc(on_motion);
    glutReshapeFunc(on_reshape);
    glutDisplayFunc(on_display);

    timer_active = 0;
    timer_active_target = 0;
    glLineWidth(2);
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);

    glTexEnvf(GL_TEXTURE_ENV,
              GL_TEXTURE_ENV_MODE,
              GL_REPLACE);

    Image* image;

    image = image_init(0, 0);

    image_read(image, FILENAME);

    glGenTextures(6, names);

    glBindTexture(GL_TEXTURE_2D, names[0]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	image_read(image, FILENAME1);

	glBindTexture(GL_TEXTURE_2D, names[1]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	image_read(image, FILENAME2);

	glBindTexture(GL_TEXTURE_2D, names[2]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    image_read(image, FILENAME3);

    glBindTexture(GL_TEXTURE_2D, names[3]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                image->width, image->height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    image_read(image, FILENAME4);

    glBindTexture(GL_TEXTURE_2D, names[4]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 image->width, image->height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

    image_read(image, FILENAME5);

    glBindTexture(GL_TEXTURE_2D, names[5]);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,
                    GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                image->width, image->height, 0,
                GL_RGB, GL_UNSIGNED_BYTE, image->pixels);

    glBindTexture(GL_TEXTURE_2D, 0);

	  image_done(image);

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
   	  if(!bullet_fired)
      	initialize_game();

      break;
   }
}

static void initialize_game(){
	menu_mode = 0;
	timer_active_target = 0;
	timer_variable = 0;
	timer_active = 0;
	bulletMoveX = 0;
	bulletMoveY = 0;
	bulletMoveZ = 0;
	bulletX = 500;
	bulletY = 100;
	bulletZ = 805;
	bullet_fired = 0;
	bullets_fired = 0;
	bullets_hit = 0;

	random_target_position();

    glutTimerFunc(200, on_timer_target, 1);
    timer_active_target = 1;

}

static void on_mouse(int button, int state, int x, int y)
{
    if(menu_mode == 0)
        if(!bullet_fired){
          aimX = 500 - corV1x;
          aimY = 100 - corH1y;
          fire_bullet();
        }

}

static void on_motion(int x, int y)
{
    y = 800 - y;

	  corV1x = x - 90;
    corV2x = x - 90;
    corH1x = x + 10 - 90;
    corH2x = x - 10 - 90;

    corV1y = y + 10 - 250;
    corV2y = y - 10 - 250;
    corH1y = y - 250;
    corH2y = y - 250;

    glutPostRedisplay();
}

static void on_timer(int value)
{
    if (value != 0)
        return;

    bulletX -= 3.0/100 * aimX;
    bulletY -= 3.0/100 * aimY;
    bulletZ -= 3.0/100 * aimZ;

    if(bulletZ <= pillarZ){
      if(target_mode == 1){
        if(bulletY >= (pillarY - 30) && bulletY <= (pillarY + 30))
          {
            timer_variable = 0;
            timer_active = 0;
            bulletMoveX = 0;
            bulletMoveY = 0;
            bulletMoveZ = 0;
            bulletX = 500;
            bulletY = 100;
            bulletZ = 805;
            bullet_fired = 0;

            if(bullets_fired > 19){
  		        menu_mode = 2;
  		        timer_active = 0;
  		        timer_active_target = 0;
  		      }

            glutPostRedisplay();
            return;
          }
        }

      else{
        if(bulletX >= (pillarX - 30) && bulletX <= (pillarX + 30)){
          timer_variable = 0;
          timer_active = 0;
          bulletMoveX = 0;
          bulletMoveY = 0;
          bulletMoveZ = 0;
          bulletX = 500;
          bulletY = 100;
          bulletZ = 805;
          bullet_fired = 0;

          if(bullets_fired > 19){
            menu_mode = 2;
            timer_active = 0;
            timer_active_target = 0;
          }

          glutPostRedisplay();
          return;
        }
      }
    }


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

          if(bullets_fired > 19){
		        menu_mode = 2;
		        timer_active = 0;
		        timer_active_target = 0;
		      }

          random_target_position();
          glutPostRedisplay();
          return;
        }

    if(bulletZ < 0){
      if(bullets_fired > 19){
		  menu_mode = 2;
		  timer_active_target = 0;
      }

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

	if (timer_active)
        glutTimerFunc(20, on_timer, 0);

    glutPostRedisplay();
}

static void on_timer_target(int value){
    if(value != 1)
      return;

    if(menu_mode == 2){
    	timer_active_target = 0;
    	return;
   	}

    if(target_mode == 1){
      if(targetY >= 470)
        status = -1;

      if(targetY <= 30)
        status = 1;

      if(status > 0)
        targetY += 2;

      else
        targetY -= 2;
    }

    if(target_mode == 2){
      if(targetX >= 970)
        status = -1;

      if(targetX <= 30)
        status = 1;

      if(status > 0)
        targetX += 2;

      else
        targetX -= 2;
    }

	if(timer_active_target)
      glutTimerFunc(6, on_timer_target, 1);

    glutPostRedisplay();

}

static void on_reshape(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float) width / height, 1, 1500);
}

static void draw_range(){
  /* FLOOR */
  glDisable(GL_LIGHTING);

  /*ambient_coeffs[0] = 0.2;
  ambient_coeffs[1] = 0.2;
  ambient_coeffs[2] = 0.2;

  diffuse_coeffs[0] = 0.2;
  diffuse_coeffs[1] = 0.2;
  diffuse_coeffs[2] = 0.2;

  specular_coeffs[0] = 0;
  specular_coeffs[1] = 0;
  specular_coeffs[2] = 0;

  glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_coeffs);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_coeffs);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular_coeffs);
  glMaterialf(GL_FRONT, GL_SHININESS, shininess);*/

  /* BACKGROUND */

  glBindTexture(GL_TEXTURE_2D, names[2]);
    glBegin(GL_QUADS);
      glNormal3f(0, 0, 1);

      glTexCoord2f(0, 0);
      glVertex3f(-500, 0, -100);

      glTexCoord2f(1, 0);
      glVertex3f(1500, 0, -100);

      glTexCoord2f(1, 1);
      glVertex3f(1500, 1000, -100);

      glTexCoord2f(0, 1);
      glVertex3f(-500, 1000, 0);
    glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);

  /* 3 WALLS */
  glBindTexture(GL_TEXTURE_2D, names[0]);
    glBegin(GL_QUADS);
      glNormal3f(0, 0, 1);

      glTexCoord2f(0, 0);
      glVertex3f(0, 0, 0);

      glTexCoord2f(3, 0);
      glVertex3f(1000, 0, 0);

      glTexCoord2f(3, 3);
      glVertex3f(1000, 500, 0);

      glTexCoord2f(0, 3);
      glVertex3f(0, 500, 0);
    glEnd();

  glBindTexture(GL_TEXTURE_2D, names[1]);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(0, 0, 0);

    glTexCoord2f(20, 0);
    glVertex3f(0, 0, 1000);

    glTexCoord2f(20, 10);
    glVertex3f(0, 500, 1000);

    glTexCoord2f(0, 10);
    glVertex3f(0, 500, 0);
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);

  glBindTexture(GL_TEXTURE_2D, names[1]);
    glBegin(GL_QUADS);
      glNormal3f(0, 0, 1);

      glTexCoord2f(0, 0);
      glVertex3f(1000, 0, 0);

      glTexCoord2f(20, 0);
      glVertex3f(1000, 0, 1000);

      glTexCoord2f(20, 10);
      glVertex3f(1000, 500, 1000);

      glTexCoord2f(0, 10);
      glVertex3f(1000, 500, 0);
    glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);

  /* FLOOR */
  glBindTexture(GL_TEXTURE_2D, names[3]);
    glBegin(GL_QUADS);
      glNormal3f(0, 0, 1);

      glTexCoord2f(0, 0);
      glVertex3f(0, 0, 0);

      glTexCoord2f(3, 0);
      glVertex3f(0, 0, 1000);

      glTexCoord2f(3, 3);
      glVertex3f(1000, 0, 1000);

      glTexCoord2f(0, 3);
      glVertex3f(1000, 0, 0);
    glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);

  glEnable(GL_LIGHTING);
}

static void draw_pillar(){
  if(target_mode == 1){
    glBindTexture(GL_TEXTURE_2D, names[5]);
    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(0, pillarY + 30, pillarZ);

      glTexCoord2f(1, 0);
      glVertex3f(0, pillarY - 30, pillarZ);

      glTexCoord2f(1, 5);
      glVertex3f(1000, pillarY - 30, pillarZ);

      glTexCoord2f(0, 5);
      glVertex3f(1000, pillarY + 30, pillarZ);
    glEnd();


    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(0, pillarY + 30, pillarZ - 60);

      glTexCoord2f(1, 0);
      glVertex3f(0, pillarY + 30, pillarZ);

      glTexCoord2f(1, 5);
      glVertex3f(1000, pillarY + 30, pillarZ);

      glTexCoord2f(0, 5);
      glVertex3f(1000, pillarY + 30, pillarZ - 60);
    glEnd();

    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(0, pillarY - 30, pillarZ - 60);

      glTexCoord2f(1, 0);
      glVertex3f(0, pillarY - 30, pillarZ);

      glTexCoord2f(1, 5);
      glVertex3f(1000, pillarY - 30, pillarZ);

      glTexCoord2f(0, 5);
      glVertex3f(1000, pillarY - 30, pillarZ - 60);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    /* SHADOW */

    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
      glVertex3f(0, 0.1 ,pillarZ - 90);

      glVertex3f(0, 0.1, pillarZ);

      glVertex3f(1000, 0.1, pillarZ);

      glVertex3f(1000, 0.1, pillarZ - 90);
    glEnd();
  }

  else{
    glBindTexture(GL_TEXTURE_2D, names[5]);

    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(pillarX - 30, 0 ,pillarZ);

      glTexCoord2f(1, 0);
      glVertex3f(pillarX + 30, 0, pillarZ);

      glTexCoord2f(1, 3);
      glVertex3f(pillarX + 30, 500, pillarZ);

      glTexCoord2f(0, 3);
      glVertex3f(pillarX - 30, 500, pillarZ);
    glEnd();

    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(pillarX - 30, 0 ,pillarZ - 60);

      glTexCoord2f(1, 0);
      glVertex3f(pillarX - 30, 0, pillarZ);

      glTexCoord2f(1, 3);
      glVertex3f(pillarX - 30, 500, pillarZ);

      glTexCoord2f(0, 3);
      glVertex3f(pillarX - 30, 500, pillarZ - 60);
    glEnd();

    glBegin(GL_QUADS);
      glTexCoord2f(0, 0);
      glVertex3f(pillarX + 30, 0 ,pillarZ);

      glTexCoord2f(1, 0);
      glVertex3f(pillarX + 30, 0, pillarZ - 60);

      glTexCoord2f(1, 3);
      glVertex3f(pillarX + 30, 500, pillarZ - 60);

      glTexCoord2f(0, 3);
      glVertex3f(pillarX + 30, 500, pillarZ);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);

    /* SHADOW */

    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
      glVertex3f(pillarX - 30, 0.1 ,pillarZ - 60);

      glVertex3f(pillarX + 30, 0.1, pillarZ);

      glVertex3f(pillarX - 1000, 0.1, pillarZ + 500);

      glVertex3f(pillarX - 1000, 0.1, pillarZ + 470);
    glEnd();
  }
}

static void on_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(500, 100, 800, 500, 150, 0, 0, 1, 0);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    /* MENU MODE */
    if(menu_mode == 1){
      /* FLOOR */
      draw_range();

      glDisable(GL_LIGHTING);

      glColor3f(0, 1, 0);
      /* MENU TEXT */
      draw_string(480, 135, 700, "Shooting Range");
      draw_string(480, 131, 700, "Press G to start the game");
      draw_string(480, 127, 700, "Use W A S D or mouse to move the crosshair");
      draw_string(480, 123, 700, "Use either click to shoot");
      draw_string(480, 119, 700, "You have 20 bullets, good luck!");

      glEnable(GL_LIGHTING);

      glutSwapBuffers();
    }

    /* NON MENU MODE */
    else if(menu_mode == 0){
      /* RANGE */

      draw_range();

      /* CORSAIR */

      glDisable(GL_LIGHTING);

      glColor3f(0, 1, 0);

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

      glEnable(GL_LIGHTING);

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

      glDisable(GL_LIGHTING);

      glBindTexture(GL_TEXTURE_2D, names[4]);
      glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(targetX - 30, targetY - 30, targetZ);

        glTexCoord2f(1, 0);
        glVertex3f(targetX + 30, targetY - 30, targetZ);

        glTexCoord2f(1, 1);
        glVertex3f(targetX + 30, targetY + 30, targetZ);

        glTexCoord2f(0, 1);
        glVertex3f(targetX - 30, targetY + 30, targetZ);
      glEnd();

      glBindTexture(GL_TEXTURE_2D, 0);

      /* PILLAR */

      draw_pillar();

      /* CURRENT SCORE */

      glColor3f(0, 0, 0);

      sprintf(score, "%d", bullets_hit);
      draw_string(380, 210, 600, "Current score:");
      draw_string(422, 210, 600, score);

      glutSwapBuffers();
    }

    /* GAME END MODE */
    else if(menu_mode == 2){
      draw_range();

      glDisable(GL_LIGHTING);
      glColor3f(1, 1, 1);
      /* MENU TEXT */
      sprintf(final_score, "%d", bullets_hit);
      draw_string(490, 130, 700, "GAME OVER");
      draw_string(490, 126, 700, "Final Score: ");
      draw_string(510, 126, 700, final_score);

      glEnable(GL_LIGHTING);

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

  /* VERTICAL TARGET */
  if(target_mode == 1){
    targetY = 500;
    upper = 970;
    lower = 30;
    targetX = (rand() %
        (upper - lower + 1)) + lower;
  }

  /* HORIZONTAL TARGET */
  else{
    targetX = 500;
    upper = 470;
    lower = 30;
    targetY = (rand() %
        (upper - lower + 1)) + lower;
  }

  /* PILLAR */
  upper = 900;
  lower = 100;
  pillarX = (rand() %
      (upper - lower + 1)) + lower;

  upper = 450;
  lower = 50;
  pillarY = (rand() %
      (upper - lower + 1)) + lower;
}
