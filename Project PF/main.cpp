#include <iostream>
#include "help.h"

using namespace std;

const int MAX_BULLETS = 1000;
bool alienActive[10][5] = { true }; // Initialize all aliens as active

// arrays to store bullet positions
int bulletX[MAX_BULLETS] = { 0 };
int bulletY[MAX_BULLETS] = { 0 };
bool bulletActive[MAX_BULLETS] = { false };

void alien(int x1, int y1) {
    int x2 = x1 + 25;
    int y2 = y1;
    float x3 = x1 + 12.5;
    int y3 = y1 + 25;
    myLine(x1, y1, x2, y2, 50, 255, 0); // top edge
    myLine(x2, y2, x3, y3, 50, 255, 0); // Right edge
    myLine(x3, y3, x1, y1, 50, 255, 0); // Left edge
}

void alienMatrix(int x1, int y1) {
    for (int i = x1; i < (x1 + 500); i += 50) {
        for (int j = y1; j < (y1 + 400); j += 80) {
                alien(i, j);
        }
    }
}

void spaceship(int x1R, int y1R, int x1S, int y1S) {
    myRect(x1R, y1R, x1R + 30, y1R + 50, 61, 154, 164, 61, 154, 164);
    myRect(x1S, y1S, x1S + 10, y1S + 10, 255, 0, 0, 255, 0, 0);
}

void eraseSpaceship(int x1R, int y1R, int x1S, int y1S) {
    myRect(x1R, y1R, x1R + 30, y1R + 50, 12, 12, 12, 12, 12, 12);
    myRect(x1S, y1S, x1S + 10, y1S + 10, 12, 12, 12, 12, 12, 12);
}

void drawGameBox() {

    // Draw the game box boundaries
    myLine(150, 160, 1750, 160, 255, 255, 255); // Top horizontal line
    myLine(150, 160, 150, 850, 255, 255, 255); // Left vertical line
    myLine(1750, 160, 1750, 850, 255, 255, 255); // Right vertical line
    myLine(150, 850, 1750, 850, 255, 255, 255); // Bottom horizontal line
}



void eraseAliens(int x1, int y1) {
    int x2 = x1 + 25;
    int y2 = y1;
    float x3 = x1 + 12.5;
    int y3 = y1 + 25;

    myLine(x1, y1, x2, y2, 12, 12, 12);
    myLine(x2, y2, x3, y3, 12, 12, 12);
    myLine(x3, y3, x1, y1, 12, 12, 12);
}


void moveAliens(float& x1, float& y1, int& prevX, int& prevY, bool& moveDown) {
    static int moveCounter = 0;
    prevX = x1;
    prevY = y1;

    // updating position based on direction and bounds
    if (y1 <= 180) 
        moveDown = true;
    if (y1 >= 480) 
        moveDown = false;

    if (moveDown) {
        y1 += 0.5; // Move down
    }
    else {
        y1 -= 0.5; // Move up
    }

    if (++moveCounter >= 350) {
        if (x1 > 150) {
            x1 -= 30;
        }
        moveCounter = 0;
    }
}

void eraseAlienMatrix(int x1, int y1) {
    for (int i = x1; i < (x1 + 500); i += 50) {
        for (int j = y1; j < (y1 + 400); j += 80) {
            eraseAliens(i, j);
        }
    }
}

void drawBullet(int x, int y) {
    myRect(x, y, x + 5, y + 10, 255, 255, 0, 255, 255, 0); 
}

void eraseBullet(int x, int y) {
    myRect(x, y, x + 5, y + 10, 12, 12, 12, 12, 12, 12); 
}

void updateBullets() {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bulletActive[i]) {
            eraseBullet(bulletX[i], bulletY[i]);
            bulletX[i] += 10;

            if (bulletX[i] > 150 && bulletX[i] < 1750) {
                drawBullet(bulletX[i], bulletY[i]);
            }
            else {
                bulletActive[i] = false;
            }
        }
    }
}

void checkBulletCollision(float alienStartX, float alienStartY) {
    for (int i = 0; i < MAX_BULLETS; ++i) {
        if (bulletActive[i]) {
            for (int x = 0; x < 10; ++x) {
                for (int y = 0; y < 5; ++y) {
                    if (alienActive[x][y]) {
                        int alienX = alienStartX + x * 50;
                        int alienY = alienStartY + y * 80;
                        // if bullet hits the alien
                        if (bulletX[i] > alienX && bulletX[i] < alienX + 30 &&
                            bulletY[i] > alienY && bulletY[i] < alienY + 30) {
                            eraseAliens(alienX, alienY);
                            alienActive[x][y] = false;
                            bulletActive[i] = false; // Deactivate the bullet
                            return;
                        }
                    }
                }
            }
        }
    }
}

int main() {
    int whichKey;

    // Initial position for spaceship
    float initial_x = 300;
    float initial_y = 300;

    // Initial positions for aliens
    float alienStartX = 1250;
    float alienStartY = 200;
    int prevAlienX = alienStartX;
    int prevAlienY = alienStartY;
    bool moveDown = true;

    spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);

    while (true) {
        drawGameBox();
        eraseAlienMatrix(prevAlienX, prevAlienY);
        moveAliens(alienStartX, alienStartY, prevAlienX, prevAlienY, moveDown);
        alienMatrix(alienStartX, alienStartY);

        updateBullets();
        checkBulletCollision(alienStartX, alienStartY);

        // Check for key presses to move the spaceship
        bool keyPressed = isKeyPressed(whichKey);
        if (keyPressed == true) {
            if (whichKey == 1) {
                eraseSpaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                initial_x -= 10;
                spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
            }
            else if (whichKey == 2) {
                if (initial_y - 1 > 160) {
                    eraseSpaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    initial_y -= 10;
                    spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                }
            }
            else if (whichKey == 3) {
                eraseSpaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                initial_x += 10;
                spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
            }
            else if (whichKey == 4) {
                if (initial_y - 1 < 630) {
                    eraseSpaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    initial_y += 10;
                    spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                }
            }
            else if (whichKey == 5) {
                for (int i = 0; i < MAX_BULLETS; ++i) {
                    if (!bulletActive[i]) {
                        bulletX[i] = initial_x + 42; // Bullet starts at spaceship's tip
                        bulletY[i] = initial_y + 21;
                        bulletActive[i] = true;
                        drawBullet(bulletX[i], bulletY[i]);
                        break;
                    }
                }
            }
        }
    }
    return 0;
}
