#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 80
#define HEIGHT 22

void clearScreen() {
    printf("\033[H\033[J");
}

void renderCube(float angleX, float angleY, float angleZ) {
    char buffer[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH * HEIGHT; i++) buffer[i] = ' ';

    float cubePoints[8][3] = {
        {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
        {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
    };

    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0},
        {4, 5}, {5, 6}, {6, 7}, {7, 4},
        {0, 4}, {1, 5}, {2, 6}, {3, 7}
    };

    float cosX = cos(angleX), sinX = sin(angleX);
    float cosY = cos(angleY), sinY = sin(angleY);
    float cosZ = cos(angleZ), sinZ = sin(angleZ);
    float projectedPoints[8][2];

    for (int i = 0; i < 8; i++) {
        float x = cubePoints[i][0];
        float y = cubePoints[i][1];
        float z = cubePoints[i][2];

        float nx = cosY * x + sinY * z;
        float nz = -sinY * x + cosY * z;
        float ny = cosX * y - sinX * nz;
        nz = sinX * y + cosX * nz;
        x = cosZ * nx - sinZ * ny;
        y = sinZ * nx + cosZ * ny;

        projectedPoints[i][0] = WIDTH / 2 + x * WIDTH / 4;
        projectedPoints[i][1] = HEIGHT / 2 - y * HEIGHT / 4;

        int screenX = (int)projectedPoints[i][0];
        int screenY = (int)projectedPoints[i][1];

        if (screenX >= 0 && screenX < WIDTH && screenY >= 0 && screenY < HEIGHT) {
            buffer[screenY * WIDTH + screenX] = 'X';
        }
    }

    for (int i = 0; i < 12; i++) {
        int p1 = edges[i][0];
        int p2 = edges[i][1];

        int x1 = (int)projectedPoints[p1][0];
        int y1 = (int)projectedPoints[p1][1];
        int x2 = (int)projectedPoints[p2][0];
        int y2 = (int)projectedPoints[p2][1];

        int dx = abs(x2 - x1), dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (1) {
            if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
                buffer[y1 * WIDTH + x1] = 'X';
            }
            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x1 += sx; }
            if (e2 < dx) { err += dx; y1 += sy; }
        }
    }

    clearScreen();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            putchar(buffer[i * WIDTH + j]);
        }
        putchar('\n');
    }
}

int main() {
    srand(time(0));
    float angleX = rand() / (float)RAND_MAX * 2 * M_PI;
    float angleY = rand() / (float)RAND_MAX * 2 * M_PI;
    float angleZ = rand() / (float)RAND_MAX * 2 * M_PI;
    float speedX = 0.03 + rand() / (float)RAND_MAX * 0.03;
    float speedY = 0.03 + rand() / (float)RAND_MAX * 0.03;
    float speedZ = 0.03 + rand() / (float)RAND_MAX * 0.03;

    while (1) {
        renderCube(angleX, angleY, angleZ);
        angleX += speedX;
        angleY += speedY;
        angleZ += speedZ;
        usleep(35000);
    }
    return 0;
}
