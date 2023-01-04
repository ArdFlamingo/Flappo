#include <Arduboy2.h>
#include <time.h>
#include "Game.h"
#include "Score.h"
#include "Particles.h"
#include "Sprites.h"

Arduboy2 arduboy;

Score score;
Score::Highscore highscore;

Game::GameState gameState = Game::GameState::Splashscreen;
Game::Player player;
Game::Pipe pipe;

Particles particles;

    constexpr uint8_t pipeGenerationFrames = 85;
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
        currentTime = millis();
        player.x = 59;
        player.y = 27;

        backgroundAx = 0;
        backgroundBx = 128;

        backgroundSpeed = 0.1;

        player.gravity = 0.05;

        player.jumpVelocity = 1.2;

        pipeSpeed = 1;

        pipeGap = 25;

        drawPlayerGameover = true;

        for (auto & pipe : pipes) {pipe.x = -14; pipe.width = 14;}
    }

    void Game::loop()
    {
        if (!arduboy.nextFrame())
            return;

        arduboy.pollButtons();
        arduboy.clear();
        updateGame();
        score.loop();
        arduboy.display();
    }

    void Game::updateGame()
    {
        switch (gameState)
        {
            case GameState::Splashscreen:
                updateSplashscreen();
                drawSplashscreen();
                    break;

            case GameState::Title:
                updateTitlescreen();
                drawTitlescreen();
                    break;

            case GameState::Highscore:
                    break;

            case GameState::Credits:
                    break;

            case GameState::Options:
                    break;

            case GameState::Preview:
                updatePreview();
                drawPreview();
                    break;

            case GameState::Game:
                updateGameplay();
                drawGame();
                drawPlayer();
                    break;

            case GameState::Death:
                updateDeath();
                    break;

            case GameState::Gameover:
                updateGameover();
                drawGameover();
                    break;
        }
    }

    void Game::updateSplashscreen()
    {
        millisecondTargetSplashscreen = millis();
        millisecondTargetSplashscreen -= currentTime;

        if (millisecondTargetSplashscreen >= 3000) {gameState = GameState::Title;}
    }

    void Game::updateTitlescreen()
    {
        if (arduboy.justPressed(DOWN_BUTTON))
        {
            if (cursorIndex < lastOptionIndex) {++cursorIndex;}
        }

        if (arduboy.justPressed(UP_BUTTON))
        {
            if (cursorIndex > firstOptionIndex) {--cursorIndex;}  
        }

        cursorX = options[cursorIndex].x;
        cursorY = options[cursorIndex].y;

        if (arduboy.justPressed(A_BUTTON) && cursorIndex == 0) 
        {
            gameState = GameState::Preview;
            resetGame();
        }
    }

    void Game::updatePreview()
    {
        if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed (A_BUTTON) || arduboy.justPressed(B_BUTTON)) {gameState = GameState::Game;}
    }

    void Game::updateGameplay()
    {
        player.yVelocity += player.gravity;
        player.y += player.yVelocity;

        if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed (A_BUTTON) || arduboy.justPressed(B_BUTTON)) 
        {
            player.yVelocity = 0; 
            player.yVelocity -= player.jumpVelocity;
        }

        if ((player.y - player.radius) <= 0) {player.yVelocity = 0;}

        if (collision()) 
        {
            pipeSpeed = 0; backgroundSpeed = 0; 
            gameState = GameState::Death;
            particles.playerX = player.x;
            particles.playerY = player.y;
            particles.resetParticles();
        }

        for (auto & pipe : pipes)
        {
            if ((player.x - player.radius) == (pipe.x + pipe.width))
                ++score.gameScore;

            if (score.gameScore == (pipeDecreaseThreshold + 5) && pipeGap > 14)
            {
                pipeGap--;
                pipeDecreaseThreshold = score.gameScore;
            }
        }

        generatePipe();
    }

    void Game::generatePipe()
    {
        for (auto & pipe : pipes)
        {
            if (pipe.active) {pipe.x -= pipeSpeed;}
            if (pipe.x == -14) {pipe.active = false;}
             
            if (arduboy.everyXFrames(pipeGenerationFrames))
            {
                if (pipe.active)
                    continue;
                        pipe.x = WIDTH;
                        pipe.active = true;
                        pipe.topPipeHeight = random(6, 29);
                    break;
            }

            pipe.bottomPipeY = (pipe.topPipeHeight + pipeGap);
            pipe.bottomPipeHeight = (WIDTH - pipe.bottomPipeY);
        }    
    }

    bool Game::collision()
    {
        Rect playerHitbox (player.x, player.y, player.radius, player.radius);
        
        for (auto & pipe : pipes)
        {
            Rect topPipeHitbox (pipe.x, pipe.topPipeY, pipe.width, pipe.topPipeHeight);
            Rect bottomPipeHitbox (pipe.x, pipe.bottomPipeY, pipe.width, pipe.bottomPipeHeight);

            if (arduboy.collide(playerHitbox, topPipeHitbox) || arduboy.collide(playerHitbox, bottomPipeHitbox) || (player.y + player.radius) >= 63)
            {
                return true;
            }
        }

        return false;
    }

    void Game::updateDeath()
    {
        if (drawPlayerGameover)
        {
            if (arduboy.everyXFrames(100))
            {
                drawPlayerGameover = false;
            }

            drawPlayer();
        }
        else
        {   
            particles.updateParticles();
        }

        for (auto & particles : particles.particleArray)
        {
            if (particles.counter > 0)
                break;
                    
                gameState = GameState::Gameover;
        }

        for (auto & highscore : highscore.highscoreList)
        {
            arduboy.println(highscore);
        }
    }

    void Game::resetGame()
    {
        player.y = 27;
        player.yVelocity = 0;

        backgroundAx = 0;
        backgroundBx = 128;

        backgroundSpeed = 0.1;

        pipeSpeed = 1;

        pipeGap = 25;

        drawPlayerGameover = true;

        for (auto & pipe : pipes) {pipe.x = -14; pipe.width = 14; pipe.active = false;}

        score.gameScore = 0;

        for (auto & particles : particles.particleArray) {particles.xVelocity = 0; particles.yVelocity = 0;}
    }

    void Game::updateGameover()
    {
        if (arduboy.justPressed(A_BUTTON))
            gameState = GameState::Title;
    }

    void Game::drawSplashscreen()
    {
        Sprites::drawOverwrite(39, 7, logo, 0);
    }

    void Game::drawTitlescreen()
    {
        Sprites::drawOverwrite(0, 0, title, 0);
        Sprites::drawOverwrite(cursorX, cursorY, cursor, 0);
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
            if (backgroundAx > -128) {backgroundAx -= backgroundSpeed;}
                else {backgroundAx = 128;}
            if (backgroundBx > -128) {backgroundBx -= backgroundSpeed;}  
                else {backgroundBx = 128;} 

        if (gameState == GameState::Game)
        {
            Sprites::drawSelfMasked(backgroundAx, 32, background, 0);
            Sprites::drawSelfMasked(backgroundBx, 32, background, 0);
        }

        for (auto & pipe : pipes) 
        {
            arduboy.fillRect(pipe.x, pipe.topPipeY, pipe.width, pipe.topPipeHeight); 
            arduboy.fillRect(pipe.x, pipe.bottomPipeY, pipe.width, pipe.bottomPipeHeight);
        }

        score.printScore();
    }

    void Game::drawPlayer()
    {
        arduboy.fillCircle(player.x, player.y, player.radius, WHITE); 
    }

    void Game::drawGameover()
    {
        arduboy.setTextSize(2);
        arduboy.setCursor(10, 3);
        arduboy.print(F("GAME OVER"));

        arduboy.setTextSize(1);

        if (score.gameScore > highscore.highscoreList[0])
        {
            arduboy.setCursor(6, 30);
            arduboy.print(F("Spectacular Job Bro!"));
        }
        else
        {
            arduboy.setCursor(18, 30);
            arduboy.println(F("Better Luck Next"));
            arduboy.setCursorX(35);
            arduboy.print("Time Dude!");
        }
    }

    void Score::printScore()
    {
        arduboy.setTextSize(1);
        arduboy.setCursor(this->calculateScoreX(this->gameScore), 0);
        arduboy.print(this->gameScore);
    }  

    void Score::updateHighscore()
    {
        if (score.gameScore > highscore.highscoreList[0])
        {
            highscore.highscoreList[0] = gameScore;
        }
        else if (score.gameScore > highscore.highscoreList[1])
        {
            highscore.highscoreList[1] = gameScore;
        }
        else if (score.gameScore > highscore.highscoreList[2])
        {
            highscore.highscoreList[2] = gameScore;
        }
        else if (score.gameScore > highscore.highscoreList[3])
        {
            highscore.highscoreList[3] = gameScore;
        }
    } 


