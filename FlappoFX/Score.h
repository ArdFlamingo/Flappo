#pragma once

class Score
{
    public:

        uint16_t gameScore;

        uint8_t countScoreDigits(uint8_t value);
        uint8_t calculateScoreStringWidth(uint8_t length);
        uint8_t calculateCenterX(uint8_t width);
        uint8_t calculateScoreX(uint8_t value);

        void printScore();
};