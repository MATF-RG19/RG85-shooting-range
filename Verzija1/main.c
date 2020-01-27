#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

static int timer_active;

static void on_keyboard(unsigned char key, int x, int y);
static void on_reshape(int width, int height);
static void on_timer(int value);
static void on_display(void);
static void draw_axis();

static float corV1x = 0, corV1y = 260;
static float corV2x = 0, corV2y = 240;
static float corH1x = -10, corH1y = 250;
static float corH2x = 10, corH2y = 250;

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
    gluLookAt(0, 100, 800, 0, 100, 0, 0, 1, 0);

    /* FLOOR */
    glPushMatrix();
      glTranslatef(0, 0, 500);
      glScalef(1, 0.1, 1);
      glColor3f(1, 0, 0);
      glutWireCube(1000);
    glPopMatrix();

    /* 3 WALLS */
    glPushMatrix();
      glTranslatef(0, 0, -50);
      glScalef(1, 1, 0.1);
      glColor3f(1, 1, 0);
      glutWireCube(1000);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-550, 0, 500);
      glScalef(0.1, 1, 1);
      glColor3f(1, 1, 0);
      glutWireCube(1000);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(550, 0, 500);
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
