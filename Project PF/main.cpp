#include <iostream>
#include "help.h"
#include <cstdlib> 
#include <string>

using namespace std;

const int MAX_BULLETS = 1000;
bool alienActive[5][10] = { true }; // Initialize all aliens as active

// arrays to store bullet positions
int bulletX[MAX_BULLETS] = { 0 };
int bulletY[MAX_BULLETS] = { 0 };
bool bulletActive[MAX_BULLETS] = { true };
int prev_score = 0;
int score = 0;
int heartStatus = 3;

void alien(int x1, int y1) {
    int x2 = x1 + 25;
    int y2 = y1;
    float x3 = x1 + 12.5;
    int y3 = y1 + 25;
    myLine(x1, y1, x2, y2, 50, 255, 0); // top edge
    myLine(x2, y2, x3, y3, 50, 255, 0); // Right edge
    myLine(x3, y3, x1, y1, 50, 255, 0); // Left edge
}

void drawAliens(int x1, int y1) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (alienActive[i][j]) {
                int x = x1 + j * 50;
                int y = y1 + i * 80;
                alien(x, y);
            }
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

void drawHeart(int x1, int y1, int x2, int y2, int R, int G, int B) {

    myLine(x1, y1, x2, y2, R, G, B); // top-left horizontal line - up
    myLine(x1 + 10, y1 - 10, x2 + 10, y2 + 10, R, G, B); // top-left horizontal line - down
    myLine(x1 + 20, y1, x2 + 20, y2, R, G, B); // top-right horizontal line - up
    myLine(x1 + 30, y1 - 10, x2 + 30, y2 + 10, R, G, B); // top-right horizontal line - down
    myLine(x1, y1, x2 + 10, y2 + 30, R, G, B); // bottom-left horizontal line - down
    myLine(x1 + 20, y1 + 20, x2 + 30, y2 + 10, R, G, B); // bottom-right horizontal line - up

}

void drawGameBox(bool ifScore, int heart) {

    // title 
    drawText(50, 600, 50, 255, 255, 255, "Space Explorers");
    drawText(20, 150, 50, 255, 255, 255, "High Score: ");
    if (ifScore) {
        string scoreText = "Score: " + to_string(score);
        string pscoreText = "Score: " + to_string(prev_score);
        drawText(20, 150, 100, 0, 0, 0, pscoreText.c_str());
        drawText(20, 150, 100, 255, 255, 255, scoreText.c_str());
    }
    else 
		drawText(20, 150, 100, 255, 255, 255, "Score: 0");
    
    drawText(15, 70, 1000, 255, 255, 255, "Press 'esc' to Pause the Game ");
    
    if (heart == 3) {
        drawHeart(1500, 120, 1510, 110, 255, 0, 0); // first heart
        drawHeart(1550, 120, 1560, 110, 255, 0, 0); // second heart
    }
    if (heart == 2) {
        drawHeart(1500, 120, 1510, 110, 255, 0, 0); // first heart       
    }    
    if (heart == 0) {
        drawHeart(1500, 120, 1510, 110, 255, 0, 0); // first heart       
        drawHeart(1550, 120, 1560, 110, 255, 0, 0); // second heart
        drawHeart(1600, 120, 1610, 110, 255, 0, 0); // third heart
    }
    
    // Draw the game box boundaries
    myLine(150, 160, 1750, 160, 255, 255, 255); // Top horizontal line
    myLine(150, 160, 150, 850, 255, 255, 255); // Left vertical line
    myLine(1750, 160, 1750, 850, 255, 255, 255); // Right vertical line
    myLine(150, 850, 1750, 850, 255, 255, 255); // Bottom horizontal line 
}

int eraseHeart(int num) {
    if (num == 1) {
        drawHeart(1500, 120, 1510, 110, 192, 192, 192); // erase first heart
        return num;
    }
    if (num == 2) {
        drawHeart(1550, 120, 1560, 110, 192, 192, 192); // erase second heart
        return num;
    }
    if (num == 3) {
        drawHeart(1600, 120, 1610, 110, 192, 192, 192); // erase third heart
        return num;
    }
}

