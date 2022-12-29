#include <Arduboy2.h>
#include "Game.h"
#include "Sprites.h"

constexpr uint8_t PIPE_ARRAY_SIZE = 2;

Arduboy2 arduboy;
Game::GameState gameState = Game::GameState::Splashscreen;
Game::Player player;
Game::Pipe pipe;

    constexpr uint8_t firstOptionIndex = 0;
    constexpr uint8_t lastOptionIndex = 3;

    constexpr Point options[] { { 59, 34 }, { 59, 41 }, {59, 48}, {59, 55}};

    void Game::setup()
    {
        arduboy.begin();
        arduboy.initRandomSeed();
        initialize();
    }

    void Game::initialize()
    {
        this->currentTime = millis();
        player.x = 59;
        player.y = 27;

        this->backgroundAx = 0;
        this->backgroundBx = 128;

        this->backgroundSpeed = 0.1;

        player.gravity = 0.05;

        player.jumpVelocity = 1.2;

        this->pipeSpeed = 1;

        this->pipeGap = 30;

        for (uint8_t i = 0; i < PIPE_ARRAY_SIZE; i++) {pipes[i].x = -14; pipes[i].width = 14;}
    }

    void Game::loop()
    {
        if (!arduboy.nextFrame())
            return;

        arduboy.pollButtons();
        arduboy.clear();
        gameLoop();
        arduboy.display();
    }

    void Game::gameLoop()
    {
        switch (gameState)
        {
            case GameState::Splashscreen:
                splashscreen();
                drawSplashscreen();
                    break;

            case GameState::Title:
                titlescreen();
                drawTitlescreen();
                    break;

            case GameState::Highscore:
                    break;

            case GameState::Credits:
                    break;

            case GameState::Options:
                    break;

            case GameState::Preview:
                preview();
                drawPreview();
                    break;

            case GameState::Game:
                game();
                drawGame();
                    break;

            case GameState::Gameover:
                    break;
        }
    }

    void Game::splashscreen()
    {
        this->millisecondTarget = millis();
        this->millisecondTarget -= this->currentTime;

        if (millisecondTarget >= 3000) {gameState = GameState::Title;}
    }

    void Game::titlescreen()
    {
        if (arduboy.justPressed(DOWN_BUTTON))
        {
            if (this->cursorIndex < lastOptionIndex) {++this->cursorIndex;}
        }

        if (arduboy.justPressed(UP_BUTTON))
        {
            if (this->cursorIndex > firstOptionIndex) {--this->cursorIndex;}  
        }

        this->cursorX = options[this->cursorIndex].x;
        this->cursorY = options[this->cursorIndex].y;

        if (arduboy.justPressed(A_BUTTON) && cursorIndex == 0) {gameState = GameState::Preview;}
    }

    void Game::preview()
    {
        if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed (A_BUTTON) || arduboy.justPressed(B_BUTTON)) {gameState = GameState::Game;}
    }

    void Game::game()
    {
        player.yVelocity += player.gravity;
        player.y += player.yVelocity;

        if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed (A_BUTTON) || arduboy.justPressed(B_BUTTON)) 
        {
            player.yVelocity = 0; 
            player.yVelocity -= player.jumpVelocity;
        }

        if ((player.y - player.radius) <= 0) {player.yVelocity = 0;}

        generatePipe();
        collision();
    }

    void Game::generatePipe()
    {
        for (auto & pipe : pipes)
        {
            if (pipe.active) {pipe.x -= pipeSpeed;}
            if (pipe.x == -14) {pipe.active = false;}
             
            if (arduboy.everyXFrames(80))
            {
                if (pipe.active)
                    continue;
                        pipe.x = WIDTH;
                        pipe.active = true;
                        pipe.topPipeHeight = random(10, 31);
                    break;
            }

            pipe.bottomPipeY = (pipe.topPipeHeight + pipeGap);
            pipe.bottomPipeHeight = (WIDTH - pipe.bottomPipeY);
        }    
    }

    bool Game::collision()
    {
        Rect playerHitbox (player.x, player.y, (player.radius - 1), (player.radius - 1));
        
        for (auto & pipe : pipes)
        {
            Rect topPipeHitbox (pipe.x, pipe.topPipeY, pipe.width, pipe.topPipeHeight);
            Rect bottomPipeHitbox (pipe.x, pipe.bottomPipeY, pipe.width, pipe.bottomPipeHeight);
        }

        return false;
    }

    void Game::drawSplashscreen()
    {
        Sprites::drawOverwrite(39, 7, logo, 0);
    }

    void Game::drawTitlescreen()
    {
        Sprites::drawOverwrite(0, 0, title, 0);
        Sprites::drawOverwrite(cursorX, cursorY, cursor, 0);

        arduboy.setCursor(0, 0);
        arduboy.print(cursorIndex);
    }

    void Game::drawPreview() 
    {
        arduboy.fillCircle(player.x, player.y, player.radius, WHITE); 
        
        Sprites::drawSelfMasked(0, 32, background, 0);

        arduboy.setCursor(15, 0);
        arduboy.print(F("PRESS ANY BUTTON"));
    }

    void Game::drawGame()
    {
        arduboy.fillCircle(player.x, player.y, player.radius, WHITE); 

        if (this->backgroundAx > -128 && gameState == GameState::Game) {backgroundAx -= backgroundSpeed;}
            else {backgroundAx = 128;}
        if (this->backgroundBx > -128 && gameState == GameState::Game) {backgroundBx -= backgroundSpeed;}  
            else {backgroundBx = 128;} 

        Sprites::drawSelfMasked(backgroundAx, 32, background, 0);
        Sprites::drawSelfMasked(backgroundBx, 32, background, 0);

        //for (auto & pipe : pipes) {arduboy.fillRect(pipe.x, pipe.topPipeY, pipe.width, pipe.topPipeHeight); arduboy.fillRect(pipe.x, pipe.bottomPipeY, pipe.width, pipe.bottomPipeHeight);}
        arduboy.fillRect(pipes[0].x, pipes[0].topPipeY, pipes[0].width, pipes[0].topPipeHeight); 
        arduboy.fillRect(pipes[0].x, pipes[0].bottomPipeY, pipes[0].width, pipes[0].bottomPipeHeight);

        arduboy.setCursor(0, 0);
        arduboy.println(pipes[0].x);
        arduboy.println(pipes[1].x);
    }


