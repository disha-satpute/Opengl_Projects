#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <cmath>


float carPosX = 0.0f;
float obstaclePosX = 0.0f; //enemy
float obstaclePosY = 1.0f;  //enemy
float obstacleSpeed = 0.01f;  //enemy
float carSpeed = 0.1f;
int score = 0;
bool gameOver = false;
bool speedBoost = false;


void drawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < num_segments; i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);  // X
        float y = r * sinf(theta);  // Y
        glVertex2f(x + cx, y + cy);  // Output
    }
    glEnd();
}

void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}


void drawCar(float carX, float carY) {

    glColor3f(0.0f, 0.0f, 1.0f);
    drawRectangle(carX - 0.1f, carY, 0.2f, 0.1f);

    glColor3f(0.0f, 0.5f, 1.0f);
    drawRectangle(carX - 0.07f, carY + 0.07f, 0.14f, 0.05f);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawRectangle(carX - 0.06f, carY + 0.08f, 0.05f, 0.03f);
    drawRectangle(carX + 0.01f, carY + 0.08f, 0.05f, 0.03f);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(carX - 0.07f, carY - 0.03f, 0.03f, 20);
    drawCircle(carX + 0.07f, carY - 0.03f, 0.03f, 20);
}


void drawObstacleCar(float carX, float carY) {
    glColor3f(1.0f, 0.0f, 0.0f);
    drawRectangle(carX - 0.1f, carY, 0.2f, 0.1f);

    glColor3f(0.8f, 0.0f, 0.0f);
    drawRectangle(carX - 0.07f, carY + 0.07f, 0.14f, 0.05f);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawRectangle(carX - 0.06f, carY + 0.08f, 0.05f, 0.03f);
    drawRectangle(carX + 0.01f, carY + 0.08f, 0.05f, 0.03f);

    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle(carX - 0.07f, carY - 0.03f, 0.03f, 20);
    drawCircle(carX + 0.07f, carY - 0.03f, 0.03f, 20);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    if (gameOver) {

        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(-0.1f, 0.0f);
        const char* msg = "Game Over!";
        for (int i = 0; msg[i] != '\0'; i++) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, msg[i]);
        }
        glFlush();
        return;
    }

    glColor3f(0.5f, 0.5f, 0.5f);
    drawRectangle(-0.5f, -1.0f, 1.0f, 2.0f);


    drawCar(carPosX, -0.8f);

    drawObstacleCar(obstaclePosX, obstaclePosY);

    if (obstaclePosY <= -0.7f && obstaclePosY >= -0.9f && abs(carPosX - obstaclePosX) < 0.2f) {
        gameOver = true;
    }

    obstaclePosY -= obstacleSpeed;

    if (obstaclePosY < -1.0f) {
        obstaclePosY = 1.0f;
        obstaclePosX = (rand() % 100 / 100.0f) * 0.8f - 0.4f;
        score++;
        obstacleSpeed += 0.001f;
    }

    glFlush();
}


void keyboard(int key, int, int) {
    if (gameOver) return;

    switch (key) {
        case GLUT_KEY_LEFT:
            carPosX -= carSpeed;
            if (carPosX < -0.5f) carPosX = -0.5f;
            break;
        case GLUT_KEY_RIGHT:
            carPosX += carSpeed;
            if (carPosX > 0.5f) carPosX = 0.5f;
            break;
        case GLUT_KEY_UP:
            speedBoost = true;
            break;
        case GLUT_KEY_DOWN:
            speedBoost = false;
            break;
    }

    glutPostRedisplay();}
void update(int value) {
    if (!gameOver) {

        carSpeed = speedBoost ? 0.15f : 0.1f;
        glutPostRedisplay();
        glutTimerFunc(16, update, 0);
    }
}

void initOpenGL() {
    glClearColor(0.0f, 0.7f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}


int main(int argc, char** argv) {
    srand(time(0));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Car Racing Game");

    initOpenGL();

    glutDisplayFunc(display);
    glutSpecialFunc(keyboard);
    glutTimerFunc(0, update, 0);

    glutMainLoop();

    return 0;
}