void eraseAlien(int x1, int y1) {
    int x2 = x1 + 25;
    int y2 = y1;
    float x3 = x1 + 12.5;
    int y3 = y1 + 25;

    myLine(x1, y1, x2, y2, 12, 12, 12);
    myLine(x2, y2, x3, y3, 12, 12, 12);
    myLine(x3, y3, x1, y1, 12, 12, 12);
}

void moveAliens(float& x1, float& y1, int& prevX, int& prevY, bool& moveDown, bool move) {
    if (move) {
        static int moveCounter = 0;
        prevX = x1;
        prevY = y1;

        // updating position based on direction and bounds
        if (y1 <= 180)
            moveDown = true;
        if (y1 >= 480)
            moveDown = false;

        if (moveDown) {
            y1 += 3.5; // Move down
        }
        else {
            y1 -= 3.5; // Move up
        }

        if (++moveCounter >= 50) {
            if (x1 > 200) {
                x1 -= 100;
            }
            moveCounter = 0;
        }
    }
}

void eraseAliens(int x1, int y1) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (alienActive[i][j]) {
                int x = x1 + j * 50;
                int y = y1 + i * 80;
                eraseAlien(x, y);
            }
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

bool checkIfBulletCollision(int bx, int by, int ax, int ay) {
    return (bx >= ax && bx <= ax + 30) && (by >= ay && by <= ay + 25);
}

bool checkIfSpaceshipCollision(int spaceshipX, int spaceshipY, int alienX, int alienY) {
    // Define the spaceship boundaries
    int spaceshipWidth = 30;
    int spaceshipHeight = 50;

    // Define the alien boundaries
    int alienWidth = 25;
    int alienHeight = 25;

    // Check if the spaceship and alien overlap
    bool collisionX = spaceshipX < alienX + alienWidth && spaceshipX + spaceshipWidth > alienX;
    bool collisionY = spaceshipY < alienY + alienHeight && spaceshipY + spaceshipHeight > alienY;

    return collisionX && collisionY;
}

void startGame(bool score, int heart);

void GameOver(bool playerDied);

void checkSpaceshipCollision(float spaceshipX, float spaceshipY, float alienStartX, float alienStartY, int& hearts) {
    int heart;
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 10; y++) {
            if (alienActive[x][y]) {
                int alienX = alienStartX + (y * 50);
                int alienY = alienStartY + (x * 80); 
                if (checkIfSpaceshipCollision(spaceshipX, spaceshipY, alienX, alienY)) {
                    if (hearts > 0) {
                        heart = eraseHeart(hearts); // Erase one heart
                        --hearts;
                        if (hearts == 0) {
                            bool playerDied = true;
                            GameOver(playerDied);
                            return;
                        }
                        eraseAliens(alienX, alienY);
                        eraseSpaceship(spaceshipX, spaceshipY, spaceshipX + 31, spaceshipY + 20);
                        startGame(true, heart);
                    }
                }
            }
        }
    }
}



void checkBulletCollision(float alienStartX, float alienStartY) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bulletActive[i]) {
            for (int x = 0; x < 5; x++) {
                for (int y = 0; y < 10; y++) {
                    if (alienActive[x][y]) {
                        int alienX = alienStartX + (y * 50);
                        int alienY = alienStartY + (x * 80);
                        // Check if bullet hits the alien
                        if (checkIfBulletCollision(bulletX[i], bulletY[i], alienX, alienY)) {
                            eraseAlien(alienX, alienY); // Remove the alien from the screen
                            alienActive[x][y] = false;  // Mark the alien as inactive
                            bulletActive[i] = false;   // Deactivate the bullet
                            eraseBullet(bulletX[i], bulletY[i]); // Erase the bullet
                            prev_score = score;
                            score += 10; // Increment score for successful hit
                        }
                    }
                }
            }
        }
    }
    GameOver(false);
}

