#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <EEPROM.h>

#include "Game.h"
#include "Score.h"
#include "Particles.h"
#include "src/Sprites.h"
#include "src/Tones.h"

#define EEPROM_STORAGE_SPACE_START_ARDUBOY EEPROM_STORAGE_SPACE_START + 1200

constexpr uint16_t initialized_eeprom = 1222;

Arduboy2 arduboy;

Score score;

Game::GameState gameState = Game::GameState::Splashscreen;
Game::Player player;
Game::Pipe pipe;

Particles particles;

ArduboyTones sound(arduboy.audio.enabled);

constexpr uint8_t pipeGenerationFrames = 85;
constexpr uint8_t firstOptionIndex = 0;
constexpr uint8_t lastOptionIndex = 3;

constexpr Point options[]
{
    { 59, 34 },
    { 59, 41 },
    { 59, 48 },
    { 59, 55 },
};

void Game::setup()
{
    arduboy.begin();
    arduboy.initRandomSeed();
    initialize();
    EEPROM.get(EEPROM_STORAGE_SPACE_START_ARDUBOY, score.gameHighscore);

    if (score.isHighscoreInitialized != initialized_eeprom)
    {
        score.gameHighscore = 0;
        score.isHighscoreInitialized = initialized_eeprom;
    }
} 

void Game::initialize()
{
    currentTime = millis();
    player.x = 59;
    player.y = 27;

    backgroundAx = 0;
    backgroundBx = 128;

    backgroundSpeed = 0.1;

    player.jumpVelocity = 1.2;

    pipeSpeed = 1;

    pipeGap = 25;

    for (auto & pipe : pipes)
    {
        pipe.x = -14;
        pipe.width = 14;
    }
}

void Game::loop()
{
    if (!arduboy.nextFrame())
        return;

    arduboy.pollButtons();
    arduboy.clear();
    updateGame();
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
            updateHighscore();
            drawHighscore();
            break;

        case GameState::Credits:
            updateCredits();
            drawCredits();
            break;

        case GameState::Options:
            updateOptions();
            drawOptions();
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
            drawDeath();
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

    if (millisecondTargetSplashscreen >= 3000)
        gameState = GameState::Title;
}

void Game::updateTitlescreen()
{
    if (arduboy.justPressed(DOWN_BUTTON))
        if (cursorIndex < lastOptionIndex)
            ++cursorIndex;

    if (arduboy.justPressed(UP_BUTTON))
        if (cursorIndex > firstOptionIndex)
            --cursorIndex;

    cursorX = options[cursorIndex].x;
    cursorY = options[cursorIndex].y;

    if(arduboy.justPressed(A_BUTTON))
        switch(cursorIndex)
        {
            case 0:
                resetGame();
                gameState = GameState::Preview;
                break;

            case 1:
                gameState = GameState::Options;
                break;

            case 2:
                gameState = GameState::Highscore;
                break;

            case 3:
                gameState = GameState::Credits;
                break;
        }
}

void Game::updatePreview()
{
    if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed (A_BUTTON) || arduboy.justPressed(B_BUTTON))
        gameState = GameState::Game;
}

void Game::updateGameplay()
{
    if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON) || arduboy.justPressed(LEFT_BUTTON) || arduboy.justPressed(RIGHT_BUTTON) || arduboy.justPressed (A_BUTTON) || arduboy.justPressed(B_BUTTON))
    {
        player.yVelocity = 0; 
        player.yVelocity -= player.jumpVelocity;
    }

    if ((player.y - player.radius) <= 0)
        player.yVelocity = 0;

    player.yVelocity += player.gravity;
    player.y += player.yVelocity;

    if (collision())
    {
        pipeSpeed = 0; backgroundSpeed = 0; 
        gameState = GameState::Death;
        particles.resetParticles(player.x, player.y);
        sound.tones(collisionTone);
    }

    for (auto & pipe : pipes)
    {
        if ((player.x - player.radius) == (pipe.x + pipe.width))
        {
            ++score.gameScore;
            sound.tones(pointTone);
        }

        if ((score.gameScore == (pipeDecreaseThreshold + 5)) && (pipeGap > 14))
        {
            --pipeGap;
            pipeDecreaseThreshold = score.gameScore;
        }
    }

    generatePipe();
}

void Game::generatePipe()
{
    for (auto & pipe : pipes)
    {
        if (pipe.active)
            pipe.x -= pipeSpeed;

        if (pipe.x == -14)
            pipe.active = false;
            
        if (arduboy.everyXFrames(pipeGenerationFrames))
        {
            if (pipe.active)
                continue;

            pipe.x = arduboy.width();
            pipe.active = true;
            pipe.topPipeHeight = random(6, 29);

            break;
        }

        pipe.bottomPipeY = (pipe.topPipeHeight + pipeGap);
        pipe.bottomPipeHeight = (arduboy.width() - pipe.bottomPipeY);
    }    
}

