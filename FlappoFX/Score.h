#pragma once
#include <stdint.h>

class Score     //this code is a modified example from @pharap from this thread: https://community.arduboy.com/t/is-there-a-way-to-anchor-text-to-the-middle-of-the-screen/10770/3
{
    public:

        uint16_t gameScore;
        uint16_t gameHighscore;

        uint16_t isHighscoreInitialized;

        uint8_t highscoreX;

        uint8_t countScoreDigits(uint16_t value);
        uint8_t calculateScoreStringWidth(uint8_t length);
        uint8_t calculateCenterX(uint8_t width);
        uint8_t calculateScoreX(uint8_t value);

        void printScore();
};