void drawPauseMenu();

void startGame(bool score, int heart) {
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
    bool move = true;
  
    spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);

    while (true) {
        drawGameBox(score, heart);
        eraseAliens(prevAlienX, prevAlienY);
        moveAliens(alienStartX, alienStartY, prevAlienX, prevAlienY, moveDown, move);
        drawAliens(alienStartX, alienStartY);
		checkSpaceshipCollision(initial_x, initial_y, alienStartX, alienStartY, heartStatus);
        updateBullets();
        checkBulletCollision(alienStartX, alienStartY);

        // Check for key presses to move the spaceship
        bool keyPressed = isKeyPressed(whichKey);
        if (keyPressed == true) {
            if (whichKey == 1) {
                if (initial_x - 1 > 150) {
                    eraseSpaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    initial_x -= 10;
                    spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    checkSpaceshipCollision(initial_x, initial_y, alienStartX, alienStartY, heartStatus);
                }
            }
            else if (whichKey == 2) {
                if (initial_y - 1 > 160) {
                    eraseSpaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    initial_y -= 10;
                    spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    checkSpaceshipCollision(initial_x, initial_y, alienStartX, alienStartY, heartStatus);
                }
            }
            else if (whichKey == 3) {
                if (initial_x - 1 < 1695) {
                    eraseSpaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    initial_x += 10;
                    spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    checkSpaceshipCollision(initial_x, initial_y, alienStartX, alienStartY, heartStatus);
                }                
            }
            else if (whichKey == 4) {
                if (initial_y - 1 < 795) {
                    eraseSpaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    initial_y += 10;
                    spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);
                    checkSpaceshipCollision(initial_x, initial_y, alienStartX, alienStartY, heartStatus);
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
            else if (whichKey == 7) {
                move = false;
                system("cls");
                drawPauseMenu();
                move = true;
            }
        }
    }
}

void drawPauseMenu() {
    int selectedOption = 2; // Default
    int whichKey;

    while (true) {
        drawText(50, 600, 50, 255, 255, 255, "Space Explorers");
        drawText(30, 650, 250, 255, 255, 255, "Game Paused");

        int startColorR = 255, startColorG = 255, startColorB = 255;
        int resumeColorR = 255, resumeColorG = 255, resumeColorB = 255;
        int exitColorR = 255, exitColorG = 255, exitColorB = 255;
        int saveexitColorR = 255, saveexitColorG = 255, saveexitColorB = 255;

        if (selectedOption == 1) {
            startColorG = 255; startColorR = 0; startColorB = 0;
        }
        else if (selectedOption == 2) {
            resumeColorG = 255; resumeColorR = 0; resumeColorB = 0;
        }
        else if (selectedOption == 3) {
            exitColorR = 255; exitColorG = 0; exitColorB = 0;
        }
        else if (selectedOption == 4) {
            saveexitColorG = 255; saveexitColorR = 0; saveexitColorB = 0;
        }

        // Start New Game Box
        myLine(750, 350, 1250, 350, startColorR, startColorG, startColorB);
        myLine(750, 350, 750, 425, startColorR, startColorG, startColorB);
        myLine(750, 425, 1250, 425, startColorR, startColorG, startColorB);
        myLine(1250, 350, 1250, 425, startColorR, startColorG, startColorB);
        drawText(25, 770, 360, startColorR, startColorG, startColorB, "Start New Game");

        // Resume Game Box
        myLine(750, 500, 1250, 500, resumeColorR, resumeColorG, resumeColorB);
        myLine(750, 500, 750, 575, resumeColorR, resumeColorG, resumeColorB);
        myLine(750, 575, 1250, 575, resumeColorR, resumeColorG, resumeColorB);
        myLine(1250, 500, 1250, 575, resumeColorR, resumeColorG, resumeColorB);
        drawText(25, 770, 510, resumeColorR, resumeColorG, resumeColorB, "Resume Game");

        // Exit 
        myLine(750, 650, 1250, 650, exitColorR, exitColorG, exitColorB);
        myLine(750, 650, 750, 725, exitColorR, exitColorG, exitColorB);
        myLine(750, 725, 1250, 725, exitColorR, exitColorG, exitColorB);
        myLine(1250, 650, 1250, 725, exitColorR, exitColorG, exitColorB);
        drawText(25, 770, 660, exitColorR, exitColorG, exitColorB, "Exit");

        // Save and Exit 
        myLine(750, 800, 1250, 800, saveexitColorR, saveexitColorG, saveexitColorB);
        myLine(750, 800, 750, 875, saveexitColorR, saveexitColorG, saveexitColorB);
        myLine(750, 875, 1250, 875, saveexitColorR, saveexitColorG, saveexitColorB);
        myLine(1250, 800, 1250, 875, saveexitColorR, saveexitColorG, saveexitColorB);
        drawText(25, 770, 810, saveexitColorR, saveexitColorG, saveexitColorB, "Save and Exit");

        if (isKeyPressed(whichKey)) {
            if (whichKey == 2) {
                selectedOption--;
                if (selectedOption < 1) 
                    selectedOption = 4; // Wrap around
            }
            else if (whichKey == 4) {
                selectedOption++;
                if (selectedOption > 4) 
                    selectedOption = 1; // Wrap around
            }
            else if (whichKey == 5) {
                if (selectedOption == 1) {
                    system("cls");
                    prev_score = 0;
                    score = 0;
                    for (int i = 0; i < 5; i++) {
                        for (int j = 0; i < 10; j++) {
                            alienActive[i][j] = true;
                        }
                    }                 
                    drawAliens(1250, 200);
                    startGame(false, 0);
                    return;
                }
                else if (selectedOption == 2) {
                    system("cls");
                    return;
                }
                else if (selectedOption == 3) {
                    exit(0);
                }
                else if (selectedOption == 4) {
                    exit(0);
                }
            }
        }
    }
}

