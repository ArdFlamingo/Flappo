#include <Arduboy2.h>
#include "Game.h"
#include "Sprites.h"

#define PIPE_ARRAY_SIZE 2

Arduboy2 ab;
Game::GameState gameState = Game::GameState::Splashscreen;
Game::Player player;
Game::Pipe pipe;

    constexpr uint8_t firstOptionIndex = 0;
    constexpr uint8_t lastOptionIndex = 3;

    constexpr Point options[] { { 59, 34 }, { 59, 41 }, {59, 48}, {59, 55}};

    void Game::setup()
    {
        ab.begin();
        ab.initRandomSeed();
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

        for (uint8_t i = 0; i < PIPE_ARRAY_SIZE; i++) {pipes[i].x = -14;}
    }

    void Game::loop()
    {
        if (!ab.nextFrame())
            return;

        ab.pollButtons();
        ab.clear();
        gameLoop();
        ab.display();
    }

    void Game::gameLoop()
    {
        switch (gameState)
        {
            case GameState::Splashscreen:
                splashscreen();
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
        Sprites::drawOverwrite(39, 7, logo, 0);

        this->millisecondTarget = millis();
        this->millisecondTarget -= this->currentTime;

        if (millisecondTarget >= 3000) {gameState = GameState::Title;}
    }

    void Game::titlescreen()
    {
        if (ab.justPressed(DOWN_BUTTON))
        {
            if (this->cursorIndex < lastOptionIndex) {++this->cursorIndex;}
        }

        if (ab.justPressed(UP_BUTTON))
        {
            if (this->cursorIndex > firstOptionIndex) {--this->cursorIndex;}  
        }

        this->cursorX = options[this->cursorIndex].x;
        this->cursorY = options[this->cursorIndex].y;

        if (ab.justPressed(A_BUTTON) && cursorIndex == 0) {gameState = GameState::Preview;}
    }

    void Game::preview()
    {
        if (ab.justPressed(UP_BUTTON) || ab.justPressed(DOWN_BUTTON) || ab.justPressed(LEFT_BUTTON) || ab.justPressed(RIGHT_BUTTON) || ab.justPressed (A_BUTTON) || ab.justPressed(B_BUTTON)) {gameState = GameState::Game;}
    }

    void Game::game()
    {
        player.yVelocity += player.gravity;
        player.y += player.yVelocity;

        if (ab.justPressed(UP_BUTTON) || ab.justPressed(DOWN_BUTTON) || ab.justPressed(LEFT_BUTTON) || ab.justPressed(RIGHT_BUTTON) || ab.justPressed (A_BUTTON) || ab.justPressed(B_BUTTON)) 
        {
            player.yVelocity = 0; 
            player.yVelocity -= player.jumpVelocity;
        }

        if ((player.y - player.radius) <= 0) {player.yVelocity = 0;}



        for (auto & pipe : pipes)
        {
            if (pipe.active) {pipe.x -= pipeSpeed;}
            if (pipe.x == -14) {pipe.active = false;}
             
            if (ab.everyXFrames(80))
            {
                    if (pipe.active)
                        continue;
                            pipe.x = WIDTH;
                            pipe.active = true;
                        break;
            }
        }    
    }

    void Game::drawTitlescreen()
    {
        Sprites::drawOverwrite(0, 0, title, 0);
        Sprites::drawOverwrite(cursorX, cursorY, cursor, 0);

        ab.setCursor(0, 0);
        ab.print(cursorIndex);
    }

    void Game::drawPreview() 
    {
        ab.fillCircle(player.x, player.y, player.radius, WHITE); 
        
        Sprites::drawSelfMasked(0, 32, background, 0);

        ab.setCursor(15, 0);
        ab.print(F("PRESS ANY BUTTON"));
    }

    void Game::drawGame()
    {
        ab.fillCircle(player.x, player.y, player.radius, WHITE); 

        if (this->backgroundAx > -128 && gameState == GameState::Game) {backgroundAx -= backgroundSpeed;}
            else {backgroundAx = 128;}
        if (this->backgroundBx > -128 && gameState == GameState::Game) {backgroundBx -= backgroundSpeed;}  
            else {backgroundBx = 128;} 

        Sprites::drawSelfMasked(backgroundAx, 32, background, 0);
        Sprites::drawSelfMasked(backgroundBx, 32, background, 0);

        for (auto & pipe : pipes) {ab.fillRect(pipe.x, pipe.y, 14, 20); ab.fillRect(pipe.x, pipe.y + 50, 14, 20);}
        
        ab.setCursor(0, 0);
        ab.println(pipes[0].x);
        ab.println(pipes[1].x);
    }


