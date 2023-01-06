#pragma once

#include <stdint.h>
#include "Game.h"


class Particles
{
    public:

        struct Particle
        {
            public:

                float x;
                float y;

                float xVelocity;
                float yVelocity;

                float xForce;
                float yForce;

                uint8_t size = 1;

                uint8_t counter = 180;

                bool isActive() const
                {
                    return (this->counter > 0);
                }
        };

    public:

        float gravity = 0.022;

        float playerX;
        float playerY;

        void resetParticles();
        void updateParticles();
        void drawParticles();

    public:

        Particle particleArray[40];
};