bool Game::collision()
{
    Rect playerHitbox { player.x, player.y, player.radius, player.radius };
    
    for (auto & pipe : pipes)
    {
        Rect topPipeHitbox { pipe.x, pipe.topPipeY, pipe.width, pipe.topPipeHeight };

        if (arduboy.collide(playerHitbox, topPipeHitbox))
            return true;

        Rect bottomPipeHitbox { pipe.x, pipe.bottomPipeY, pipe.width, pipe.bottomPipeHeight };

        if (arduboy.collide(playerHitbox, bottomPipeHitbox))
            return true;

        if ((player.y + player.radius) >= arduboy.height())
            return true;
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
    }
    else
    {   
        particles.updateParticles();
    }

    for (auto & particle : particles.particleArray)
    {
        if (particle.isActive())
            break;

        gameState = GameState::Gameover;
        sound.tones(gameoverTone);
        drawPlayerGameover = true;
    }
}

void Game::drawDeath()
{
    if (drawPlayerGameover)
    {
        drawPlayer();
    }
    else
    {
        particles.drawParticles();
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

    for (auto & pipe : pipes)
    {
        pipe.x = -14;
        pipe.width = 14;
        pipe.active = false;
    }

    score.gameScore = 0;

    for (auto & particle : particles.particleArray)
    {
        particle.xVelocity = 0;
        particle.yVelocity = 0;
    }
}

void Game::updateGameover()
{
    if (arduboy.justPressed(A_BUTTON))
        gameState = GameState::Title;

    EEPROM.put(EEPROM_STORAGE_SPACE_START_ARDUBOY, score.gameHighscore);
}

void Game::updateCredits()
{
    if (arduboy.justPressed(B_BUTTON))
        gameState = GameState::Title;
}

void Game::updateOptions()
{
    if (arduboy.justPressed(B_BUTTON))
        gameState = GameState::Title;

    if (arduboy.justPressed(A_BUTTON))
        arduboy.audio.toggle();

    if (arduboy.justPressed(RIGHT_BUTTON))
    {
        score.gameHighscore = 0;
        EEPROM.put(EEPROM_STORAGE_SPACE_START_ARDUBOY, score.gameHighscore);
    }
}

void Game::updateHighscore()
{
    if (arduboy.justPressed(B_BUTTON))
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

    arduboy.setTextSize(1);
    arduboy.setCursor(15, 0);
    arduboy.print(F("PRESS ANY BUTTON"));
}

void Game::drawGame()
{
    if (backgroundAx > -128)
        backgroundAx -= backgroundSpeed;
    else
        backgroundAx = 128;

    if (backgroundBx > -128)
        backgroundBx -= backgroundSpeed;
    else
        backgroundBx = 128;

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

    this->printScore();
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

    if (score.gameScore >= score.gameHighscore)
    {
        arduboy.setCursor(6, 30);
        arduboy.print(F("Spectacular Job Bro!"));
        score.gameHighscore = score.gameScore;
    }
    else
    {
        arduboy.setCursor(18, 30);
        arduboy.println(F("Better Luck Next"));
        arduboy.setCursorX(35);
        arduboy.print(F("Time Dude!"));
    }
}

void Game::drawCredits()
{
    Sprites::drawOverwrite(0, 0, credits, 0);
}

void Game::drawOptions()
{
    if (arduboy.audio.enabled())
    {
        arduboy.setTextSize(1);
        arduboy.setCursor(0, 0);
        arduboy.println(F("Sound:On"));
    }
    else
    {
        arduboy.setTextSize(1);
        arduboy.setCursor(0, 0);
        arduboy.println(F("Sound:Off"));
    }

    arduboy.print(F("RIGHT:Clear Highscore"));
}

void Game::drawHighscore()
{
    Sprites::drawOverwrite(0, 0, highscoreTitle, 0);

    uint8_t highscoreX = 58;

    if (score.gameHighscore > 999)
        highscoreX = 40;

    else if (score.gameHighscore > 99)
        highscoreX = 46;

    else if (score.gameHighscore > 9)
        highscoreX = 52;

    arduboy.setTextSize(2);
    arduboy.setCursor(highscoreX, 35);
    arduboy.print(score.gameHighscore);
}

void Game::printScore()
{
    arduboy.setTextSize(1);
    arduboy.setCursor(score.calculateScoreX(score.gameScore), 0);
    arduboy.print(score.gameScore);
}