#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>

const float PI = 3.14159f;
float anglesOrbit[9] = {0.0f};
float anglesRotation[9] = {0.0f};
float moonAngles[79];
const int numStars = 100;

float planetColors[10][3] = {
    {1.0f, 0.5f, 0.0f},
    {0.5f, 0.5f, 0.5f},
    {0.8f, 0.5f, 0.2f},
    {0.0f, 0.5f, 1.0f},
    {1.0f, 0.0f, 0.0f},
    {0.9f, 0.7f, 0.2f},
    {0.8f, 0.7f, 0.5f},
    {0.4f, 0.8f, 0.9f},
    {0.0f, 0.0f, 0.5f},
    {0.8f, 0.5f, 0.3f}
};


struct Star {
    float x, y, z, brightness;
} stars[numStars];


void initStars() {
    for (int i = 0; i < numStars; i++) {
        stars[i].x = (float(rand()) / RAND_MAX - 0.5f) * 20.0f;
        stars[i].y = (float(rand()) / RAND_MAX - 0.5f) * 20.0f;
        stars[i].z = -10.0f;
        stars[i].brightness = float(rand()) / RAND_MAX;
    }
}


void drawStars() {
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < numStars; i++) {
        float twinkle = (0.5f + 0.5f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.001 + i)); // Twinkling effect
        glColor3f(twinkle * stars[i].brightness, twinkle * stars[i].brightness, twinkle * stars[i].brightness);
        glVertex3f(stars[i].x, stars[i].y, stars[i].z);
    }
    glEnd();
}


void drawColoredSphere(float radius, float color[3]) {
    glColor3f(color[0], color[1], color[2]);
    glutSolidSphere(radius, 50, 50);
}

void drawOrbit(float radius) {
    glBegin(GL_LINE_LOOP);
    glColor3f(0.7f, 0.7f, 0.7f);
    for (int i = 0; i < 360; i++) {
        float angle = i * PI / 180.0f;
        glVertex3f(cos(angle) * radius, 0.0f, sin(angle) * radius);
    }
    glEnd();
}

// Draw a moon orbiting a planet
void drawMoon(float distance, float size, float angle) {
    glPushMatrix();
    glRotatef(angle, 0.0f, 1.0f, 0.0f);
    glTranslatef(distance, 0.0f, 0.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    glutSolidSphere(size, 20, 20);
    glPopMatrix();
}

void drawPlanet(int index, float distance, float size, int numMoons, float moonDistance, float moonSize) {
    glPushMatrix();
    glRotatef(anglesOrbit[index], 0.0f, 1.0f, 0.0f);
    glTranslatef(distance, 0.0f, 0.0f);
    glRotatef(anglesRotation[index], 0.0f, 1.0f, 0.0f);
    drawColoredSphere(size, planetColors[index + 1]);


    for (int i = 0; i < numMoons; i++) {
        drawMoon(moonDistance, moonSize, moonAngles[i % 79]);
    }

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0f, 2.0f, 15.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);


    drawStars();


    glPushMatrix();
    drawColoredSphere(1.0f, planetColors[0]);
    glPopMatrix();


    drawOrbit(2.0f);
    drawOrbit(3.0f);
    drawOrbit(4.0f);
    drawOrbit(5.0f);
    drawOrbit(6.5f);
    drawOrbit(8.0f);
    drawOrbit(9.5f);
    drawOrbit(11.0f);
    drawOrbit(12.5f);


    drawPlanet(0, 2.0f, 0.1f, 0, 0, 0);
    drawPlanet(1, 3.0f, 0.15f, 0, 0, 0);
    drawPlanet(2, 4.0f, 0.2f, 1, 0.3f, 0.05f);
    drawPlanet(3, 5.0f, 0.15f, 2, 0.25f, 0.04f);
    drawPlanet(4, 6.5f, 0.4f, 79, 0.5f, 0.06f);
    drawPlanet(5, 8.0f, 0.35f, 82, 0.6f, 0.05f);
    drawPlanet(6, 9.5f, 0.3f, 27, 0.3f, 0.04f);
    drawPlanet(7, 11.0f, 0.3f, 14, 0.35f, 0.04f);
    drawPlanet(8, 12.5f, 0.1f, 5, 0.2f, 0.03f);

    glutSwapBuffers();
}

void update(int value) {
    for (int i = 0; i < 9; i++) {
        anglesOrbit[i] += (i + 1) * 0.1f;
        anglesRotation[i] += 2.0f;
        if (anglesOrbit[i] > 360) anglesOrbit[i] -= 360;
        if (anglesRotation[i] > 360) anglesRotation[i] -= 360;
    }

    for (int i = 0; i < 79; i++) {
        moonAngles[i] += 0.5f + (i % 10) * 0.1f;
        if (moonAngles[i] > 360) moonAngles[i] -= 360;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void initRendering() {
    glEnable(GL_DEPTH_TEST);
    glPointSize(2.0f);
    initStars();
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);

    glutCreateWindow("3D Solar System Simulation ");
    initRendering();

    glutDisplayFunc(display);
    glutReshapeFunc(handleResize);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}