void drawMenu() {
    static int selectedOption = 1; // default
    int whichKey;
    drawText(50, 600, 50, 255, 255, 255, "Space Explorers");

    int startColorR = 255, startColorG = 255, startColorB = 255;
    int scoresColorR = 255, scoresColorG = 255, scoresColorB = 255;
    int exitColorR = 255, exitColorG = 255, exitColorB = 255;
    if (selectedOption == 1) {
        startColorG = 255; startColorR = 0; startColorB = 0;
    }
    else if (selectedOption == 2) {
        scoresColorG = 255; scoresColorR = 0; scoresColorB = 0;
    }
    else if (selectedOption == 3) {
        exitColorR = 255; exitColorG = 0; exitColorB = 0;
    }

    // Start New Game Box
    myLine(700, 350, 1250, 350, startColorR, startColorG, startColorB);
    myLine(700, 350, 700, 425, startColorR, startColorG, startColorB);
    myLine(700, 425, 1250, 425, startColorR, startColorG, startColorB);
    myLine(1250, 350, 1250, 425, startColorR, startColorG, startColorB);
    drawText(25, 720, 360, startColorR, startColorG, startColorB, "Start New Game");

    // View Previous Scores Box
    myLine(700, 500, 1250, 500, scoresColorR, scoresColorG, scoresColorB);
    myLine(700, 500, 700, 575, scoresColorR, scoresColorG, scoresColorB);
    myLine(700, 575, 1250, 575, scoresColorR, scoresColorG, scoresColorB);
    myLine(1250, 500, 1250, 575, scoresColorR, scoresColorG, scoresColorB);
    drawText(25, 720, 510, scoresColorR, scoresColorG, scoresColorB, "View Previous Scores");

    // Exit Box
    myLine(700, 650, 1250, 650, exitColorR, exitColorG, exitColorB);
    myLine(700, 650, 700, 725, exitColorR, exitColorG, exitColorB);
    myLine(700, 725, 1250, 725, exitColorR, exitColorG, exitColorB);
    myLine(1250, 650, 1250, 725, exitColorR, exitColorG, exitColorB);
    drawText(25, 720, 660, exitColorR, exitColorG, exitColorB, "Exit");

    if (isKeyPressed(whichKey)) {
        if (whichKey == 2) { // Arrow Up
            selectedOption--;
            if (selectedOption < 1) selectedOption = 3; // Wrap around
        }
        else if (whichKey == 4) { // Arrow Down
            selectedOption++;
            if (selectedOption > 3) selectedOption = 1; // Wrap around
        }
        else if (whichKey == 5) {
            if (selectedOption == 1) {
                system("cls");
                startGame(false, 0);
            }
            else if (selectedOption == 2) {
                return;
            }
            else if (selectedOption == 3) {
                exit(0);
            }
        }
    }
}

