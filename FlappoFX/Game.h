#pragma once
#include <stdint.h>

class Game
{
public:

    struct Player
    {
        static constexpr uint8_t radius = 5;

        static constexpr float gravity = 0.05;

        float x;
        float y;

        float yVelocity;

        float jumpVelocity;
    };

    struct Pipe
    {
        uint16_t x;
        uint8_t topPipeY;
        uint8_t bottomPipeY;

        uint8_t width;
        uint8_t topPipeHeight;
        uint8_t bottomPipeHeight;

        bool active;
    };

    Pipe pipes[2];

    enum class GameState : uint8_t
    {
        Splashscreen,
        Title,
        Highscore,
        Credits,
        Options,
        Preview,
        Game,
        Death,
        Gameover
    };

private:

    uint8_t cursorIndex;
    uint8_t cursorX;
    uint8_t cursorY;

    unsigned long currentTime;

    unsigned long millisecondTargetSplashscreen;

    float backgroundAx;
    float backgroundBx;

    float backgroundSpeed;

    uint8_t pipeSpeed;

    uint8_t pipeGap;

    bool drawPlayerGameover;

    uint16_t pipeDecreaseThreshold;

public:

    void updateGame();
    void setup();
    void loop();
    void initialize();
    void resetGame();

    void updateSplashscreen();  
    void updateTitlescreen();
    void updatePreview();
    void updateGameplay();
    void updateDeath();
    void updateGameover();

    void updateOptions();
    void updateHighscore();
    void updateCredits();

    void generatePipe();
    bool collision();

    void drawSplashscreen();
    void drawTitlescreen();
    void drawPreview();
    void drawGame();
    void drawDeath();
    void drawPlayer();
    void drawGameover();

    void drawOptions();
    void drawHighscore();
    void drawCredits();

    void printScore();
};