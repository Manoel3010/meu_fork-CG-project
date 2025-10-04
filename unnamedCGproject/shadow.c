#include "shadow.h"
#include <GL/glut.h>

// ------------------------------------------------------------
// Função: makeShadowMatrix
// Entrada: plano (A,B,C,D) e posição da luz (x,y,z,w)
// Saída: matriz de sombra 4x4 (1D array com 16 elementos)
// ------------------------------------------------------------
void makeShadowMatrix(GLfloat plane[4], GLfloat lightPos[4], GLfloat shadowMat[16]) {
    GLfloat dot = plane[0] * lightPos[0] +
                  plane[1] * lightPos[1] +
                  plane[2] * lightPos[2] +
                  plane[3] * lightPos[3];

    shadowMat[0]  = dot - lightPos[0] * plane[0];
    shadowMat[4]  = 0.0f - lightPos[0] * plane[1];
    shadowMat[8]  = 0.0f - lightPos[0] * plane[2];
    shadowMat[12] = 0.0f - lightPos[0] * plane[3];

    shadowMat[1]  = 0.0f - lightPos[1] * plane[0];
    shadowMat[5]  = dot - lightPos[1] * plane[1];
    shadowMat[9]  = 0.0f - lightPos[1] * plane[2];
    shadowMat[13] = 0.0f - lightPos[1] * plane[3];

    shadowMat[2]  = 0.0f - lightPos[2] * plane[0];
    shadowMat[6]  = 0.0f - lightPos[2] * plane[1];
    shadowMat[10] = dot - lightPos[2] * plane[2];
    shadowMat[14] = 0.0f - lightPos[2] * plane[3];

    shadowMat[3]  = 0.0f - lightPos[3] * plane[0];
    shadowMat[7]  = 0.0f - lightPos[3] * plane[1];
    shadowMat[11] = 0.0f - lightPos[3] * plane[2];
    shadowMat[15] = dot - lightPos[3] * plane[3];
}

// ------------------------------------------------------------
// Função: drawShadow
// Desenha a sombra de um objeto em relação a um plano
// Entrada: plano, luz e função de desenho do objeto
// ------------------------------------------------------------
void drawShadow(GLfloat plane[4], GLfloat lightPos[4], void (*drawFunc)(void)) {
    GLfloat shadowMat[16];
    
    makeShadowMatrix(plane, lightPos, shadowMat);

    glPushMatrix();
    glMultMatrixf(shadowMat);
    // Deixar a sombra preta e sem iluminação
    glDisable(GL_LIGHTING);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

    drawFunc(); // chama a função que desenha o objeto

    glEnable(GL_LIGHTING);
    glPopMatrix();
}