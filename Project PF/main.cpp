#include <iostream>
#include "help.h"
#include <cstdlib> 
#include <string>
#include <fstream>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib") // link with the winmm library

using namespace std;

int spaceshipX, spaceshipY;
float alienX, alienY;
bool moveDown;

const int MAX_BULLETS = 1000;
bool alienActive[5][10] = { true }; // Initialize all aliens as active

// arrays to store bullet positions
int bulletX[MAX_BULLETS] = { 0 };
int bulletY[MAX_BULLETS] = { 0 };
bool bulletActive[MAX_BULLETS] = { true };
int prev_score = 0;
int score = 0;
int heartStatus = 3;
bool playerDied = false;

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

void saveGameState(int spaceshipX, int spaceshipY, float alienX, float alienY, bool moveDown) {
    ofstream outFile("gamestate.txt");
    if (outFile.is_open()) {
        // Save spaceship position
        outFile << spaceshipX << " " << spaceshipY << endl;

        // Save alien positions and status
        outFile << alienX << " " << alienY << " " << moveDown << endl;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 10; j++) {
                outFile << alienActive[i][j] << " ";
            }
            outFile << endl;
        }

        // Save heart status
        outFile << heartStatus << endl;

        // Save score
        outFile << score << endl;

        outFile.close();
        cout << "Game state saved successfully!" << endl;
    }
    else {
        cout << "Unable to open file for saving game state." << endl;
    }
}

void loadGameState(int& spaceshipX, int& spaceshipY, float& alienX, float& alienY, bool& moveDown) {
    ifstream inFile("gamestate.txt");
    if (inFile.is_open()) {
        // Load spaceship position
        inFile >> spaceshipX >> spaceshipY;

        // Load alien positions and status
        inFile >> alienX >> alienY >> moveDown;
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 10; j++) {
                inFile >> alienActive[i][j];
            }
        }

        // Load heart status
        inFile >> heartStatus;

        // Load score
        inFile >> score;

        inFile.close();
        cout << "Game state loaded successfully!" << endl;
    }
    else {
        cout << "Unable to open file for loading game state." << endl;
    }
}

void saveScore(int currentScore, int heartStatus) {
    bool scoreExists = false;
    ifstream inFile("scores.txt");

    // Check if the score already exists
    int score, hearts;
    if (inFile.is_open()) {
        while (inFile >> score >> hearts) {
            if (score == currentScore && hearts == heartStatus) {
                scoreExists = true;
                break;
            }
        }
        inFile.close();
    }
    else {
        cout << "Unable to open file for reading." << endl;
    }

    // Save the current score and heart status if it is not already present
    if (!scoreExists) {
        ofstream outFile("scores.txt", ios::app);
        if (outFile.is_open()) {
            outFile << currentScore << " " << heartStatus << endl;
            outFile.close();
            cout << "Score and heart status saved successfully!" << endl; // Confirmation message
        }
        else {
            cout << "Unable to open file for writing." << endl;
        }
    }
    else {
        cout << "Score and heart status already exist. Not saved." << endl;
    }
}

// Function to display the 5 highest scores
void displayHighScores() {
    ifstream inFile("highscores.txt");
    int score;
    int rank = 1;

    if (inFile.is_open()) {
        cout << "Top 5 High Scores:" << endl;
        while (inFile >> score) {
            cout << rank << ". " << score << endl;
            rank++;
        }
        inFile.close();
    }
    else {
        cout << "Unable to open file for reading." << endl;
    }
}

// Function to calculate the highest score
int calculateHighScore() {
    ifstream inFile("scores.txt");
    int score, hearts;
    int highestScore = 0;
    int leastHeartsErased = 4;
    int highScores[5] = { 0 }; // Array to store the top 5 scores

    if (inFile.is_open()) {
        while (inFile >> score >> hearts) {
            if ((score > highestScore) || (score == highestScore && hearts < leastHeartsErased)) {
                highestScore = score;
                leastHeartsErased = hearts;
            }
            for (int i = 0; i < 5; i++) {
                if (score > highScores[i]) {
                    // Shift lower scores down
                    for (int j = 4; j > i; j--) {
                        highScores[j] = highScores[j - 1];
                    }
                    highScores[i] = score;
                    break;
                }
            }
        }
        inFile.close();
    }
    else {
        cout << "Unable to open file for reading." << endl;
    }
    ofstream outFile("highscores.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < 5; i++) {
            outFile << highScores[i] << endl;
        }
        outFile.close();
    }
    else {
        cout << "Unable to open file for writing." << endl;
    }
    return highestScore;
}

