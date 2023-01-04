#include <Arduboy2.h>
#include "Particles.h"

Game::Player gamePlayer;


void Particles::updateParticles()
{
    for (auto & particles : particleArray)
    {
        particles.yVelocity += this->gravity * particles.size;
        
        particles.yVelocity -= particles.yForce / (particles.size * 0.5);
        particles.xVelocity += particles.xForce / (particles.size * 0.15);

        particles.xForce *= 0.95;
        particles.yForce *= 0.95;

        particles.x += particles.xVelocity;
        particles.y += particles.yVelocity;

        if (particles.counter > 0)
        {
            particles.counter--;
            Arduboy2::drawRect(particles.x, particles.y, particles.size, particles.size);
        }
    }
}

void Particles::resetParticles()
{
    for (auto & particles : particleArray)
    {
        particles.size = random(2, 7);
        particles.counter = random(85, 100);

        particles.x = random(playerX - 5, playerX + 5 + 1);
        particles.y = random(playerY - 5, playerY + 5 + 1);

        particles.xForce = random(-5, 6) / 100.0;
        particles.yForce = random(25, 50) / 100.0;
    }
}