bool checkAliensDead() {
    bool check = false;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (alienActive[i][j] == false)
                check = true;
            else
                return false;
            
        }
    }
    if (check)
        return true;
    else
        return false;
}

void drawWinGame() {
    int selectedOption = 1; // Default
    int whichKey;
    while (true) {
        drawText(50, 600, 50, 255, 255, 255, "Space Explorers");
        drawText(50, 750, 200, 0, 255, 0, "YOU WIN!");

        int startColorR = 255, startColorG = 255, startColorB = 255;
        int exitColorR = 255, exitColorG = 255, exitColorB = 255;
        int saveexitColorR = 255, saveexitColorG = 255, saveexitColorB = 255;

        if (selectedOption == 1) {
            startColorG = 255; startColorR = 0; startColorB = 0;
        }
        else if (selectedOption == 2) {
            exitColorR = 255; exitColorG = 0; exitColorB = 0;
        }
        else if (selectedOption == 3) {
            saveexitColorG = 255; saveexitColorR = 0; saveexitColorB = 0;
        }

        // Start New Game Box
        myLine(700, 350, 1200, 350, startColorR, startColorG, startColorB);
        myLine(700, 350, 700, 425, startColorR, startColorG, startColorB);
        myLine(700, 425, 1200, 425, startColorR, startColorG, startColorB);
        myLine(1200, 350, 1200, 425, startColorR, startColorG, startColorB);
        drawText(25, 720, 360, startColorR, startColorG, startColorB, "Start New Game");

        // Exit 
        myLine(700, 500, 1200, 500, exitColorR, exitColorG, exitColorB);
        myLine(700, 500, 700, 575, exitColorR, exitColorG, exitColorB);
        myLine(700, 575, 1200, 575, exitColorR, exitColorG, exitColorB);
        myLine(1200, 500, 1200, 575, exitColorR, exitColorG, exitColorB);
        drawText(25, 720, 510, exitColorR, exitColorG, exitColorB, "Exit");

        // Save and Exit 
        myLine(700, 650, 1200, 650, saveexitColorR, saveexitColorG, saveexitColorB);
        myLine(700, 650, 700, 725, saveexitColorR, saveexitColorG, saveexitColorB);
        myLine(700, 725, 1200, 725, saveexitColorR, saveexitColorG, saveexitColorB);
        myLine(1200, 650, 1200, 725, saveexitColorR, saveexitColorG, saveexitColorB);
        drawText(25, 720, 660, saveexitColorR, saveexitColorG, saveexitColorB, "Save and Exit");

        if (isKeyPressed(whichKey)) {
            if (whichKey == 2) {
                selectedOption--;
                if (selectedOption < 1)
                    selectedOption = 3; // Wrap around
            }
            else if (whichKey == 4) {
                selectedOption++;
                if (selectedOption > 3)
                    selectedOption = 1; // Wrap around
            }
            else if (whichKey == 5) {
                if (selectedOption == 1) {
                    system("cls");
                    prev_score = 0;
                    score = 0;
                    for (int i = 0; i < 5; i++) {
                        for (int j = 0; i < 10; j++) {
                            //alienActive[i][j] = true;
                        }
                    }
                    drawAliens(1250, 200);
                    startGame(true, 0);
                    return;
                }
                else if (selectedOption == 2) {
                    exit(0);
                }
                else if (selectedOption == 3) {
                    exit(0);
                }
            }
        }
    }
}

