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
        };

    public:

        float gravity = 0.022;

        float playerX;
        float playerY;

        void resetParticles();
        void updateParticles();

    public:

        Particle particleArray[20];
};