#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <GL/freeglut.h>

#include <math.h>

int verticalMovement;
int horizontalMovement;

int lastMousex, lastMousey;

float thetaAngle = 0.0f;  // �ngulo horizontal
float phiAngle = 0.0f;  // �ngulo vertical
float camRadius = 2.0f; // dist�ncia da c�mera ao alvo


int init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_PROJECTION);
    glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    float camX = 0.0 + camRadius * cosf(phiAngle) * sinf(thetaAngle);
    float camY = 0.0 + camRadius * sinf(phiAngle);
    float camZ = 0.0 + camRadius * cosf(phiAngle) * cosf(thetaAngle);

    gluLookAt(camX, camY, camZ,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    glColor3f(1.0f, 0.0f, 0.0f);
    glutWireCube(1.0f);

    glutSwapBuffers();
}

void handleKeyboardInput() {

}

void handleMouseMovement(int x, int y) {
    int dx = x - lastMousex;
    int dy = y - lastMousey;

    float mouseSensitivity = 0.005f;

    thetaAngle += dx * mouseSensitivity;
    phiAngle += dy * mouseSensitivity;

    if (phiAngle > 1.5f) phiAngle = 1.5f;
    if (phiAngle < -1.5f) phiAngle = -1.5f;

    lastMousex = x;
    lastMousey = y;

    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // mudar diretivas de inicializa��o depois pra algo melhor
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(200, 0);
    glutInitWindowSize(500, 500);
    glutCreateWindow("HALLO");

    init();

    glutKeyboardFunc(handleKeyboardInput);
    glutPassiveMotionFunc(handleMouseMovement); // fun��o que detecta movimento do mouse de forma passiva (sem nenhum bot�o pressionado)

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
