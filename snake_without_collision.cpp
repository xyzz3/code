#include <iostream>
#include <Windows.h>
#include <list>

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 30; // Reduced screen height for better visibility

struct sSnakeSegment {
    int x;
    int y;
};

int main() {
    // Create screen buffer
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwByteWritten = 0;
    list<sSnakeSegment> snake = { {60,15},{61,15},{62,15},{63,15},{64,15},{65,15},{66,15},{67,15},{68,15},{69,15} };

    int nFoodX = 30;
    int nFoodY = 15;
    int nScore = 0;
    int nSnakeDirection = 1; // Initially moving right
    bool bDead = false;
    bool bKeyLeft = false;
    bool bKeyRight = false;
    bool bKeyLeftOld = false;
    bool bKeyRightOld = false;

    while (1) {
        // Input
        // Get input
        bKeyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0; // Right arrow key
        bKeyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0; // Left arrow key

        if (bKeyRight && !bKeyRightOld) {
            nSnakeDirection = (nSnakeDirection + 1) % 4;
        }
        if (bKeyLeft && !bKeyLeftOld) {
            nSnakeDirection = (nSnakeDirection + 3) % 4;
        }
        bKeyRightOld = bKeyRight;
        bKeyLeftOld = bKeyLeft;

        // Game Logic
        // Update snake position
        switch (nSnakeDirection) {
        case 0: // up
            snake.push_front({ snake.front().x, snake.front().y - 1 });
            break;
        case 1: // right
            snake.push_front({ snake.front().x + 1, snake.front().y });
            break;
        case 2: // down
            snake.push_front({ snake.front().x, snake.front().y + 1 });
            break;
        case 3: // left
            snake.push_front({ snake.front().x - 1, snake.front().y });
            break;
        }

        // Collision detection and other game logic...
        // (Your existing collision detection and game logic code)







         // Chop off snake tail
        snake.pop_back();

        // Check if snake head collides with food
        if (snake.front().x == nFoodX && snake.front().y == nFoodY) {
            nScore++;

            // Place new food at random location
            nFoodX = rand() % nScreenWidth;
            nFoodY = rand() % nScreenHeight;

            // Increase snake length
            snake.push_back(snake.back());
        }

        // Clear screen 
        for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';

        // Draw the snake and other game elements
        for (auto s : snake) screen[s.y * nScreenWidth + s.x] = L'O'; // Draw snake body
        screen[snake.front().y * nScreenWidth + snake.front().x] = L'X'; // Draw snake head
        screen[nFoodY * nScreenWidth + nFoodX] = L'@'; // Draw food

        // Display stats
        wsprintf(&screen[2 * nScreenWidth + 5], L"JERIN'S cute SNAKE Hehee || SCORE: %d", nScore);

        // Display instructions
        if (bDead)
            wsprintf(&screen[15 * nScreenWidth + 40], L" PRESS 'SPACE' TO PLAY AGAIN ");

        // Display frame
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwByteWritten);

        // Introduce delay to control the speed of the game
        Sleep(100); // Adjust the sleep duration as needed to control the game speed
    }

    // Wait for space to restart the game
    while (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) {}



    delete[] screen;

    return 0;
}