void drawGameBox(bool ifScore) {
    // title 
    drawText(50, 600, 50, 255, 255, 255, "Space Explorers");
    int highScore = calculateHighScore();
    string highScoreText = "High Score: " + to_string(highScore);
    drawText(20, 150, 50, 255, 255, 0, highScoreText.c_str());
    string pscoreText = "Score: " + to_string(prev_score);
    if (prev_score != score) {
        drawText(20, 150, 100, 0, 0, 0, pscoreText.c_str());
    }
    string scoreText = "Score: " + to_string(score);
    drawText(20, 150, 100, 255, 255, 255, scoreText.c_str());

    drawText(15, 70, 1000, 255, 255, 255, "Press 'esc' to Pause the Game ");

    if (heartStatus == 3) {
        drawHeart(1500, 120, 1510, 110, 255, 0, 0); // first heart
        drawHeart(1550, 120, 1560, 110, 255, 0, 0); // second heart
        drawHeart(1600, 120, 1610, 110, 255, 0, 0); // third heart
    }
    else if (heartStatus == 2) {
        drawHeart(1500, 120, 1510, 110, 255, 0, 0); // first heart   
        drawHeart(1550, 120, 1560, 110, 255, 0, 0); // second heart
        drawHeart(1600, 120, 1610, 110, 192, 192, 192); // third heart erased
    }
    else if (heartStatus == 1) {
        drawHeart(1500, 120, 1510, 110, 255, 0, 0); // first heart   
        drawHeart(1550, 120, 1560, 110, 192, 192, 192); // second heart erased
        drawHeart(1600, 120, 1610, 110, 192, 192, 192); // third heart erased
    }
    else if (heartStatus == 0) {
        drawHeart(1500, 120, 1510, 110, 192, 192, 192); // first heart erased
        drawHeart(1550, 120, 1560, 110, 192, 192, 192); // second heart erased
        drawHeart(1600, 120, 1610, 110, 192, 192, 192); // third heart erased
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
    }
    if (num == 2) {
        drawHeart(1550, 120, 1560, 110, 192, 192, 192); // erase second heart
    }
    if (num == 3) {
        drawHeart(1600, 120, 1610, 110, 192, 192, 192); // erase third heart
    }
    return num;
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

void eraseAliens(int x1, int y1);

void moveAliens(float& x1, float& y1, int& prevX, int& prevY, bool& moveDown, bool move) {
    if (move) {
        static int moveCounter = 0;
        prevX = x1;
        prevY = y1;
        eraseAliens(prevX, prevY);
        int x = x1, y = y1;

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
            int leftmostX = x1 + 9 * 50; // Initialize to the rightmost possible position
            for (int i = 0; i < 5; i++) {
                for (int j = 9; j > 0; j--) {
                    if (alienActive[i][j]) {
                        int x = x1 + j * 50;
                        if (x < leftmostX) {
                            leftmostX = x;
                        }
                    }
                }
            }

            if (leftmostX > 150) {
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

void startGame(bool score, bool restart);

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
                        PlaySound(TEXT("negativeBeeps.wav"), NULL, SND_FILENAME | SND_ASYNC);
                        if (hearts == 0) {
                            playerDied = true;
                            GameOver(playerDied);
                            return;
                        }
                        system("cls");
                        startGame(true, false);
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

void drawPauseMenu(int spaceshipX, int spaceshipY, float alienX, float alienY, bool moveDown);

void checkLeftBoundaryCollision(int alienStartX, int alienStartY);

void startGame(bool score, bool restart) {
    int whichKey;
    prev_score = score ? 0 : prev_score;
    if (restart) {
        PlaySound(TEXT("Countdown.wav"), NULL, SND_FILENAME | SND_ASYNC);
        system("cls");
        Sleep(3000);
        heartStatus = 3;
    }
    // Initial position for spaceship
    float initial_x = 300.0;
    float initial_y = 300.0;

    // Initial positions for aliens
    float alienStartX = 1250.0;
    float alienStartY = 200.0;
    int prevAlienX = alienStartX;
    int prevAlienY = alienStartY;
    bool moveDown = true;
    bool move = true;

    for (int i = 0; i < MAX_BULLETS; ++i) {
        bulletX[i] = 0;
        bulletY[i] = 0;
        bulletActive[i] = false;
    }

    spaceship(initial_x, initial_y, initial_x + 31, initial_y + 20);

    while (true) {
        drawGameBox(score);
        moveAliens(alienStartX, alienStartY, prevAlienX, prevAlienY, moveDown, move);
        drawAliens(alienStartX, alienStartY);
        checkLeftBoundaryCollision(alienStartX, alienStartY);
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
            else if (whichKey == 8) {
                PlaySound(TEXT("laserBullet.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play laser sound
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
                spaceshipX = initial_x;
                spaceshipY = initial_y;
                alienX = alienStartX;
                alienY = alienStartY;
                drawPauseMenu(spaceshipX, spaceshipY, alienX, alienY, moveDown);
                move = true;
            }
        }
    }
}

void resetAliens() {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 10; ++j) {
            alienActive[i][j] = true;
        }
    }
}

void drawPauseMenu(int spaceshipX, int spaceshipY, float alienX, float alienY, bool moveDown) {
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
                    resetAliens();
                    drawAliens(1250, 200);
                    startGame(false, true);
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
                    saveScore(score, heartStatus);
                    saveGameState(spaceshipX, spaceshipY, alienX, alienY, moveDown);
                    exit(0);
                }
            }
        }
    }
}

