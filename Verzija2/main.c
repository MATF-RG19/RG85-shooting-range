#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

static int timer_active;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);
static void draw_axis();

static float corV1x = 500, corV1y = 260;
static float corV2x = 500, corV2y = 240;
static float corH1x = 490, corH1y = 250;
static float corH2x = 510, corH2y = 250;

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
      corV1x -= 5;
      corV2x -= 5;
      corH1x -= 5;
      corH2x -= 5;
      glutPostRedisplay();
      break;

    case 'W':
    case 'w':
      corV1y += 5;
      corV2y += 5;
      corH1y += 5;
      corH2y += 5;
      glutPostRedisplay();
      break;

    case 'D':
    case 'd':
      corV1x += 5;
      corV2x += 5;
      corH1x += 5;
      corH2x += 5;
      glutPostRedisplay();
      break;

    case 'S':
    case 's':
      corV1y -= 5;
      corV2y -= 5;
      corH1y -= 5;
      corH2y -= 5;
      glutPostRedisplay();
      break;

    }
}

static void on_timer(int value)
{
    if (value != 0)
        return;

    glutPostRedisplay();

    if (timer_active)
        glutTimerFunc(50, on_timer, 0);
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
        glVertex3f(corV1x, corV1y, 0.5);
        glVertex3f(corV2x, corV2y, 0.5);
      glEnd();

      glBegin(GL_LINES);
        glVertex3f(corH1x, corH1y, 0.5);
        glVertex3f(corH2x, corH2y, 0.5);
      glEnd();
    glPopMatrix();


    glutSwapBuffers();
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
