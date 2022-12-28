#pragma once

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
            uint8_t y;

            bool active;
        };

        Pipe pipes[4];

        enum class GameState : uint8_t
        {
            Splashscreen,
            Title,
            Highscore,
            Credits,
            Options,
            Preview,
            Game,
            Gameover,
        };

    private:

        uint8_t cursorIndex;
        uint8_t cursorX;
        uint8_t cursorY;

        unsigned long currentTime;
        unsigned long millisecondTarget;

        float backgroundAx;
        float backgroundBx;

        float backgroundSpeed;

        uint8_t pipeSpeed;

        uint8_t pipeIndex;

    public:

        void gameLoop();
        void setup();
        void loop();
        void initialise();

        void splashscreen();  
        void titlescreen();
        void preview();
        void game();

        void drawTitlescreen();
        void drawPreview();
        void drawGame();
};