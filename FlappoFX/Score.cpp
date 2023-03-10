#include <Arduboy2.h>
#include "Score.h"

uint8_t Score::countScoreDigits(uint16_t value)
{
    if (value > 999)
        return 4;

    else if (value > 99)
        return 3;

    else if (value > 9)
        return 2;

    else
        return 1;
}

uint8_t Score::calculateScoreStringWidth(uint8_t length)
{
    constexpr uint8_t characterWidth = Arduboy2::getCharacterWidth();
    constexpr uint8_t characterSpace = Arduboy2::getCharacterSpacing();

    return ((length * characterWidth) + ((length - 1) * characterSpace));
}

uint8_t Score::calculateCenterX(uint8_t width)
{
    return ((Arduboy2::width() - width) / 2);
}

uint8_t Score::calculateScoreX(uint8_t value)
{
    auto digits = countScoreDigits(value);
    auto width = calculateScoreStringWidth(digits);
    auto x = calculateCenterX(width);

    return x;
}