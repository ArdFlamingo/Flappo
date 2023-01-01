#pragma once
#include <stdint.h>

class Game
{
    public:

        struct Player
        {
            float x;
            float y;

            float yVelocity;

            float gravity;

            float jumpVelocity;

            uint8_t radius = 5;
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

    public:

        void updateGame();
        void setup();
        void loop();
        void initialize();

        void updateSplashscreen();  
        void updateTitlescreen();
        void updatePreview();
        void updateGameplay();
        void updateGameover();

        void generatePipe();
        bool collision();

        void drawSplashscreen();
        void drawTitlescreen();
        void drawPreview();
        void drawGame();
        void drawPlayer();
        void drawGameover();

        void printScore();

    public:

        Game::GameState gameState = Game::GameState::Splashscreen;
        Game::Player player;
        Game::Pipe pipe;
};