void drawLoseGame() {
    int selectedOption = 1; // Default
    int whichKey;
    while (true) {
        drawText(50, 600, 50, 255, 255, 255, "Space Explorers");
        drawText(50, 750, 200, 255, 0, 0, "YOU LOST!");

        int startColorR = 255, startColorG = 255, startColorB = 255;
        int exitColorR = 255, exitColorG = 255, exitColorB = 255;
        int saveexitColorR = 255, saveexitColorG = 255, saveexitColorB = 255;

        if (selectedOption == 1) {
            startColorG = 255; startColorR = 0; startColorB = 0;
        }
        else if (selectedOption == 2) {
            exitColorR = 255; exitColorG = 0; exitColorB = 0;
        }
        else if (selectedOption == 3) {
            saveexitColorG = 255; saveexitColorR = 0; saveexitColorB = 0;
        }

        // Start New Game Box
        myLine(700, 350, 1200, 350, startColorR, startColorG, startColorB);
        myLine(700, 350, 700, 425, startColorR, startColorG, startColorB);
        myLine(700, 425, 1200, 425, startColorR, startColorG, startColorB);
        myLine(1200, 350, 1200, 425, startColorR, startColorG, startColorB);
        drawText(25, 720, 360, startColorR, startColorG, startColorB, "Play Again");

        // Exit 
        myLine(700, 500, 1200, 500, exitColorR, exitColorG, exitColorB);
        myLine(700, 500, 700, 575, exitColorR, exitColorG, exitColorB);
        myLine(700, 575, 1200, 575, exitColorR, exitColorG, exitColorB);
        myLine(1200, 500, 1200, 575, exitColorR, exitColorG, exitColorB);
        drawText(25, 720, 510, exitColorR, exitColorG, exitColorB, "Exit");

        // Save and Exit 
        myLine(700, 650, 1200, 650, saveexitColorR, saveexitColorG, saveexitColorB);
        myLine(700, 650, 700, 725, saveexitColorR, saveexitColorG, saveexitColorB);
        myLine(700, 725, 1200, 725, saveexitColorR, saveexitColorG, saveexitColorB);
        myLine(1200, 650, 1200, 725, saveexitColorR, saveexitColorG, saveexitColorB);
        drawText(25, 720, 660, saveexitColorR, saveexitColorG, saveexitColorB, "Save and Exit");

        if (isKeyPressed(whichKey)) {
            if (whichKey == 2) {
                selectedOption--;
                if (selectedOption < 1)
                    selectedOption = 3; // Wrap around
            }
            else if (whichKey == 4) {
                selectedOption++;
                if (selectedOption > 3)
                    selectedOption = 1; // Wrap around
            }
            else if (whichKey == 5) {
                if (selectedOption == 1) {
                    system("cls");
                    prev_score = 0;
                    score = 0;
                    for (int i = 0; i < 5; i++) {
                        for (int j = 0; i < 10; j++) {
                            //alienActive[i][j] = true;
                        }
                    }
                    drawAliens(1250, 200);
                    startGame(true, 0);
                    return;
                }
                else if (selectedOption == 2) {
                    exit(0);
                }
                else if (selectedOption == 3) {
                    exit(0);
                }
            }
        }
    }
}


void GameOver(bool playerDied) {
    if (checkAliensDead()) {
        system("cls");
        drawWinGame();
        return;
    }
    if (playerDied) {
        system("cls");
        drawLoseGame();
        return;
    }
}

int main() {
    for (int x = 0; x < 5; x++) {
       for (int y = 0; y < 10; y++) {
           alienActive[x][y] = true;
        }
    }
    while (true) {
        drawMenu();
    }
    return 0;
}