void drawMenu(int spaceshipX, int spaceshipY, float alienX, float alienY, bool moveDown) {
    static int selectedOption = 1; // default
    int whichKey;
    drawText(50, 600, 50, 255, 255, 255, "Space Explorers");
    drawText(20, 580, 800, 255, 255, 0, "Press the esc key to view your top 5 scores");

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
    drawText(25, 720, 510, scoresColorR, scoresColorG, scoresColorB, "Load Previous Game");

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
                startGame(false, true);
            }
            else if (selectedOption == 2) {
                system("cls");
                loadGameState(spaceshipX, spaceshipY, alienX, alienY, moveDown);
                startGame(false, false);
                return;
            }
            else if (selectedOption == 3) {
                exit(0);
            }
        }
        else if (whichKey == 7) {
            system("cls");
            displayHighScores();
            return;
        }
    }
}

void checkLeftBoundaryCollision(int alienStartX, int alienStartY) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (alienActive[i][j]) {
                int x = alienStartX + j * 50;
                int y = alienStartY + i * 80;
                if (x <= 150) { // Left game boundary
                    playerDied = true;
                    GameOver(playerDied);
                }
            }
        }
    }
    return;
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
    PlaySound(TEXT("YouWin.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play win sound
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
                    resetAliens();
                    drawAliens(1250, 200);
                    system("cls");
                    startGame(true, true);
                    return;
                }
                else if (selectedOption == 2) {
                    exit(0);
                }
                else if (selectedOption == 3) {
                    saveScore(score, heartStatus);
                    exit(0);
                }
            }
        }
    }
}

void drawLoseGame() {
    int selectedOption = 1; // Default
    int whichKey;
    PlaySound(TEXT("YouLose.wav"), NULL, SND_FILENAME | SND_ASYNC); // Play lose sound
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
                    resetAliens();
                    drawAliens(1250, 200);
                    system("cls");
                    startGame(true, true);
                    return;
                }
                else if (selectedOption == 2) {
                    exit(0);
                }
                else if (selectedOption == 3) {
                    saveScore(score, heartStatus);
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
        ofstream outFile("scores.txt", ios::app);
        if (!outFile.is_open()) {
            cout << "Unable to create file." << endl;
            return 1;
        }
        outFile.close();
        drawMenu(spaceshipX, spaceshipY, alienX, alienY, moveDown);
    }
    return 0;
}